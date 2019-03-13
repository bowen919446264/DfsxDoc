//
//  NSBundle+XEdit.m
//  XEdit
//
//  Created by DFSX on 2018/8/1.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "NSBundle+XEdit.h"
#import "XEditBundleClass.h"

@implementation NSBundle (XEdit)

static NSBundle * __XEditBundle;

+ (NSBundle *)xEditBundle {
    if (!__XEditBundle) {
        __XEditBundle = [NSBundle bundleForClass: XEditBundleClass.class];
    }
    return __XEditBundle;
}

@end
