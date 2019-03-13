//
//  XEditVideoClipView.h
//  XEdit
//
//  Created by DFSX on 2018/8/3.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import <UIKit/UIKit.h>

#include <IClip.h>

@interface XEditVideoClipView : UIView
@property (assign, getter=isSelected, nonatomic) BOOL selected;

+ (instancetype)videoClipViewWithClip:(xedit::IClip *)clip;

@property (assign, nonatomic, readonly) CGFloat lastEndOffsetX;
@property (assign, nonatomic, readonly) CGFloat lastEndDurationX;

- (void)resetLastEndOffsetX;
- (void)resetLastEndDurationX;

@end

@protocol XEditVideoClipViewDelegate <NSObject>

- (void)videoClipView:(XEditVideoClipView *)view didSelected:(int64_t)clipId;
- (void)videoClipView:(XEditVideoClipView *)view didUnselected:(int64_t)clipId;

@end

@interface XEditVideoClipView ()

@property (weak, nonatomic) id<XEditVideoClipViewDelegate> delegate;

@end
