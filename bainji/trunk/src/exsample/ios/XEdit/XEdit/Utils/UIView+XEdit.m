//
//  UIView+XEdit.m
//  XEdit
//
//  Created by DFSX on 2018/8/1.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "UIView+XEdit.h"

@implementation UIView (XEdit)

- (void)setCornerWithRadius:(CGFloat)radius {
    [self.layer setCornerRadius:radius];
    [self.layer setMasksToBounds:YES];
}

- (void)setBorderWithWidth:(CGFloat)width color:(UIColor *)color {
    [self.layer setBorderWidth:width];
    [self.layer setBorderColor:color.CGColor];
}

- (UIViewController *)viewController {
    for (UIView* next = self; next; next = next.superview) {
        UIResponder *nextResponder = [next nextResponder];
        if ([nextResponder isKindOfClass:[UIViewController class]]) {
            return (UIViewController *)nextResponder;
        }
    }
    return nil;
}

@end
