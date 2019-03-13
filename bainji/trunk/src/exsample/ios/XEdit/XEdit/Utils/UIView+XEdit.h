//
//  UIView+XEdit.h
//  XEdit
//
//  Created by DFSX on 2018/8/1.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIView (XEdit)

/**
 视图圆角

 @param radius 角度
 */
- (void)setCornerWithRadius:(CGFloat)radius;

/**
 视图边框

 @param width 宽度
 @param color 颜色
 */
- (void)setBorderWithWidth:(CGFloat)width color:(UIColor *)color;

/**
 获取当前视图的控制器

 @return 当前视图s的控制器
 */
- (UIViewController *)viewController;

@end
