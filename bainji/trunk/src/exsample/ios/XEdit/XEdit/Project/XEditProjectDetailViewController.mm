//
//  XEditProjectDetailViewController.mm
//  XEdit
//
//  Created by DFSX on 2018/8/1.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "XEditProjectDetailViewController.h"
#import "XEditTimelineViewController.h"
#import "XEditProjectOutputViewController.h"
#import <Photos/Photos.h>

#include <IXEngine.h>
#include <IProject.h>
#include <ITimeLine.h>



@interface XEditProjectDetailViewController () <UITextFieldDelegate, CTAssetsPickerControllerDelegate>

@property (weak, nonatomic) IBOutlet UIView *playerContainer;
@property (weak, nonatomic) IBOutlet UIButton *addMaterialButton;
@property (weak, nonatomic) IBOutlet UIButton *backwardSpeedlyButton;
@property (weak, nonatomic) IBOutlet UIButton *backwardButton;
@property (weak, nonatomic) IBOutlet UIButton *playButton;
@property (weak, nonatomic) IBOutlet UIButton *forwardButton;
@property (weak, nonatomic) IBOutlet UIButton *forwardSpeedlyButton;
@property (weak, nonatomic) IBOutlet UIButton *repealButton;
@property (weak, nonatomic) IBOutlet UIButton *recoverButton;
@property (weak, nonatomic) IBOutlet UIView *timelineContainer;

@property (strong, nonatomic) UITextField *projectNameTextField;

@property (strong, nonatomic) XEditTimelineViewController *timelineController;

@property (assign, nonatomic) BOOL presentToAssetsPicker;

@property (strong, nonatomic) SKGLKView *glView;

@end

@implementation XEditProjectDetailViewController {
    
    xedit::IXEngine *engine;
    xedit::IProject *project;
    
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self customNavigationBar];
    [self setupSubControllers];
    
    engine = xedit::IXEngine::getSharedInstance();
    
    if (_projectPath == NULL || [_projectPath isEqualToString:@""]) {
        [self newProject];
    } else {
        [self openProject];
    }
}

-(void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    _presentToAssetsPicker = NO;
    UIApplication.sharedApplication.statusBarStyle = UIStatusBarStyleLightContent;
}

-(void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    if (_presentToAssetsPicker) {
        UIApplication.sharedApplication.statusBarStyle = UIStatusBarStyleDefault;
    }
}

- (void)setupSubControllers {
    _timelineController = [[UIStoryboard storyboardWithName:@"XEditProject" bundle:[NSBundle xEditBundle]] instantiateViewControllerWithIdentifier:@"XEditTimelineViewController"];
    [self addChildViewController:_timelineController];
    [_timelineContainer addSubview:_timelineController.view];
    [_timelineController.view autoPinEdgesToSuperviewEdges];
    
    self.glView = [[SKGLKView alloc] initWithFrame:_playerContainer.bounds];
    self.glView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    [_playerContainer insertSubview:self.glView atIndex:0];
    [self.glView autoPinEdgesToSuperviewEdges];
    
    //TimelineController事件响应
    kWeakSelf(self);
    _timelineController.didReceivedVideoBuffer = ^(CVPixelBufferRef pixelBuffer) {
        kStrongSelf(self);
        if (!self) {
            return;
        }
        [self.glView renderWithCVPixelBuffer:pixelBuffer orientation:0 mirror:NO];
    };
    _timelineController.didTimelineStatusChanged = ^(xedit::ETimeLineStatus status) {
        kStrongSelf(self);
        if (!self) {
            return;
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            switch (status) {
                case xedit::ETimeLineStatus_Playing:
                    [self.playButton setSelected:YES];
                    break;
                case xedit::ETimeLineStatus_Pause:
                    [self.playButton setSelected:NO];
                    break;
                case xedit::ETimeLineStatus_PlayEnd:
                    [self.playButton setSelected:NO];
                    break;
                case xedit::ETimeLineStatus_DropFrame:
                    NSLog(@"播放掉帧！");
                    break;
                case xedit::ETimeLineStatus_PlayFailed:
                    [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"播放失败" actionTitle:@"确定" handler:NULL];
                    [self.playButton setSelected:NO];
                    break;
                case xedit::ETimeLineStatus_Error:
                    NSLog(@"错误！");
                    break;
                case xedit::ETimeLineStatus_Generating:
                    NSLog(@"正在生成！");
                    break;
                case xedit::ETimeLineStatus_GenerateFinish:
                    NSLog(@"生成完成！");
                    break;
                case xedit::ETimeLineStatus_GenerateFailed:
                    NSLog(@"生成失败！");
                    break;
                default:
                    break;
            }
        });
    };
}

- (void)customNavigationBar {
    UIBarButtonItem *outputButton = [[UIBarButtonItem alloc] initWithTitle:@"输出" style:UIBarButtonItemStyleDone target:self action:@selector(output)];
    self.navigationItem.rightBarButtonItem = outputButton;
    
    _projectNameTextField = [[UITextField alloc] initWithFrame:CGRectMake(0, 0, 200, 30)];
    _projectNameTextField.returnKeyType = UIReturnKeyDone;
    _projectNameTextField.borderStyle = UITextBorderStyleNone;
    _projectNameTextField.textAlignment = NSTextAlignmentCenter;
    _projectNameTextField.textColor = UIColor.whiteColor;
    _projectNameTextField.font = [UIFont systemFontOfSize:18 weight:UIFontWeightMedium];
    _projectNameTextField.text = @"工程名称";
    _projectNameTextField.delegate = self;
    self.navigationItem.titleView = _projectNameTextField;
}

- (void)newProject {
    NSString *newProjectName = @"新建工程";
    int result = engine->newProject([newProjectName UTF8String], [self defaultProjectSettings]);
    if (result < 0) {
        kWeakSelf(self);
        [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"新建工程失败" actionTitle:@"确定" handler:^(UIAlertAction *action) {
            kStrongSelf(self);
            if (!self) {
                return;
            }
            [self.navigationController popViewControllerAnimated:YES];
        }];
    }
    NSLog(@"新建工程成功！");
    project = engine->getCurrentProject();
    [self.timelineController setupXEdit];
}

- (xedit::ProjectSetting)defaultProjectSettings {
    xedit::ProjectSetting setting;
    
    setting.ePixFormat = libav::EPIX_FMT_NV12;
    setting.eSampleFormat = libav::EAV_SAMPLE_FMT_S16;
    setting.nSampleRate = 48000;
    setting.nBitsPerSample = 16;
    setting.nChannelCount = 2;
    setting.nHeight = 720;
    setting.nWidth = 1280;
    setting.rFrameRate = libav::Rational(25,1);
    setting.bInterlaced = NO;
    setting.bTopFieldFirst = NO;
    setting.rAspectRatio = libav::Rational(16,9);
    
    return setting;
}

- (void)openProject {
    
}

- (void)output {
    if (engine->getTimeLine()->getMediaCount() <= 0) {
        [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"没有可输出的内容" actionTitle:@"确定" handler:nil];
        return;
    }
    XEditProjectOutputViewController *outputVC = [[UIStoryboard storyboardWithName:@"XEditProject" bundle:[NSBundle xEditBundle]] instantiateViewControllerWithIdentifier:@"XEditProjectOutputViewController"];
    outputVC.title = _projectNameTextField.text;
    xedit::PreviewFrame *firstFrame = [_timelineController getFirstFrame];
    if (firstFrame) {
        outputVC.firstFramePath = [NSString stringWithCString:firstFrame->path.c_str() encoding:[NSString defaultCStringEncoding]];
    }
    [self.navigationController pushViewController:outputVC animated:YES];
}

- (void)hideKeyboard:(UIButton *)sender {
    [_projectNameTextField resignFirstResponder];
    [sender removeFromSuperview];
}

- (void)releaseAll {
    [self.timelineController destroyXEdit];
    int result = engine->closeCurrentProject();
    if (result < 0) {
        NSLog(@"关闭当前工程失败！");
    } else {
        NSLog(@"关闭当前工程成功！");
    }
}

#pragma mark - Buttons Action

///添加素材
- (IBAction)addMaterialClick:(id)sender {
    CTAssetsPickerController *picker = [[CTAssetsPickerController alloc] init];
    picker.delegate = self;
    picker.showsSelectionIndex = YES;
    _presentToAssetsPicker = YES;
    [self presentViewController:picker animated:YES completion:nil];
}

///快退
- (IBAction)backwardSpeedlyClick:(id)sender {
    NSLog(@"快退！");
}

///退一帧
- (IBAction)backwardClick:(id)sender {
    NSLog(@"退一帧！");
}

///播放（暂停）
- (IBAction)playClick:(id)sender {
    if (self.playButton.isSelected) {
        [self.timelineController pause];
    } else {
        [self.timelineController play];
    }
}

///进一帧
- (IBAction)forwardClick:(id)sender {
    NSLog(@"进一帧！");
}

///快进
- (IBAction)forwardSpeedlyClick:(id)sender {
    NSLog(@"快进！");
}

///撤销上一个操作
- (IBAction)repealClick:(id)sender {
    [self.timelineController undo];
}

///恢复上一个操作
- (IBAction)recoverClick:(id)sender {
    [self.timelineController redo];
}

#pragma mark - UITextFieldDelegate

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField {
    UIButton *hideKeyboardButton = [UIButton buttonWithType:UIButtonTypeCustom];
    hideKeyboardButton.frame = self.view.bounds;
    [hideKeyboardButton addTarget:self action:@selector(hideKeyboard:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:hideKeyboardButton];
    return YES;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

#pragma mark - CTAssetsPickerControllerDelegate

- (void)assetsPickerControllerDidCancel:(CTAssetsPickerController *)picker {
    [picker dismissViewControllerAnimated:YES completion:nil];
}

- (BOOL)assetsPickerController:(CTAssetsPickerController *)picker shouldSelectAsset:(PHAsset *)asset {
    return YES;
}

- (void)assetsPickerController:(CTAssetsPickerController *)picker didFinishPickingAssets:(NSArray<PHAsset *> *)assets {
    [picker dismissViewControllerAnimated:YES completion:nil];
    for (PHAsset *asset in assets) {
        switch (asset.mediaType) {
            case PHAssetMediaTypeImage: {
                PHImageRequestOptions *options = [[PHImageRequestOptions alloc] init];
                [options setNetworkAccessAllowed:YES];
                [options setSynchronous:YES];
                kWeakSelf(self);
                [[PHImageManager defaultManager] requestImageDataForAsset:asset options:options resultHandler:^(NSData * _Nullable imageData, NSString * _Nullable dataUTI, UIImageOrientation orientation, NSDictionary * _Nullable info) {
                    kStrongSelf(self);
                    if (!self) {
                        return;
                    }
                    if (imageData) {
                        NSString *assetName = [asset valueForKey:@"filename"];
                        if (assetName == NULL || [assetName isEqualToString:@""]) {
                            assetName = @"tmpImage.jpg";
                        }
                        NSData *writeData = imageData;
                        if ([XEditUtils assetIsHEIC:asset]) {
                            writeData = [XEditUtils HEIC2JPEG:imageData];
                        }
                        NSString *docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
                        NSString *xeditPath = [docPath stringByAppendingPathComponent:@"xedit"];
                        NSString *writePath = [xeditPath stringByAppendingFormat:@"/%@", assetName];
                        BOOL isExist = [[NSFileManager defaultManager] fileExistsAtPath:writePath];
                        if (isExist) {
                            [[NSFileManager defaultManager] removeItemAtPath:writePath error:NULL];
                        }
                        BOOL writeResult = [imageData writeToFile:writePath atomically:YES];
                        if (writeResult) {
                            NSLog(@"添加图片成功！");
                            dispatch_async(dispatch_get_main_queue(), ^{
                                [self.timelineController addMedia:writePath];
                            });
                        }
                    }
                }];
                break;
            }
            case PHAssetMediaTypeAudio:
            case PHAssetMediaTypeVideo: {
                NSArray<PHAssetResource *> *resources = [PHAssetResource assetResourcesForAsset:asset];
                if (resources.count > 0) {
                    PHAssetResource *resource = [resources objectAtIndex:0];
                    NSString *docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
                    NSString *xeditPath = [docPath stringByAppendingPathComponent:@"xedit"];
                    NSString *writePath = [xeditPath stringByAppendingFormat:@"/%@", resource.originalFilename];
                    NSURL *writeUrl = [NSURL fileURLWithPath:writePath];
                    BOOL isExist = [[NSFileManager defaultManager] fileExistsAtPath:writePath];
                    if (isExist) {
                        [[NSFileManager defaultManager] removeItemAtPath:writePath error:NULL];
                    }
                    PHAssetResourceRequestOptions *options = [[PHAssetResourceRequestOptions alloc] init];
                    [options setNetworkAccessAllowed:YES];
                    kWeakSelf(self);
                    [[PHAssetResourceManager defaultManager] writeDataForAssetResource:resource toFile:writeUrl options:options completionHandler:^(NSError * _Nullable error) {
                        kStrongSelf(self);
                        if (!self) {
                            return;
                        }
                        if (!error) {
                            NSLog(@"添加视/音频成功！");
                            dispatch_async(dispatch_get_main_queue(), ^{
                                [self.timelineController addMedia:writePath];
                            });
                        }
                    }];
                }
                break;
            }
            default:
                break;
        }
    }
}

#pragma mark - NavigationBackButtonProtocol

- (BOOL)navigationShouldPop {
    [self releaseAll];
    return YES;
}

#pragma mark - dealloc

- (void)dealloc {
    NSLog(@"%@ Released", NSStringFromClass(self.class));
}

@end
