//
//  UIAlertController+XEdit.m
//  XEdit
//
//  Created by DFSX on 2019/2/18.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import "UIAlertController+XEdit.h"

@implementation UIAlertController (XEdit)

+ (void)showOneActionWithViewController:(UIViewController *)viewController title:(NSString *)title message:(NSString *)message actionTitle:(NSString *)actionTitle handler:(void (^)(UIAlertAction *))handler {
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:title message:message preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *action = [UIAlertAction actionWithTitle:actionTitle style:UIAlertActionStyleDefault handler:handler];
    [alertController addAction:action];
    if (viewController) {
        [viewController presentViewController:alertController animated:YES completion:nil];
    } else {
        [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:alertController animated:YES completion:nil];
    }
    
}

+ (void)showTwoActionWithViewController:(UIViewController *)viewController title:(NSString *)title message:(NSString *)message firstActionTitle:(NSString *)firstActionTitle secondActionTitle:(NSString *)secondActionTitle firstActionHandler:(void (^)(UIAlertAction *))firstActionhandler secondActionHandler:(void (^)(UIAlertAction *))secondActionHandler {
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:title message:message preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *firstAction = [UIAlertAction actionWithTitle:firstActionTitle style:UIAlertActionStyleDefault handler:firstActionhandler];
    [alertController addAction:firstAction];
    UIAlertAction *secondAction = [UIAlertAction actionWithTitle:secondActionTitle style:UIAlertActionStyleDefault handler:secondActionHandler];
    [alertController addAction:secondAction];
    if (viewController) {
        [viewController presentViewController:alertController animated:YES completion:nil];
    } else {
        [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:alertController animated:YES completion:nil];
    }
}

@end
