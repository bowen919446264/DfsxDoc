//
//  XEditTimelineViewController.h
//  XEdit
//
//  Created by DFSX on 2018/8/1.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import <UIKit/UIKit.h>
#include <ITimeLine.h>

@interface XEditTimelineViewController : UIViewController

- (void)onPositionDidChanged:(Rational)newPosition;
- (void)onTrackCreated:(xedit::ITrack *)pTrack;
- (void)onTrackRemoved:(ID)trackId;
- (void)onTimeLineStatusChanged:(xedit::ETimeLineStatus)newStatus;
- (int)onReceivedVideoBuffer:(xedit::IVideoBuffer *)buffer;

- (void)setupXEdit;
- (void)destroyXEdit;
- (void)addMedia:(NSString *)mediaPath;
- (void)play;
- (void)pause;
- (void)undo;
- (void)redo;
- (xedit::PreviewFrame *)getFirstFrame;

typedef void(^BufferBlock)(CVPixelBufferRef);
typedef void(^TimelineStatusBlock)(xedit::ETimeLineStatus);
@property(nonatomic, copy) BufferBlock didReceivedVideoBuffer;
@property(nonatomic, copy) TimelineStatusBlock didTimelineStatusChanged;

@end
