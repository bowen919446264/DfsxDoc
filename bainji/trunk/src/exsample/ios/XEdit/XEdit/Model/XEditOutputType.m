//
//  XEditOutputType.m
//  XEdit
//
//  Created by DFSX on 2019/3/11.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import "XEditOutputType.h"

@interface XEditOutputType ()

@property (nonatomic, copy, readwrite) NSString *name;
@property (nonatomic, assign, readwrite) int width;
@property (nonatomic, assign, readwrite) int height;

@end

@implementation XEditOutputType

+ (instancetype)outputType1080P {
    return [[XEditOutputType alloc] initWithName:@"1080P" width:1920 height:1080];
}

+ (instancetype)outputType720P {
    return [[XEditOutputType alloc] initWithName:@"720P" width:1280 height:720];
}

+ (instancetype)outputType480P {
    return [[XEditOutputType alloc] initWithName:@"480P" width:720 height:480];
}

- (instancetype)initWithName:(NSString *)name width:(int)width height:(int)height {
    self = [super init];
    if (self) {
        self.name = name;
        self.width = width;
        self.height = height;
    }
    return self;
}

@end
