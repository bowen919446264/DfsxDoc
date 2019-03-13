//
//  XEditVideoTrackTableViewCell.mm
//  XEdit
//
//  Created by DFSX on 2018/8/2.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "XEditVideoTrackTableViewCell.h"
#import "XEditVideoClipView.h"

#include <IXEngine.h>
#include <ITimeLine.h>

@interface XEditVideoTrackTableViewCell () <XEditVideoClipViewDelegate>

@property (weak, nonatomic) IBOutlet UIScrollView *clipsScrollView;

@property (strong, nonatomic) XEditVideoClipView *currentSelectedView;

@property (strong, nonatomic) NSMutableArray<XEditVideoClipView *> *clipViews;
@property (strong, nonatomic) NSMutableArray<NSLayoutConstraint *> *clipWidthConstraints;

@end

@implementation XEditVideoTrackTableViewCell {
    
    xedit::ITrack *track;
    xedit::IClip *currentSelectedClip;
    
}

- (void)awakeFromNib {
    [super awakeFromNib];
    
    _clipViews = [NSMutableArray new];
    _clipWidthConstraints = [NSMutableArray new];
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(clipDurationChangeBegan:) name:XEditClipDurationChangeBegan object:nil];
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(clipDurationChanged:) name:XEditClipDurationChanged object:nil];
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(clipDurationChangeEnded:) name:XEditClipDurationChangeEnded object:nil];
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(clipOffsetChangeBegan:) name:XEditClipOffsetChangeBegan object:nil];
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(clipOffsetChanged:) name:XEditClipOffsetChanged object:nil];
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(clipOffsetChangeEnded:) name:XEditClipOffsetChangeEnded object:nil];
}

- (void)setTrackIndex:(int)trackIndex {
    _trackIndex = trackIndex;
    track = xedit::IXEngine::getSharedInstance()->getTimeLine()->getTrack(xedit::ETrackType_Video, trackIndex);
    if (track) {
        [self loadUI];
    }
}

- (void)loadUI {
    for (XEditVideoClipView *view in _clipViews) {
        view.delegate = nil;
        [view removeFromSuperview];
    }
    [_clipViews removeAllObjects];
    [_clipWidthConstraints removeAllObjects];
    
    CGFloat lastEndXOrigin = 0.0;
    int clipCount = track->getClipCount();
    for (int i=0; i < clipCount; i++) {
        xedit::IClip *clip = track->getClip(i);
        double clipDuration = clip->getDuration().doubleValue();
        XEditVideoClipView *view = [XEditVideoClipView videoClipViewWithClip:clip];
        view.delegate = self;
        [_clipsScrollView addSubview:view];
        [_clipViews addObject:view];
        [view autoSetDimension:ALDimensionHeight toSize:_clipsScrollView.frame.size.height];
        NSLayoutConstraint *widthConstraint = [view autoSetDimension:ALDimensionWidth toSize:clipDuration * XEditFrameWidth];
        [_clipWidthConstraints addObject:widthConstraint];
        [view autoPinEdgeToSuperviewEdge:ALEdgeTop];
        if (i == 0) {
            [view autoPinEdgeToSuperviewEdge:ALEdgeLeading withInset:0];
        } else {
            XEditVideoClipView *lastView = [_clipViews objectAtIndex:i - 1];
            [view autoPinEdge:ALEdgeLeading toEdge:ALEdgeTrailing ofView:lastView];
        }
        lastEndXOrigin += clipDuration * XEditFrameWidth;
    }
    [_clipsScrollView setContentSize:CGSizeMake(lastEndXOrigin, 0)];
}

- (xedit::ITrack *)getTrack {
    return track;
}

- (xedit::IClip *)getCurrentSelectedClip {
    return currentSelectedClip;
}

- (UIView *)getCurrentSelectedView {
    return _currentSelectedView;
}

- (void)cancelSelected {
    [_currentSelectedView setSelected:NO];
    _currentSelectedView = nil;
    currentSelectedClip = NULL;
}

- (void)updateUIForClipSplitedWithLeftClip:(xedit::IClip *)leftClip rightClip:(xedit::IClip *)rightClip {
    if (_currentSelectedView) {
        _currentSelectedView.delegate = nil;
        [_currentSelectedView removeFromSuperview];
        NSUInteger index = [_clipViews indexOfObject:_currentSelectedView];
        if (index < _clipViews.count) {
            [_clipViews removeObjectAtIndex:index];
            [_clipWidthConstraints removeObjectAtIndex:index];
            //左切片加入
            double leftClipDuration = leftClip->getDuration().doubleValue();
            XEditVideoClipView *leftView = [XEditVideoClipView videoClipViewWithClip:leftClip];
            leftView.delegate = self;
            [_clipsScrollView addSubview:leftView];
            [_clipViews insertObject:leftView atIndex:index];
            [leftView autoSetDimension:ALDimensionHeight toSize:_clipsScrollView.frame.size.height];
            NSLayoutConstraint *leftWidthConstraint = [leftView autoSetDimension:ALDimensionWidth toSize:leftClipDuration * XEditFrameWidth];
            [_clipWidthConstraints insertObject:leftWidthConstraint atIndex:index];
            [leftView autoPinEdgeToSuperviewEdge:ALEdgeTop];
            if (index == 0) {
                [leftView autoPinEdgeToSuperviewEdge:ALEdgeLeading withInset:0];
            } else {
                XEditVideoClipView *lastView = [_clipViews objectAtIndex:index - 1];
                [leftView autoPinEdge:ALEdgeLeading toEdge:ALEdgeTrailing ofView:lastView];
            }
            //右切片加入
            double rightClipDuration = rightClip->getDuration().doubleValue();
            XEditVideoClipView *rightView = [XEditVideoClipView videoClipViewWithClip:rightClip];
            rightView.delegate = self;
            [_clipsScrollView addSubview:rightView];
            [_clipViews insertObject:rightView atIndex:index + 1];
            [rightView autoSetDimension:ALDimensionHeight toSize:_clipsScrollView.frame.size.height];
            NSLayoutConstraint *rightWidthConstraint = [rightView autoSetDimension:ALDimensionWidth toSize:rightClipDuration * XEditFrameWidth];
            [_clipWidthConstraints insertObject:rightWidthConstraint atIndex:index + 1];
            [rightView autoPinEdgeToSuperviewEdge:ALEdgeTop];
            [rightView autoPinEdge:ALEdgeLeading toEdge:ALEdgeTrailing ofView:leftView];
            if (index + 2 < _clipViews.count) {
                XEditVideoClipView *nextView = [_clipViews objectAtIndex:index + 2];
                [nextView autoPinEdge:ALEdgeLeading toEdge:ALEdgeTrailing ofView:rightView];
            }
        }
        //由于此时切片所属视图和被拆分的切片已不存在，且后续响应方法中不需要使用这两个参数，所以默认给一个空，id默认给一个0
        [self videoClipView:_currentSelectedView didUnselected:0];
    }
}

#pragma mark - Notification Methods

- (void)clipDurationChangeBegan:(NSNotification *)notification {
    XEditVideoClipView *view = (XEditVideoClipView *)notification.object;
    if (view == _currentSelectedView && currentSelectedClip) {
        if (_delegate && [_delegate respondsToSelector:@selector(videoTrackTableViewCell:track:clipDurationChangeBegan:)]) {
            [_delegate videoTrackTableViewCell:self track:track clipDurationChangeBegan:currentSelectedClip];
        }
    }
}

- (void)clipDurationChanged:(NSNotification *)notification {
    XEditVideoClipView *view = (XEditVideoClipView *)notification.object;
    if (view == _currentSelectedView && currentSelectedClip) {
        NSDictionary *userInfo = notification.userInfo;
        if (userInfo) {
            NSUInteger index = [_clipViews indexOfObject:view];
            if (index < _clipWidthConstraints.count) {
                NSLayoutConstraint *widthConstraint = [_clipWidthConstraints objectAtIndex:index];
                CGFloat changedWidth = ((NSNumber *)[userInfo objectForKey:kXEditClipDurationChangedValue]).floatValue;
                widthConstraint.constant = changedWidth;
                if (_delegate && [_delegate respondsToSelector:@selector(videoTrackTableViewCell:track:clip:durationChanged:)]) {
                    CGFloat totalWidth = 0.0;
                    for (NSLayoutConstraint *widthConstraint in _clipWidthConstraints) {
                        totalWidth += widthConstraint.constant;
                    }
                    [_delegate videoTrackTableViewCell:self track:track clip:currentSelectedClip durationChanged:totalWidth];
                }
            }
        }
    }
}

- (void)clipDurationChangeEnded:(NSNotification *)notification {
    XEditVideoClipView *view = (XEditVideoClipView *)notification.object;
    if (view == _currentSelectedView && currentSelectedClip) {
        NSDictionary *userInfo = notification.userInfo;
        if (userInfo) {
            CGFloat changedDuration = ((NSNumber *)[userInfo objectForKey:kXEditClipDurationChangeEndedValue]).floatValue;
            Rational duration = Rational(changedDuration, XEditFrameWidth);
            int result = track->changeClipDuration(currentSelectedClip->getId(), duration);
            if (result < 0) {
                [UIAlertController showOneActionWithViewController:[[[self superview] superview] viewController]  title:@"温馨提示" message:@"改变切片时长失败" actionTitle:@"确定" handler:nil];
                NSUInteger index = [_clipViews indexOfObject:view];
                if (index < _clipWidthConstraints.count) {
                    NSLayoutConstraint *widthConstraint = [_clipWidthConstraints objectAtIndex:index];
                    widthConstraint.constant = changedDuration - view.lastEndDurationX;
                    changedDuration = widthConstraint.constant;
                    [view resetLastEndDurationX];
                }
            }
            if (_delegate && [_delegate respondsToSelector:@selector(videoTrackTableViewCell:track:clipDurationChangeEnded:durationChanged:)]) {
                [_delegate videoTrackTableViewCell:self track:track clipDurationChangeEnded:currentSelectedClip durationChanged:changedDuration];
            }
        }
    }
}

- (void)clipOffsetChangeBegan:(NSNotification *)notification {
    XEditVideoClipView *view = (XEditVideoClipView *)notification.object;
    if (view == _currentSelectedView && currentSelectedClip) {
        if (_delegate && [_delegate respondsToSelector:@selector(videoTrackTableViewCell:track:clipOffsetChangeBegan:)]) {
            [_delegate videoTrackTableViewCell:self track:track clipOffsetChangeBegan:currentSelectedClip];
        }
    }
}

- (void)clipOffsetChanged:(NSNotification *)notification {
    XEditVideoClipView *view = (XEditVideoClipView *)notification.object;
    if (view == _currentSelectedView && currentSelectedClip) {
        NSDictionary *userInfo = notification.userInfo;
        if (userInfo) {
            CGFloat changedValue = ((NSNumber *)[userInfo objectForKey:kXEditClipOffsetChangedValue]).floatValue;
            if (_delegate && [_delegate respondsToSelector:@selector(videoTrackTableViewCell:track:clip:OffsetChanged:)]) {
                [_delegate videoTrackTableViewCell:self track:track clip:currentSelectedClip OffsetChanged:changedValue];
            }
        }
    }
}

- (void)clipOffsetChangeEnded:(NSNotification *)notification {
    XEditVideoClipView *view = (XEditVideoClipView *)notification.object;
    if (view == _currentSelectedView && currentSelectedClip) {
        NSDictionary *userInfo = notification.userInfo;
        if (userInfo) {
            CGFloat changedOffset = ((NSNumber *)[userInfo objectForKey:kXEditClipOffsetChangeEndedValue]).floatValue;
            Rational rational = Rational(changedOffset, XEditFrameWidth);
            int result = track->changeClipOffsetInMedia(currentSelectedClip->getId(), rational);
            if (result < 0) {
                [UIAlertController showOneActionWithViewController:[[[self superview] superview] viewController]  title:@"温馨提示" message:@"改变切片偏移量失败" actionTitle:@"确定" handler:nil];
                NSUInteger index = [_clipViews indexOfObject:view];
                if (index < _clipWidthConstraints.count) {
                    NSLayoutConstraint *widthConstraint = [_clipWidthConstraints objectAtIndex:index];
                    widthConstraint.constant += changedOffset;
                    [view resetLastEndOffsetX];
                    changedOffset = view.lastEndOffsetX;
                    if (_delegate && [_delegate respondsToSelector:@selector(videoTrackTableViewCell:track:clipDurationChangeEnded:durationChanged:)]) {
                        [_delegate videoTrackTableViewCell:self track:track clipDurationChangeEnded:currentSelectedClip durationChanged:widthConstraint.constant];
                    }
                }
            }
            if (_delegate && [_delegate respondsToSelector:@selector(videoTrackTableViewCell:track:clipOffsetChangeEnded:OffsetChanged:)]) {
                [_delegate videoTrackTableViewCell:self track:track clipOffsetChangeEnded:currentSelectedClip OffsetChanged:changedOffset];
            }
        }
    }
}

#pragma mark - XEditVideoClipViewDelegate

-(void)videoClipView:(XEditVideoClipView *)view didSelected:(int64_t)clipId {
    if (_currentSelectedView && currentSelectedClip) {
        if (_currentSelectedView == view && currentSelectedClip->getId() == clipId) { return; }
        [_currentSelectedView setSelected:NO];
        currentSelectedClip = NULL;
    }
    _currentSelectedView = view;
    currentSelectedClip = track->getClipById(clipId);
    
    [NSNotificationCenter.defaultCenter postNotificationName:XEditAClipDidSelected object:self];
}

-(void)videoClipView:(XEditVideoClipView *)view didUnselected:(int64_t)clipId {
    _currentSelectedView = nil;
    currentSelectedClip = NULL;
    [NSNotificationCenter.defaultCenter postNotificationName:XEditAClipDidUnselected object:self];
}

#pragma marl - Others

- (void)dealloc {
    [NSNotificationCenter.defaultCenter removeObserver:self name:XEditClipDurationChangeBegan object:nil];
    [NSNotificationCenter.defaultCenter removeObserver:self name:XEditClipDurationChanged object:nil];
    [NSNotificationCenter.defaultCenter removeObserver:self name:XEditClipDurationChangeEnded object:nil];
    [NSNotificationCenter.defaultCenter removeObserver:self name:XEditClipOffsetChangeBegan object:nil];
    [NSNotificationCenter.defaultCenter removeObserver:self name:XEditClipOffsetChanged object:nil];
    [NSNotificationCenter.defaultCenter removeObserver:self name:XEditClipOffsetChangeEnded object:nil];
}

@end
