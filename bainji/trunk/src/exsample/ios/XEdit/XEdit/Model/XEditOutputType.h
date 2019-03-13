//
//  XEditOutputType.h
//  XEdit
//
//  Created by DFSX on 2019/3/11.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface XEditOutputType : NSObject

@property (nonatomic, copy, readonly) NSString *name;
@property (nonatomic, assign, readonly) int width;
@property (nonatomic, assign, readonly) int height;

+ (instancetype)outputType1080P;
+ (instancetype)outputType720P;
+ (instancetype)outputType480P;

@end
