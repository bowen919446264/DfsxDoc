//
//  XEditVideoClipView.mm
//  XEdit
//
//  Created by DFSX on 2018/8/3.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "XEditVideoClipView.h"
#import "XEditPanGestureRecognizer.h"

#include <IXEngine.h>
#include <ITimeLine.h>

@interface XEditVideoClipView ()

@property (weak, nonatomic) IBOutlet UIScrollView *framesContainer;
@property (weak, nonatomic) IBOutlet UIView *selectedView;
@property (weak, nonatomic) IBOutlet UIView *beginTimeBackgroundView;
@property (weak, nonatomic) IBOutlet UIView *endTimeBackgroundView;

@property (strong, nonatomic) XEditPanGestureRecognizer *beginTimeGesture;
@property (strong, nonatomic) XEditPanGestureRecognizer *endTimeGesture;

@property (assign, nonatomic) CGFloat lastBeginOffsetX;
@property (assign, nonatomic, readwrite) CGFloat lastEndOffsetX;

@property (assign, nonatomic) CGFloat lastBeginDurationX;
@property (assign, nonatomic, readwrite) CGFloat lastEndDurationX;

@end

@implementation XEditVideoClipView {
    
    xedit::IClip *clip;
    
}

+ (instancetype)videoClipViewWithClip:(xedit::IClip *)clip {
    NSArray *array = [[NSBundle xEditBundle] loadNibNamed:@"XEditVideoClipView" owner:self options:nil];
    XEditVideoClipView *view = (XEditVideoClipView *)[array firstObject];
    view->clip = clip;
    
    [view setupPreviews];
    [view config];
    
    return view;
}

- (void)setupPreviews {
    double duration = self->clip->getDuration().doubleValue();
    [_framesContainer setContentSize:CGSizeMake(duration * XEditFrameWidth, 0)];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        int64_t mediaId = self->clip->getRefMediaId();
        xedit::IMedia *media = xedit::IXEngine::getSharedInstance()->getTimeLine()->getMediaById(mediaId);
        if (media) {
            xedit::IAVMedia *avmedia = dynamic_cast<xedit::IAVMedia *>(media);
            if (avmedia) {
                [self setupVideoPreviews:avmedia];
                return;
            }
            xedit::IImageMedia *imagemedia = dynamic_cast<xedit::IImageMedia *>(media);
            if (imagemedia) {
                [self setupImagePreviews:imagemedia];
            }
        }
    });
}

- (void)config {
    _beginTimeGesture = [XEditPanGestureRecognizer panGestureWithDirection:XEditPanDirectionHorizontal target:self selector:@selector(timeChanged:)];
    [_beginTimeBackgroundView addGestureRecognizer:_beginTimeGesture];
    _endTimeGesture = [XEditPanGestureRecognizer panGestureWithDirection:XEditPanDirectionHorizontal target:self selector:@selector(timeChanged:)];
    [_endTimeBackgroundView addGestureRecognizer:_endTimeGesture];
}

- (void)setupVideoPreviews:(xedit::IAVMedia *)avmedia {
    avmedia->openPreviewSession();
    libav::AVMediaInfo mediaInfo;
    avmedia->getMediaInfo(&mediaInfo);
    libav::VideoStream videoStream = mediaInfo.vStreams[0];
    int duration = 0;
    for (int i=0; i < videoStream.nFrameCount; i+= 25) {
        Rational offsetInMedia = Rational(i, 25);
        xedit::PreviewFrame *frame = NULL;
        xedit::IPreview *preview = avmedia->getPreview(0);
        if (preview) {
            frame = preview->getPreviewFrameNearBy(offsetInMedia);
            if (!frame || (offsetInMedia.doubleValue() - frame->rTimeOffset.doubleValue() >= 1)) {
                frame = avmedia->createPreviewFrame(0, offsetInMedia);
            }
        } else {
            frame = avmedia->createPreviewFrame(0, offsetInMedia);
        }
        if (!frame) {
            continue;
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            UIImage *frameImage = [UIImage imageWithContentsOfFile:[NSString stringWithCString:frame->path.c_str() encoding:[NSString defaultCStringEncoding]]];
            UIImageView *frameView = [[UIImageView alloc] initWithFrame:CGRectMake(duration * XEditFrameWidth, 0, XEditFrameWidth, self.framesContainer.frame.size.height)];
            frameView.backgroundColor = [UIColor blackColor];
            frameView.contentMode = UIViewContentModeScaleAspectFill;
            frameView.clipsToBounds = YES;
            frameView.image = frameImage;
            
            [self.framesContainer addSubview:frameView];
        });
        duration += 1;
    }
    avmedia->closePreviewSession();
}

- (void)setupImagePreviews:(xedit::IImageMedia *)imagemedia {
    xedit::IPreview *preview = imagemedia->getPreview();
    if (preview && preview->getPreviewFrameCount() > 0) {
        xedit::PreviewFrame *frame = preview->getPreviewFrame(0);
        if (frame) {
            int64_t duration = ceil(self->clip->getDuration().doubleValue());
            UIImage *frameImage = [UIImage imageWithContentsOfFile:[NSString stringWithCString:frame->path.c_str() encoding:[NSString defaultCStringEncoding]]];
            for (int i=0; i < duration; i++) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    UIImageView *frameView = [[UIImageView alloc] initWithFrame:CGRectMake(i * XEditFrameWidth, 0, XEditFrameWidth, self.framesContainer.frame.size.height)];
                    frameView.backgroundColor = [UIColor blackColor];
                    frameView.contentMode = UIViewContentModeScaleAspectFill;
                    frameView.clipsToBounds = YES;
                    frameView.image = frameImage;
                    
                    [self.framesContainer addSubview:frameView];
                });
            }
        }
    }
}

- (void)timeChanged:(XEditPanGestureRecognizer *)gesture {
    if (gesture.direction == XEditPanDirectionHorizontal) {
        CGPoint vel = [gesture translationInView:self];
        CGFloat offsetX = vel.x;
        if (gesture.state == UIGestureRecognizerStateBegan) {
            if (gesture == _beginTimeGesture) {
                _lastBeginOffsetX = _lastEndOffsetX;
                [NSNotificationCenter.defaultCenter postNotificationName:XEditClipOffsetChangeBegan object:self];
            } else if (gesture == _endTimeGesture) {
                _lastBeginDurationX = _lastEndDurationX;
                [NSNotificationCenter.defaultCenter postNotificationName:XEditClipDurationChangeBegan object:self];
            }
        } else if (gesture.state == UIGestureRecognizerStateChanged) {
            if (gesture == _beginTimeGesture) {
                CGFloat contentOffsetX = 0;
                contentOffsetX = MAX(_lastEndOffsetX + offsetX, 0);
                contentOffsetX = MIN(contentOffsetX, _framesContainer.contentSize.width + _lastEndDurationX - XEditFrameWidth);
                [_framesContainer setContentOffset:CGPointMake(contentOffsetX, 0) animated:NO];
                NSDictionary *notificationDict = @{kXEditClipDurationChangedValue: [NSNumber numberWithFloat:_framesContainer.contentSize.width + _lastEndDurationX - contentOffsetX]};
                [NSNotificationCenter.defaultCenter postNotificationName:XEditClipDurationChanged object:self userInfo:notificationDict];
                NSDictionary *notificationDictTwo = @{kXEditClipOffsetChangedValue: [NSNumber numberWithFloat:contentOffsetX]};
                [NSNotificationCenter.defaultCenter postNotificationName:XEditClipOffsetChanged object:self userInfo:notificationDictTwo];
            } else if (gesture == _endTimeGesture) {
                CGFloat sizeWidthChanged = 0;
                sizeWidthChanged = MIN(_lastEndDurationX + offsetX, 0);
                sizeWidthChanged = MAX(sizeWidthChanged, -(_framesContainer.contentSize.width - _lastEndOffsetX - XEditFrameWidth));
                NSDictionary *notificationDict = @{kXEditClipDurationChangedValue: [NSNumber numberWithFloat:_framesContainer.contentSize.width - _lastEndOffsetX + sizeWidthChanged]};
                [NSNotificationCenter.defaultCenter postNotificationName:XEditClipDurationChanged object:self userInfo:notificationDict];
            }
        } else if (gesture.state == UIGestureRecognizerStateEnded) {
            if (gesture == _beginTimeGesture) {
                _lastEndOffsetX = _framesContainer.contentOffset.x;
                NSDictionary *notificationDict = @{kXEditClipOffsetChangeEndedValue: [NSNumber numberWithFloat:_framesContainer.contentOffset.x]};
                [NSNotificationCenter.defaultCenter postNotificationName:XEditClipOffsetChangeEnded object:self userInfo:notificationDict];
            } else if (gesture == _endTimeGesture) {
                CGFloat tmpLastEndOffsexX = 0;
                tmpLastEndOffsexX = MIN(_lastEndDurationX + offsetX, 0);
                tmpLastEndOffsexX = MAX(tmpLastEndOffsexX, -(_framesContainer.contentSize.width - XEditFrameWidth));
                _lastEndDurationX = tmpLastEndOffsexX;
                NSDictionary *notificationDict = @{kXEditClipDurationChangeEndedValue: [NSNumber numberWithFloat:_framesContainer.frame.size.width]};
                [NSNotificationCenter.defaultCenter postNotificationName:XEditClipDurationChangeEnded object:self userInfo:notificationDict];
            }
        }
    }
}

- (void)setSelected:(BOOL)selected {
    if (_selected != selected) {
        _selected = selected;
        [_selectedView setHidden:!selected];
    }
}

- (void)resetLastEndOffsetX {
    _lastEndOffsetX = _lastBeginOffsetX;
    [_framesContainer setContentOffset:CGPointMake(_lastBeginOffsetX, 0) animated:NO];
}

- (void)resetLastEndDurationX {
    _lastEndDurationX = _lastBeginDurationX;
}

- (IBAction)selectedClick:(id)sender {
    if (!_selected) {
        _selected = YES;
        [_selectedView setHidden:NO];
        if (_delegate && [_delegate respondsToSelector:@selector(videoClipView:didSelected:)]) {
            [_delegate videoClipView:self didSelected:clip->getId()];
        }
    }
}

- (IBAction)unselectedClick:(id)sender {
    if (_selected) {
        _selected = NO;
        [_selectedView setHidden:YES];
        if (_delegate && [_delegate respondsToSelector:@selector(videoClipView:didUnselected:)]) {
            [_delegate videoClipView:self didUnselected:clip->getId()];
        }
    }
}

@end
