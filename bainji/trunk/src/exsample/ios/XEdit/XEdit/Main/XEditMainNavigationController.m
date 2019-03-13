//
//  XEditMainNavigationController.m
//  XEdit
//
//  Created by DFSX on 2018/8/6.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "XEditMainNavigationController.h"

@implementation UIViewController (NavigationBackButton)

@end

@interface XEditMainNavigationController () <UINavigationBarDelegate>

@end

@implementation XEditMainNavigationController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self.interactivePopGestureRecognizer setEnabled:NO];
}

#pragma mark - UINavigationBarDelegate

- (BOOL)navigationBar:(UINavigationBar *)navigationBar shouldPopItem:(UINavigationItem *)item {
    if([self.viewControllers count] < [navigationBar.items count]) {
        return YES;
    }
    
    BOOL shouldPop = YES;
    UIViewController* vc = [self topViewController];
    if([vc respondsToSelector:@selector(navigationShouldPop)]) {
        shouldPop = [vc navigationShouldPop];
    }
    
    if(shouldPop) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self popViewControllerAnimated:YES];
        });
    } else {
        for(UIView *subview in [navigationBar subviews]) {
            if(0. < subview.alpha && subview.alpha < 1.) {
                [UIView animateWithDuration:.25 animations:^{
                    subview.alpha = 1.;
                }];
            }
        }
    }
    
    return NO;
}

@end
