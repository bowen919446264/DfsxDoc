//
//  XEditProject.m
//  XEdit
//
//  Created by DFSX on 2019/3/12.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import "XEditProject.h"

#define kPROJECTID @"projectID"
#define kPROJECTNAME @"projectName"
#define kPROJECTFIRSTFRAME @"projectFirstFrame"
#define kPROJECTPATH @"projectPath"
#define kPROJECTLASTDATE @"projectLastDate"
#define kPROJECTDURATION @"projectDuration"

@interface XEditProject ()

@property (assign, nonatomic, readwrite) int64_t Id;

@end

@implementation XEditProject

+ (instancetype)projectWithId:(int64_t)Id {
    return [[XEditProject alloc] initWithId:Id];
}

- (instancetype)initWithId:(int64_t)Id {
    self = [super init];
    if (self) {
        self.Id = Id;
    }
    return self;
}

#pragma mark - NSCoding

- (void)encodeWithCoder:(nonnull NSCoder *)aCoder {
    [aCoder encodeInt64:self.Id forKey:kPROJECTID];
    [aCoder encodeObject:self.name forKey:kPROJECTNAME];
    [aCoder encodeObject:self.firstFrame forKey:kPROJECTFIRSTFRAME];
    [aCoder encodeObject:self.path forKey:kPROJECTPATH];
    [aCoder encodeObject:self.lastDate forKey:kPROJECTLASTDATE];
    [aCoder encodeDouble:self.duration forKey:kPROJECTDURATION];
}

- (nullable instancetype)initWithCoder:(nonnull NSCoder *)aDecoder {
    self = [super init];
    if (self) {
        self.Id = [aDecoder decodeInt64ForKey:kPROJECTID];
        self.name = [aDecoder decodeObjectForKey:kPROJECTNAME];
        self.firstFrame = [aDecoder decodeObjectForKey:kPROJECTFIRSTFRAME];
        self.path = [aDecoder decodeObjectForKey:kPROJECTPATH];
        self.lastDate = [aDecoder decodeObjectForKey:kPROJECTLASTDATE];
        self.duration = [aDecoder decodeDoubleForKey:kPROJECTDURATION];
    }
    return self;
}

@end
