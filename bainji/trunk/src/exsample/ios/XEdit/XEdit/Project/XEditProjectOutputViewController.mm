//
//  XEditProjectOutputViewController.mm
//  XEdit
//
//  Created by DFSX on 2018/8/2.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "XEditProjectOutputViewController.h"
#import "XEditProjectOutputTableViewCell.h"
#import "XEditProjectOutputProgressView.h"

#import "XEditGenerateObserver.hpp"

#include "IXEngine.h"
#include "ITimeLine.h"

#define kOUTPUTTYPECELL @"outputTypeCell"
#define kOUTPUTTYPEHEIGHT 53

@interface XEditProjectOutputViewController () <UITableViewDelegate, UITableViewDataSource>

@property (weak, nonatomic) IBOutlet NSLayoutConstraint *scrollViewWidthConstraint;
@property (weak, nonatomic) IBOutlet UIImageView *thumbnailImageView;
@property (weak, nonatomic) IBOutlet UITableView *outputTypeTableView;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *outputTypeTableViewHeightConstraint;
@property (weak, nonatomic) IBOutlet UIButton *outputButton;

@property (strong, nonatomic) XEditProjectOutputProgressView *progressView;

@property (copy, nonatomic) NSArray<XEditOutputType *> *outputTypes;

@property (strong, nonatomic) XEditProjectOutputTableViewCell *currentSelectedCell;
@property (strong, nonatomic) XEditOutputType *currentSelectedOutputType;

@property (strong, nonatomic) NSDateFormatter *dateFormatter;

@end

@implementation XEditProjectOutputViewController {
    
    xedit::ITimeLine *timeline;
    XEditGenerateObserver *generateObserver;
    
}

- (void)viewDidLoad {
    [super viewDidLoad];
    _scrollViewWidthConstraint.constant = kSCREENWIDTH;
    [_outputTypeTableView registerNib:[UINib nibWithNibName:@"XEditProjectOutputTableViewCell" bundle:[NSBundle xEditBundle]] forCellReuseIdentifier:kOUTPUTTYPECELL];
    
    _outputTypes = [NSArray arrayWithObjects:[XEditOutputType outputType1080P], [XEditOutputType outputType720P], [XEditOutputType outputType480P], nil];
    _outputTypeTableViewHeightConstraint.constant = kOUTPUTTYPEHEIGHT * _outputTypes.count;
    [_outputTypeTableView reloadData];
    
    timeline = xedit::IXEngine::getSharedInstance()->getTimeLine();
    generateObserver = new XEditGenerateObserver(self);
    
    if (_firstFramePath && ![_firstFramePath isEqualToString:@""]) {
        _thumbnailImageView.image = [UIImage imageWithContentsOfFile:_firstFramePath];
    }
}

- (void)showProgressView {
    if (!_progressView) {
        _progressView = [XEditProjectOutputProgressView projectOutputProgressView];
        kWeakSelf(self);
        _progressView.didCanceled = ^{
            kStrongSelf(self);
            if (!self) {
                return;
            }
            self->timeline->cancelGenerate();
        };
    }
    [[UIApplication sharedApplication].keyWindow addSubview:_progressView];
    [_progressView autoPinEdgesToSuperviewEdges];
}

- (xedit::EncodeParam)getEncodeParamWithOutputType:(XEditOutputType *)outputType {
    xedit::EncodeParam encodeParam;
    memset(&encodeParam, 0, sizeof(encodeParam));
    encodeParam.eMuxerType = EMUXER_TYPE_MOV;
    
    xedit::VideoCodecParam videoCodecParam;
    memset(&videoCodecParam, 0, sizeof(videoCodecParam));
    videoCodecParam.bInterlaced = NO;
    videoCodecParam.bTopFieldFirst = NO;
    videoCodecParam.ePixFormat = EPIX_FMT_NV12;
    videoCodecParam.nWidth = outputType.width;
    videoCodecParam.nHeight = outputType.height;
    videoCodecParam.eBitrateMode = EBITRATE_MODE_VBR;
    videoCodecParam.eCodecID = ECODEC_ID_H264;
    videoCodecParam.eCodecLevel = ECODEC_LEVEL_NONE;
    videoCodecParam.eCodecProfile = ECODEC_PROFILE_NONE;
    videoCodecParam.nBitrate = 8 * 1000000;
    encodeParam.videoParam = videoCodecParam;
    
    xedit::AudioCodecParam audioCodecParam;
    memset(&audioCodecParam, 0, sizeof(audioCodecParam));
    audioCodecParam.eSampleFmt = EAV_SAMPLE_FMT_S16;
    audioCodecParam.nBitsPerSample = 16;
    audioCodecParam.nChannels = 2;
    audioCodecParam.nSampleRate = 48000;
    audioCodecParam.eBitrateMode = EBITRATE_MODE_VBR;
    audioCodecParam.eCodecID = ECODEC_ID_MP3;
    audioCodecParam.eCodecLevel = ECODEC_LEVEL_NONE;
    audioCodecParam.eCodecProfile = ECODEC_PROFILE_NONE;
    audioCodecParam.nBitrate = 128000;
    encodeParam.audioParam = audioCodecParam;
    
    return encodeParam;
}

- (NSString *)getEngineGeneratedPath {
    NSString* docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString* generatedPath = [docPath stringByAppendingPathComponent:@"xedit/generated"];
    BOOL isDirectory = NO;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL existed = [fileManager fileExistsAtPath:generatedPath isDirectory:&isDirectory];
    if ( !(isDirectory == YES && existed == YES) ) {//如果文件夹不存在
        [fileManager createDirectoryAtPath:generatedPath withIntermediateDirectories:YES attributes:nil error:nil];
    }
    return generatedPath;
}

#pragma mark - Lazy Load

-(NSDateFormatter *)dateFormatter {
    if (!_dateFormatter) {
        _dateFormatter = [[NSDateFormatter alloc] init];
        [_dateFormatter setDateFormat:@"yyyyMMddHHmmss"];
    }
    return _dateFormatter;
}

#pragma mark - Buttons Action

- (IBAction)clickOutput:(id)sender {
    if (!_currentSelectedOutputType) {
        [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"请选择输出类型" actionTitle:@"确定" handler:nil];
        return;
    }
    xedit::GenerateSetting generateSetting;
    memset(&generateSetting, 0, sizeof(generateSetting));
    NSString *generatedPath = [self getEngineGeneratedPath];
    memcpy(generateSetting.strDestDir, [generatedPath cStringUsingEncoding:NSUTF8StringEncoding], [generatedPath length]);
    NSString *generatedName = [[self.dateFormatter stringFromDate:[NSDate date]] stringByAppendingString:@".mp4"];
    memcpy(generateSetting.strDestName, [generatedName cStringUsingEncoding:NSUTF8StringEncoding], [generatedName length]);
    generateSetting.rStartTime = Rational(0, 1);
    generateSetting.rDuration = timeline->getDuration();
    generateSetting.encodeParam = [self getEncodeParamWithOutputType:_currentSelectedOutputType];

    int result = timeline->generate(generateSetting, generateObserver);
    if (result < 0) {
        [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"生成失败" actionTitle:@"确定" handler:nil];
        return;
    }
    [self showProgressView];
}

#pragma mark - GenerateObserver

- (void)onGenerateFinished:(StatusCode)code {
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.progressView hide];
        if (code < 0) {
            [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"生成失败" actionTitle:@"确定" handler:nil];
        } else {
            [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"生成成功" actionTitle:@"确定" handler:^(UIAlertAction *action) {
                [self.navigationController popViewControllerAnimated:YES];
            }];
        }
    });
    
}

- (void)onGenerateUpdateProgress:(Rational)duration {
    float value = duration.doubleValue() / timeline->getDuration().doubleValue();
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.progressView setProgressValue:value];
    });
}

#pragma mark - UITableViewDelegate UITableViewDataSource

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return _outputTypes.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    XEditProjectOutputTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:kOUTPUTTYPECELL forIndexPath:indexPath];
    if (!cell) {
        cell = [[XEditProjectOutputTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:kOUTPUTTYPECELL];
    }
    if (indexPath.row < _outputTypes.count) {
        [cell setOutputType:[_outputTypes objectAtIndex:indexPath.row]];
    }
    if (_currentSelectedCell == cell) {
        [cell setOutputSelected:YES];
    } else {
        [cell setOutputSelected:NO];
    }
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    if (_currentSelectedCell) {
        [_currentSelectedCell setOutputSelected:NO];
        _currentSelectedCell = nil;
        _currentSelectedOutputType = nil;
    }
    XEditProjectOutputTableViewCell *tmpCell = [tableView cellForRowAtIndexPath:indexPath];
    [tmpCell setOutputSelected:YES];
    _currentSelectedCell = tmpCell;
    if (indexPath.row < _outputTypes.count) {
        _currentSelectedOutputType = [_outputTypes objectAtIndex:indexPath.row];
    }
}

#pragma mark - NavigationBackButtonProtocol

- (BOOL)navigationShouldPop {
    generateObserver->releaseMainVC();
    return YES;
}

#pragma mark - dealloc

- (void)dealloc {
    NSLog(@"%@ Released", NSStringFromClass(self.class));
}

@end
