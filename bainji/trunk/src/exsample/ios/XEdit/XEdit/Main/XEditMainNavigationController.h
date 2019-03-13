//
//  XEditMainNavigationController.h
//  XEdit
//
//  Created by DFSX on 2018/8/6.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol NavigationBackButtonProtocol <NSObject>

@optional
- (BOOL)navigationShouldPop;

@end

@interface UIViewController (NavigationBackButton) <NavigationBackButtonProtocol>

@end

@interface XEditMainNavigationController : UINavigationController

@end
