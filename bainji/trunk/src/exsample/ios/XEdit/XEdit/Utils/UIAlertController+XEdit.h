//
//  UIAlertController+XEdit.h
//  XEdit
//
//  Created by DFSX on 2019/2/18.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface UIAlertController (XEdit)

/**
 快速构造单Action提示

 @param viewController 要弹出的控制器
 @param title 标题
 @param message 信息
 @param actionTitle 动作名字
 @param handler 动作操作
 */
+ (void)showOneActionWithViewController:(UIViewController * _Nullable)viewController title:(NSString *)title message:(NSString *)message actionTitle:(NSString *)actionTitle handler:(void (^ __nullable)(UIAlertAction *action))handler;

/**
 快速构造双Action提示

 @param viewController 要弹出的控制器
 @param title 标题
 @param message 信息
 @param firstActionTitle 第一个动作名字
 @param secondActionTitle 第二个动作名字
 @param firstActionhandler 第一个动作操作
 @param secondActionHandler 第二个动作操作
 */
+ (void)showTwoActionWithViewController:(UIViewController * _Nullable)viewController title:(NSString *)title message:(NSString *)message firstActionTitle:(NSString *)firstActionTitle secondActionTitle:(NSString *)secondActionTitle firstActionHandler:(void (^ __nullable)(UIAlertAction *action))firstActionhandler secondActionHandler:(void (^ __nullable)(UIAlertAction *action))secondActionHandler;

@end
