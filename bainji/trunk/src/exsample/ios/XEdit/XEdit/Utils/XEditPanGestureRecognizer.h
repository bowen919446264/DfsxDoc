//
//  XEditPanGestureRecognizer.h
//  XEdit
//
//  Created by DFSX on 2018/8/3.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef NS_ENUM(NSInteger, XEditPanDirection) {
    XEditPanDirectionHorizontal,
    XEditPanDirectionVertical
};

///只识别水平或垂直方向的滑动手势
@interface XEditPanGestureRecognizer : UIPanGestureRecognizer

@property (assign, readonly, nonatomic) XEditPanDirection direction;

+ (instancetype)panGestureWithDirection:(XEditPanDirection)direction target:(id)target selector:(SEL)selector;

@end
