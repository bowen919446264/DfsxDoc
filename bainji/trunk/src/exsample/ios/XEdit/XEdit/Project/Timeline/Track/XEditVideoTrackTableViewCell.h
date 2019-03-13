//
//  XEditVideoTrackTableViewCell.h
//  XEdit
//
//  Created by DFSX on 2018/8/2.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import <UIKit/UIKit.h>

#include <ITrack.h>

@interface XEditVideoTrackTableViewCell : UITableViewCell

@property (nonatomic, assign) int trackIndex;

- (xedit::ITrack *)getTrack;
- (xedit::IClip *)getCurrentSelectedClip;
- (UIView *)getCurrentSelectedView;
- (void)cancelSelected;
- (void)updateUIForClipSplitedWithLeftClip:(xedit::IClip *)leftClip rightClip:(xedit::IClip *)rightClip;

@end

@protocol XEditVideoTrackTableViewCellDelegate <NSObject>

- (void)videoTrackTableViewCell:(XEditVideoTrackTableViewCell *)cell track:(xedit::ITrack *)track clipDurationChangeBegan:(xedit::IClip *)clip;
- (void)videoTrackTableViewCell:(XEditVideoTrackTableViewCell *)cell track:(xedit::ITrack *)track clip:(xedit::IClip *)clip durationChanged:(CGFloat)changedWidth;
- (void)videoTrackTableViewCell:(XEditVideoTrackTableViewCell *)cell track:(xedit::ITrack *)track clipDurationChangeEnded:(xedit::IClip *)clip durationChanged:(CGFloat)changedWidth;

- (void)videoTrackTableViewCell:(XEditVideoTrackTableViewCell *)cell track:(xedit::ITrack *)track clipOffsetChangeBegan:(xedit::IClip *)clip;
- (void)videoTrackTableViewCell:(XEditVideoTrackTableViewCell *)cell track:(xedit::ITrack *)track clip:(xedit::IClip *)clip OffsetChanged:(CGFloat)changedWidth;
- (void)videoTrackTableViewCell:(XEditVideoTrackTableViewCell *)cell track:(xedit::ITrack *)track clipOffsetChangeEnded:(xedit::IClip *)clip OffsetChanged:(CGFloat)changedWidth;

@end

@interface XEditVideoTrackTableViewCell ()

@property (weak, nonatomic) id<XEditVideoTrackTableViewCellDelegate> delegate;

@end
