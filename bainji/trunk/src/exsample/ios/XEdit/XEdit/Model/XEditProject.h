//
//  XEditProject.h
//  XEdit
//
//  Created by DFSX on 2019/3/12.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 工程
 */
@interface XEditProject : NSObject <NSCoding>

/**
 工程ID
 */
@property (assign, nonatomic, readonly) int64_t Id;

/**
 工程名称
 */
@property (copy, nonatomic) NSString *name;

/**
 首帧
 */
@property (strong, nonatomic) NSData *firstFrame;

/**
 工程本地路径
 */
@property (copy, nonatomic) NSString *path;

/**
 工程最近一次保存的时间
 */
@property (strong, nonatomic) NSDate *lastDate;

/**
 工程时长
 */
@property (assign, nonatomic) double duration;

/**
 构造工程

 @param Id 工程id
 @return 工程对象
 */
+ (instancetype)projectWithId:(int64_t)Id;

@end
