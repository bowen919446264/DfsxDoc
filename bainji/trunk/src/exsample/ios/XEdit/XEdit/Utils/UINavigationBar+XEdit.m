//
//  UINavigationBar+XEdit.m
//  XEdit
//
//  Created by DFSX on 2018/8/1.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "UINavigationBar+XEdit.h"

@implementation UINavigationBar (XEdit)

+ (void)hideBackButtonText {
    if (IOS_VERSION_LATER(11.0)) {
        [UIBarButtonItem.appearance setBackButtonTitlePositionAdjustment:UIOffsetMake(-200, 0) forBarMetrics:UIBarMetricsDefault];
    } else {
        [UIBarButtonItem.appearance setBackButtonTitlePositionAdjustment:UIOffsetMake(CGFLOAT_MIN, CGFLOAT_MIN) forBarMetrics:UIBarMetricsDefault];
    }
}

@end
