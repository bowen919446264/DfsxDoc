//
//  XEditTimelineViewController.mm
//  XEdit
//
//  Created by DFSX on 2018/8/1.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "XEditTimelineViewController.h"
#import "XEditTimelineTotalTimeCollectionViewCell.h"
#import "XEditVideoTrackTableViewCell.h"
#import "XEditTimelineCutView.h"
#import "XEditTimelineVolumeView.h"

#import "XEditTimelineObserver.hpp"
#import "XEditVideoRenderer.hpp"

#include <IXEngine.h>
#include <ITimeLine.h>

#define kVIDEOTRACKCELL @"videoTrackCell"
#define kVIDEOTRACKCELLHEIGHT 84

@interface XEditTimelineViewController () <UICollectionViewDelegate, UICollectionViewDataSource, UIScrollViewDelegate, UITableViewDelegate, UITableViewDataSource, XEditVideoTrackTableViewCellDelegate>

@property (weak, nonatomic) IBOutlet UICollectionView *totalTimeCollectionView;
@property (weak, nonatomic) IBOutlet UIView *totalTimeView;
@property (weak, nonatomic) IBOutlet UILabel *totalTimeLabel;
@property (weak, nonatomic) IBOutlet UIScrollView *trackScrollView;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *trackScrollViewContentSizeWidth;
@property (weak, nonatomic) IBOutlet UITableView *trackTableView;
@property (weak, nonatomic) IBOutlet UIView *currentTimeView;
@property (weak, nonatomic) IBOutlet UILabel *currentTimeLabel;
@property (weak, nonatomic) IBOutlet UIToolbar *trackToolBar;
@property (weak, nonatomic) IBOutlet UIButton *cutButton;
@property (weak, nonatomic) IBOutlet UIButton *volumeButton;
@property (weak, nonatomic) IBOutlet UIButton *effectButton;
@property (weak, nonatomic) IBOutlet UIButton *deleteButton;
@property (weak, nonatomic) IBOutlet UIToolbar *defaultToolBar;
@property (weak, nonatomic) IBOutlet UIButton *themeButton;
@property (weak, nonatomic) IBOutlet UIButton *textButton;
@property (weak, nonatomic) IBOutlet UIButton *recordButton;
@property (weak, nonatomic) IBOutlet UIButton *musicButton;
@property (weak, nonatomic) IBOutlet UIView *trackToolDetailContainer;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *trackToolDetailContainerBottomConstraint;

@property (strong, nonatomic) XEditTimelineCutView *cutView;
@property (strong, nonatomic) XEditTimelineVolumeView *volumeView;

@property (strong, nonatomic) UITableViewCell *currentSelectedTrackCell;

@property (strong, nonatomic) NSMutableDictionary<NSNumber*, NSNumber*> *clipsLastOffsetDict;

@property (assign, nonatomic) CGFloat trackScrollViewAllowedMinXOffset;
@property (assign, nonatomic) CGFloat trackScrollViewAllowedMaxXOffset;

@end

@implementation XEditTimelineViewController {
    
    xedit::ITimeLine *timeline;
    XEditTimelineObserver *timelineObserver;
    XEditVideoRenderer *videoRenderer;
    
    xedit::ITrack *currentSelectedTrack;
    xedit::IClip *currentSelectedClip;
    
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self customSubviews];
    
    _clipsLastOffsetDict = [NSMutableDictionary new];
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(aClipDidSelected:) name:XEditAClipDidSelected object:nil];
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(aClipDidUnselected:) name:XEditAClipDidUnselected object:nil];
}

- (void)customSubviews {
    _trackScrollView.contentInset = UIEdgeInsetsMake(0, kSCREENWIDTH / 2, 0, kSCREENWIDTH / 2);
    _totalTimeCollectionView.contentInset = _trackScrollView.contentInset;
    [_trackTableView registerNib:[UINib nibWithNibName:@"XEditVideoTrackTableViewCell" bundle:[NSBundle xEditBundle]] forCellReuseIdentifier:kVIDEOTRACKCELL];
    _trackScrollViewContentSizeWidth.constant = 0;
    
    [_totalTimeView setCornerWithRadius:6];
    [_totalTimeView setBorderWithWidth:1 color:UIColor.blackColor];
    [_currentTimeView setCornerWithRadius:6];
    [_currentTimeView setBorderWithWidth:1 color:UIColor.blackColor];
}

- (void)setupXEdit {
    timeline = xedit::IXEngine::getSharedInstance()->getTimeLine();
    timelineObserver = new XEditTimelineObserver(self);
    timeline->addObserver(timelineObserver);
    videoRenderer= new XEditVideoRenderer(self);
    timeline->addRenderer(videoRenderer);
}

-(void)addMedia:(NSString *)mediaPath {
    xedit::IMedia *media = self->timeline->addMedia([mediaPath UTF8String]);
    [self addClip:media];
}

- (void)addClip:(xedit::IMedia *)media {
    xedit::IImageMedia *imageMedia = dynamic_cast<xedit::IImageMedia *>(media);
    if (imageMedia) {
        xedit::ITrack *track = timeline->getTrack(xedit::ETrackType_Video, 0);
        xedit::IClip *clip = track->addClip(media->getId(), timeline->getDuration());
        if (clip) {
            NSLog(@"创建图片切片成功！");
            [self updateUI];
            [_clipsLastOffsetDict setObject:[NSNumber numberWithFloat:0.0] forKey:[NSNumber numberWithLongLong:clip->getId()]];
        }
        return;
    }
    xedit::IAVMedia *avMedia = dynamic_cast<xedit::IAVMedia *>(media);
    if (avMedia) {
        libav::AVMediaInfo mediaInfo;
        avMedia->getMediaInfo(&mediaInfo);
        if (mediaInfo.nVideoCount > 0) {
            xedit::ITrack *track = timeline->getTrack(xedit::ETrackType_Video, 0);
            xedit::IClip *clip = track->addClip(media->getId(), timeline->getDuration());
            if (clip) {
                NSLog(@"创建视频切片成功！");
                [self updateUI];
                [_clipsLastOffsetDict setObject:[NSNumber numberWithFloat:0.0] forKey:[NSNumber numberWithLongLong:clip->getId()]];
            }
        } else if (mediaInfo.nAudioCount > 0) {
//            xedit::ITrack *track = timeline->getTrack(xedit::ETrackType_Audio, 0);
//            xedit::IClip *clip = track->addAVStreamClip(media->getId(), timeline->getDuration(), mediaInfo.aStreams[0].nIndex);
//            if (clip) {
//                NSLog(@"创建音频切片成功！");
//                [self updateUI];
//                [_clipsLastOffsetDict setObject:[NSNumber numberWithFloat:0.0] forKey:[NSNumber numberWithLongLong:clip->getId()]];
//            }
        }
    }
}

- (void)destroyXEdit {
    timeline->removeAllObservers();
    timeline->removeAllRenderers();
    timelineObserver->releaseMainVC();
    videoRenderer->releaseMainVC();
    [self pause];
}

- (void)play {
    timeline->play();
}

- (void)pause {
    timeline->pause();
}

- (void)updateUI {
    //更新时间线总时长
    Rational duration = timeline->getDuration();
    _totalTimeLabel.text = [XEditUtils getTimeHHmmssFromSeconds:ceil(duration.integerValue())];
    //设置时间线当前时间
    Rational current = timeline->getCurrentPos();
    _currentTimeLabel.text = [XEditUtils getTimeHHmmssFromSeconds:current.integerValue()];
    //设置时间线滚动宽度
    _trackScrollViewContentSizeWidth.constant = XEditFrameWidth * duration.doubleValue();
    [_totalTimeCollectionView reloadData];
    [_trackTableView reloadData];
}

- (void)undo {
    if (timeline->canUndo()) {
        xedit::IAction *action = timeline->undo();
        if (action) {
            NSLog(@"撤销成功！");
        } else {
           [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"撤销失败" actionTitle:@"确定" handler:NULL];
        }
    } else {
        [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"没有动作可以撤销" actionTitle:@"确定" handler:NULL];
    }
}

- (void)redo {
    if (timeline->canRedo()) {
        xedit::IAction *action = timeline->redo();
        if (action) {
            NSLog(@"重做成功！");
        } else {
            [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"重做失败" actionTitle:@"确定" handler:NULL];
        }
    } else {
        [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"没有动作可以重做" actionTitle:@"确定" handler:NULL];
    }
}

- (xedit::PreviewFrame *)getFirstFrame {
    xedit::PreviewFrame *frame = NULL;
    xedit::IClip *firstClip = NULL;
    int videoTrackCount = timeline->getTrackCount(ETrackType_Video);
    for (int i=0; i < videoTrackCount; i++) {
        xedit::ITrack *track = timeline->getTrack(ETrackType_Video, i);
        if (track->getClipCount() > 0) {
            xedit::IClip *firstClipInCurrentTrack = track->getClip(0);
            if (firstClip) {
                if (firstClipInCurrentTrack->getOffsetOnTrack().doubleValue() < firstClip->getOffsetOnTrack().doubleValue()) {
                    firstClip = firstClipInCurrentTrack;
                }
            } else {
                firstClip = firstClipInCurrentTrack;
            }
        }
    }
    if (firstClip) {
        int64_t mediaId = firstClip->getRefMediaId();
        xedit::IMedia *media = timeline->getMediaById(mediaId);
        if (media) {
            xedit::IAVMedia *avmedia = dynamic_cast<xedit::IAVMedia *>(media);
            if (avmedia) {
                avmedia->openPreviewSession();
                xedit::IPreview *preview = avmedia->getPreview(0);
                if (preview) {
                    frame = preview->getPreviewFrameNearBy(firstClip->getOffsetInMedia());
                    if (!frame || (firstClip->getOffsetInMedia().doubleValue() - frame->rTimeOffset.doubleValue() >= 1)) {
                        frame = avmedia->createPreviewFrame(0, firstClip->getOffsetInMedia());
                    }
                } else {
                    frame = avmedia->createPreviewFrame(0, firstClip->getOffsetInMedia());
                }
                avmedia->closePreviewSession();
            }
            xedit::IImageMedia *imagemedia = dynamic_cast<xedit::IImageMedia *>(media);
            if (imagemedia) {
                xedit::IPreview *preview = imagemedia->getPreview();
                if (preview && preview->getPreviewFrameCount() > 0) {
                    frame = preview->getPreviewFrame(0);
                }
            }
        }
    }
    return frame;
}

- (void)removeTrackToolDetailContainerAllSubviews {
    for(UIView *view in [_trackToolDetailContainer subviews]) {
        [view removeFromSuperview];
    }
}

- (void)showTrackToolDetailView {
    [_trackToolDetailContainer setHidden:NO];
    _trackToolDetailContainerBottomConstraint.constant = 0;
}

- (void)hideTrackToolDetailView {
    [_trackToolDetailContainer setHidden:YES];
    _trackToolDetailContainerBottomConstraint.constant = -44;
    [self removeTrackToolDetailContainerAllSubviews];
    [_cutButton setSelected:NO];
    [_volumeButton setSelected:NO];
    [_effectButton setSelected:NO];
}

- (void)showTrackToolCutView {
    [_cutButton setSelected:YES];
    [_volumeButton setSelected:NO];
    [_effectButton setSelected:NO];
    [self removeTrackToolDetailContainerAllSubviews];
    if (!_cutView) {
        _cutView = [XEditTimelineCutView timelineCutView];
        kWeakSelf(self);
        _cutView.didEnterSplit = ^{
            //拆分
            kStrongSelf(self);
            if (!self) {
                return;
            }
            if (self->currentSelectedTrack == NULL || self->currentSelectedClip == NULL) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"没有获取到当前轨道或切片" actionTitle:@"确定" handler:nil];
                });
                return;
            }
            CGFloat splitValue = self.trackScrollView.contentOffset.x - self.trackScrollViewAllowedMinXOffset;
            Rational splitRational = Rational(splitValue, XEditFrameWidth);
            xedit::IClip *leftClip = NULL;
            xedit::IClip *rightClip = NULL;
            int64_t currentClipId = self->currentSelectedClip->getId();
            int result = self->currentSelectedTrack->splitClip(currentClipId, splitRational, &leftClip, &rightClip);
            if (result < 0) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"切片拆分失败" actionTitle:@"确定" handler:nil];
                });
            } else {
                [self.clipsLastOffsetDict removeObjectForKey:[NSNumber numberWithLongLong:currentClipId]];
                [self.clipsLastOffsetDict setObject:[NSNumber numberWithFloat:0.0] forKey:[NSNumber numberWithLongLong:leftClip->getId()]];
                [self.clipsLastOffsetDict setObject:[NSNumber numberWithFloat:0.0] forKey:[NSNumber numberWithLongLong:rightClip->getId()]];
                if ([self.currentSelectedTrackCell isKindOfClass:[XEditVideoTrackTableViewCell class]]) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [(XEditVideoTrackTableViewCell*)self.currentSelectedTrackCell updateUIForClipSplitedWithLeftClip:leftClip rightClip:rightClip];
                    });
                }
            }
        };
        _cutView.didEnterSeparate = ^{
            //分离
            kStrongSelf(self);
            if (!self) {
                return;
            }
            if (self->currentSelectedTrack == NULL || self->currentSelectedClip == NULL) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"没有获取到当前轨道或切片" actionTitle:@"确定" handler:nil];
                });
                return;
            }
        };
        _cutView.didEnterCopy = ^{
            //复制
            kStrongSelf(self);
            if (!self) {
                return;
            }
            if (self->currentSelectedTrack == NULL || self->currentSelectedClip == NULL) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"没有获取到当前轨道或切片" actionTitle:@"确定" handler:nil];
                });
                return;
            }
        };
    }
    [_trackToolDetailContainer addSubview:_cutView];
    [_cutView autoPinEdgesToSuperviewEdges];
    [self showTrackToolDetailView];
}

- (void)showTrackToolVolumeView {
    [_volumeButton setSelected:YES];
    [_cutButton setSelected:NO];
    [_effectButton setSelected:NO];
    [self removeTrackToolDetailContainerAllSubviews];
    if (!_volumeView) {
        _volumeView = [XEditTimelineVolumeView timelineVolumeView];
        kWeakSelf(self);
        _volumeView.didVolumeChanged = ^(float) {
            //声音调整
            kStrongSelf(self);
            if (!self) {
                return;
            }
            if (self->currentSelectedTrack == NULL || self->currentSelectedClip == NULL) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"没有获取到当前轨道或切片" actionTitle:@"确定" handler:nil];
                });
                return;
            }
        };
        _volumeView.didEnterAutoChange = ^{
            //声音渐变
            kStrongSelf(self);
            if (!self) {
                return;
            }
            if (self->currentSelectedTrack == NULL || self->currentSelectedClip == NULL) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"没有获取到当前轨道或切片" actionTitle:@"确定" handler:nil];
                });
                return;
            }
        };
    }
    [_trackToolDetailContainer addSubview:_volumeView];
    [_volumeView autoPinEdgesToSuperviewEdges];
    [self showTrackToolDetailView];
}

- (xedit::ITrack *)getMaxDurationTrackOnTimeline {
    xedit::ITrack *maxDurationTrack = NULL;
    int videoTrackCount = timeline->getTrackCount(xedit::ETrackType_Video);
    for (int i=0; i < videoTrackCount; i++) {
        xedit::ITrack *track = timeline->getTrack(xedit::ETrackType_Video, i);
        if (track) {
            double trackTotalDuration = [self getTrackTotalDuration:track];
            if (maxDurationTrack) {
                double tmpDuration = [self getTrackTotalDuration:maxDurationTrack];
                if (trackTotalDuration > tmpDuration) {
                    maxDurationTrack = track;
                }
            } else {
                maxDurationTrack = track;
            }
        }
    }
    int audioTrackCount = timeline->getTrackCount(xedit::ETrackType_Audio);
    for (int i=0; i < audioTrackCount; i++) {
        xedit::ITrack *track = timeline->getTrack(xedit::ETrackType_Audio, i);
        if (track) {
            double trackTotalDuration = [self getTrackTotalDuration:track];
            if (maxDurationTrack) {
                double tmpDuration = [self getTrackTotalDuration:maxDurationTrack];
                if (trackTotalDuration > tmpDuration) {
                    maxDurationTrack = track;
                }
            } else {
                maxDurationTrack = track;
            }
        }
    }
    return maxDurationTrack;
}

- (double)getTrackTotalDuration:(xedit::ITrack *)track {
    double totalDuration = 0;
    int clipCount = track->getClipCount();
    for (int i=0; i < clipCount; i++) {
        xedit::IClip *clip = track->getClip(i);
        double clipDuration = clip->getDuration().doubleValue();
        totalDuration +=clipDuration;
    }
    return totalDuration;
}

#pragma mark - Notification Methods

- (void)aClipDidSelected:(NSNotification *)notification {
    if (_currentSelectedTrackCell) {
        UITableViewCell *tmpCell = (UITableViewCell *)notification.object;
        if (tmpCell != _currentSelectedTrackCell) {
            if ([_currentSelectedTrackCell isKindOfClass:[XEditVideoTrackTableViewCell class]]) {
                XEditVideoTrackTableViewCell *tmpCell = (XEditVideoTrackTableViewCell *)_currentSelectedTrackCell;
                [tmpCell cancelSelected];
                _currentSelectedTrackCell = NULL;
                currentSelectedTrack = NULL;
                currentSelectedClip = NULL;
                [self hideTrackToolDetailView];
            }
        }
    }
    if ([notification.object isKindOfClass:[XEditVideoTrackTableViewCell class]]) {
        _currentSelectedTrackCell = (XEditVideoTrackTableViewCell *)notification.object;
        currentSelectedTrack = [(XEditVideoTrackTableViewCell *)_currentSelectedTrackCell getTrack];
        currentSelectedClip = [(XEditVideoTrackTableViewCell *)_currentSelectedTrackCell getCurrentSelectedClip];
        [_defaultToolBar setHidden:YES];
        [_trackToolBar setHidden:NO];
        CGRect convertRect = [_trackScrollView convertRect:[((XEditVideoTrackTableViewCell *)_currentSelectedTrackCell) getCurrentSelectedView].frame fromView:_currentSelectedTrackCell];
        _trackScrollViewAllowedMinXOffset = convertRect.origin.x - (kSCREENWIDTH / 2);
        _trackScrollViewAllowedMaxXOffset = convertRect.size.width + _trackScrollViewAllowedMinXOffset;
    }
}

- (void)aClipDidUnselected:(NSNotification *)notification {
    _currentSelectedTrackCell = NULL;
    currentSelectedTrack = NULL;
    currentSelectedClip = NULL;
    [_trackToolBar setHidden:YES];
    [_defaultToolBar setHidden:NO];
    [self hideTrackToolDetailView];
}

#pragma mark - TimelineObserver

- (void)onPositionDidChanged:(Rational)newPosition {
    double currentTime = newPosition.doubleValue();
    [_trackScrollView setContentOffset:CGPointMake(currentTime * XEditFrameWidth, 0) animated:NO];
}

- (void)onTrackCreated:(ITrack *)pTrack {
    NSLog(@"轨道被创建！ID:%lld", pTrack->getId());
}

- (void)onTrackRemoved:(ID)trackId {
    NSLog(@"轨道被移除！ID:%lld", trackId);
}

- (void)onTimeLineStatusChanged:(ETimeLineStatus)newStatus {
    self.didTimelineStatusChanged(newStatus);
}

#pragma mark - VideoRenderer

- (int)onReceivedVideoBuffer:(IVideoBuffer *)buffer {
    int width = buffer->getVideoRect().nWidth;
    int height = buffer->getVideoRect().nHeight;
    CVPixelBufferRef pixelBuffer;
    NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:
                             [NSNumber numberWithBool:YES], kCVPixelBufferCGImageCompatibilityKey,
                             [NSNumber numberWithBool:YES], kCVPixelBufferCGBitmapContextCompatibilityKey,
                             [NSNumber numberWithBool:YES], kCVPixelBufferOpenGLCompatibilityKey,
                             [NSNumber numberWithInt:width], kCVPixelBufferWidthKey,
                             [NSNumber numberWithInt:height], kCVPixelBufferHeightKey,
                             nil];
    CVReturn status = CVPixelBufferCreate(kCFAllocatorDefault, width, height, kCVPixelFormatType_32BGRA, (CFDictionaryRef)CFBridgingRetain(options), &pixelBuffer);
    if (status != kCVReturnSuccess || pixelBuffer == NULL) {
        NSLog(@"IVideobBuffer to CVPixelBufferRef failure!");
        return -1;
    }
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    void *baseAddress = CVPixelBufferGetBaseAddress(pixelBuffer);
    memcpy(baseAddress, buffer->getPlanePointer(0), buffer->getSize(0));
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    dispatch_sync(dispatch_get_main_queue(), ^{
        self.didReceivedVideoBuffer(pixelBuffer);
    });
    CVPixelBufferRelease(pixelBuffer);
    return 0;
}

#pragma mark - Track ToolBar Buttons Action

//裁剪素材
- (IBAction)cutClick:(id)sender {
    if (_cutButton.isSelected) {
        [self hideTrackToolDetailView];
    } else {
        [self showTrackToolCutView];
    }
}

//调整素材音量
- (IBAction)volumnClick:(id)sender {
    if (_volumeButton.isSelected) {
        [self hideTrackToolDetailView];
    } else {
        [self showTrackToolVolumeView];
    }
}

//素材选择滤镜
- (IBAction)effectClick:(id)sender {
    [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"暂不支持滤镜功能" actionTitle:@"确定" handler:NULL];
}

//删除素材
- (IBAction)deleteClick:(id)sender {
    kWeakSelf(self);
    [UIAlertController showTwoActionWithViewController:self title:@"温馨提示" message:@"确定要删除选中的切片吗？" firstActionTitle:@"取消" secondActionTitle:@"删除" firstActionHandler:NULL secondActionHandler:^(UIAlertAction *action) {
        kStrongSelf(self);
        if (!self) {
            return;
        }
        if (self->currentSelectedTrack == NULL || self->currentSelectedClip == NULL) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"没有获取到当前轨道或切片" actionTitle:@"确定" handler:nil];
            });
            return;
        }
        int result = self->currentSelectedTrack->removeClipById(self->currentSelectedClip->getId());
        if (result < 0) {
            //删除失败
            [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"删除选中的切片失败，请重试" actionTitle:@"确定" handler:NULL];
        } else {
            //删除成功
            [self updateUI];
        }
    }];
}

#pragma mark - Default ToolBar Buttons Action

//选择主题
- (IBAction)themeClick:(id)sender {
}

//添加文字
- (IBAction)textClick:(id)sender {
}

//添加录音
- (IBAction)recordClick:(id)sender {
}

//添加本地音乐
- (IBAction)musicClick:(id)sender {
}

#pragma mark - UICollectionViewDelegate UICollectionViewDataSource

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return 1;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return ceil(_trackScrollView.contentSize.width / (XEditFrameWidth * 2));
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    XEditTimelineTotalTimeCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"XEditTimelineTotalTimeCollectionViewCell" forIndexPath:indexPath];
    [cell setTime:indexPath.item * 2 + 1];
    return cell;
}

#pragma mark - UITableViewDelegate UITableViewDataSource

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return timeline->getTrackCount(xedit::ETrackType_Video);
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    XEditVideoTrackTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:kVIDEOTRACKCELL forIndexPath:indexPath];
    if (!cell) {
        cell = [[XEditVideoTrackTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:kVIDEOTRACKCELL];
    }
    cell.delegate = self;
    if (indexPath.row < timeline->getTrackCount(xedit::ETrackType_Video)) {
        [cell setTrackIndex:(int)indexPath.row];
    }
    
    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return kVIDEOTRACKCELLHEIGHT;
}

#pragma mark - UIScrollViewDelegate

- (void)scrollViewDidScroll:(UIScrollView *)scrollView {
    CGFloat currentOffsetX = scrollView.contentOffset.x;
    if (_currentSelectedTrackCell) {
        if (currentOffsetX > _trackScrollViewAllowedMaxXOffset) {
            currentOffsetX = _trackScrollViewAllowedMaxXOffset;
            [scrollView setContentOffset:CGPointMake(currentOffsetX, 0) animated:NO];
        }
        if (currentOffsetX < _trackScrollViewAllowedMinXOffset) {
            currentOffsetX = _trackScrollViewAllowedMinXOffset;
            [scrollView setContentOffset:CGPointMake(currentOffsetX, 0) animated:NO];
        }
    }
    [_totalTimeCollectionView setContentOffset:CGPointMake(currentOffsetX, 0) animated:NO];
    
}

#pragma mark - XEditVideoTrackTableViewCellDelegate

- (void)videoTrackTableViewCell:(XEditVideoTrackTableViewCell *)cell track:(xedit::ITrack *)track clipDurationChangeBegan:(xedit::IClip *)clip {
    [_trackScrollView setScrollEnabled:NO];
}

- (void)videoTrackTableViewCell:(XEditVideoTrackTableViewCell *)cell track:(xedit::ITrack *)track clip:(xedit::IClip *)clip durationChanged:(CGFloat)changedWidth {
    xedit::ITrack *maxDurationTrack = [self getMaxDurationTrackOnTimeline];
    if (maxDurationTrack) {
        if (maxDurationTrack->getId() == track->getId()) {
            _trackScrollViewContentSizeWidth.constant = changedWidth;
            [_totalTimeCollectionView reloadData];
        }
    }
}

- (void)videoTrackTableViewCell:(XEditVideoTrackTableViewCell *)cell track:(xedit::ITrack *)track clipDurationChangeEnded:(xedit::IClip *)clip durationChanged:(CGFloat)changedWidth {
    [_trackScrollView setScrollEnabled:YES];
    xedit::ITrack *maxDurationTrack = [self getMaxDurationTrackOnTimeline];
    if (maxDurationTrack) {
        if (maxDurationTrack->getId() == track->getId()) {
            _trackScrollViewContentSizeWidth.constant = changedWidth;
            [_totalTimeCollectionView reloadData];
        }
    }
}

- (void)videoTrackTableViewCell:(XEditVideoTrackTableViewCell *)cell track:(xedit::ITrack *)track clipDurationChangeEnded:(xedit::IClip *)clip {
    [_trackScrollView setScrollEnabled:YES];
}

- (void)videoTrackTableViewCell:(XEditVideoTrackTableViewCell *)cell track:(xedit::ITrack *)track clipOffsetChangeBegan:(xedit::IClip *)clip {
    [_trackScrollView setScrollEnabled:NO];
}

- (void)videoTrackTableViewCell:(XEditVideoTrackTableViewCell *)cell track:(xedit::ITrack *)track clip:(xedit::IClip *)clip OffsetChanged:(CGFloat)changedWidth {
    CGFloat currentClipLastOffsetChangedValue = [_clipsLastOffsetDict objectForKey:[NSNumber numberWithLongLong:clip->getId()]].floatValue;
    CGFloat trackScrollViewShouldChangedXOffset = changedWidth - currentClipLastOffsetChangedValue;
    [_trackScrollView setContentOffset:CGPointMake(_trackScrollView.contentOffset.x - trackScrollViewShouldChangedXOffset, 0) animated:NO];
    [_clipsLastOffsetDict setObject:[NSNumber numberWithFloat:changedWidth] forKey:[NSNumber numberWithLongLong:clip->getId()]];
}

- (void)videoTrackTableViewCell:(XEditVideoTrackTableViewCell *)cell track:(xedit::ITrack *)track clipOffsetChangeEnded:(xedit::IClip *)clip OffsetChanged:(CGFloat)changedWidth {
    [_trackScrollView setScrollEnabled:YES];
    CGFloat currentClipLastOffsetChangedValue = [_clipsLastOffsetDict objectForKey:[NSNumber numberWithLongLong:clip->getId()]].floatValue;
    CGFloat trackScrollViewShouldChangedXOffset = changedWidth - currentClipLastOffsetChangedValue;
    [_trackScrollView setContentOffset:CGPointMake(_trackScrollView.contentOffset.x - trackScrollViewShouldChangedXOffset, 0) animated:NO];
    [_clipsLastOffsetDict setObject:[NSNumber numberWithFloat:changedWidth] forKey:[NSNumber numberWithLongLong:clip->getId()]];
}

#pragma mark - dealloc

- (void)dealloc {
    [NSNotificationCenter.defaultCenter removeObserver:self name:XEditAClipDidSelected object:nil];
    [NSNotificationCenter.defaultCenter removeObserver:self name:XEditAClipDidUnselected object:nil];
    NSLog(@"%@ Released", NSStringFromClass(self.class));
}

@end
