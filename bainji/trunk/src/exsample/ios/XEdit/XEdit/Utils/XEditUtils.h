//
//  XEditUtils.h
//  XEdit
//
//  Created by DFSX on 2018/8/1.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Photos/Photos.h>

@interface XEditUtils : NSObject

+ (BOOL)assetIsHEIC:(PHAsset * _Nonnull)asset;
+ (NSData * _Nullable)HEIC2JPEG:(NSData * _Nonnull)heicData;
+ (UIViewController * _Nonnull)getCurrentViewController;
+ (NSString * _Nonnull)getTimeHHmmssFromSeconds:(NSInteger)seconds;
@end
