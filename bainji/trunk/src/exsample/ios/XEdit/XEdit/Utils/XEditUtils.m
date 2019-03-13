//
//  XEditUtils.m
//  XEdit
//
//  Created by DFSX on 2018/8/1.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "XEditUtils.h"

@implementation XEditUtils

+ (BOOL)assetIsHEIC:(PHAsset * _Nonnull)asset {
    if IOS_VERSION_LATER(9.0) {
        NSArray<PHAssetResource *> *resourceList = [PHAssetResource assetResourcesForAsset:asset];
        for (PHAssetResource *assetResource in resourceList) {
            NSString *uti = assetResource.uniformTypeIdentifier;
            if ([uti isEqualToString:@"public.heif"] || [uti isEqualToString:@"public.heic"]) {
                return YES;
            }
        }
    } else {
        NSString *uti = [asset valueForKey:@"uniformTypeIdentifier"];
        if ([uti isEqualToString:@"public.heif"] || [uti isEqualToString:@"public.heic"]) {
            return YES;
        }
    }
    
    return NO;
}

+ (NSData * _Nullable)HEIC2JPEG:(NSData * _Nonnull)heicData {
    NSData *jpegData = nil;
    UIImage *image = [[UIImage alloc] initWithData:heicData];
    if (image) {
        NSMutableData *destinationData = [NSMutableData new];
        CGImageDestinationRef destination = CGImageDestinationCreateWithData((__bridge CFMutableDataRef)destinationData, (__bridge CFStringRef)@"public.jpeg", 1, NULL);
        if (destination) {
            NSDictionary *options = @{(__bridge NSString *)kCGImageDestinationLossyCompressionQuality: @(1)};
            CGImageDestinationAddImage(destination, image.CGImage, (__bridge CFDictionaryRef)options);
            CGImageDestinationFinalize(destination);
            jpegData = destinationData;
            CFRelease(destination);
        }
    }
    return jpegData;
}

+ (UIViewController * _Nonnull)getCurrentViewController {
    UIViewController *result = nil;
    UIWindow * window = [[UIApplication sharedApplication] keyWindow];
    if (window.windowLevel != UIWindowLevelNormal) {
        NSArray *windows = [[UIApplication sharedApplication] windows];
        for(UIWindow * tmpWin in windows) {
            if (tmpWin.windowLevel == UIWindowLevelNormal) {
                window = tmpWin;
                break;
            }
        }
    }
    
    UIView *frontView = [[window subviews] objectAtIndex:0];
    id nextResponder = [frontView nextResponder];
    
    if ([nextResponder isKindOfClass:[UIViewController class]]) {
        result = nextResponder;
    } else {
        result = window.rootViewController;
    }
    
    return result;
}

+ (NSString * _Nonnull)getTimeHHmmssFromSeconds:(NSInteger)seconds {
    NSString *str_hour = [NSString stringWithFormat:@"%02ld",seconds/3600];
    NSString *str_minute = [NSString stringWithFormat:@"%02ld",(seconds%3600)/60];
    NSString *str_second = [NSString stringWithFormat:@"%02ld",seconds%60];
    NSString *format_time = [NSString stringWithFormat:@"%@:%@:%@",str_hour,str_minute,str_second];
    
    return format_time;
}

@end
