//
//  XEditProjectOutputProgressView.h
//  XEdit
//
//  Created by DFSX on 2019/3/11.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface XEditProjectOutputProgressView : UIView

/**
 进度（最小为0，最大为1）
 */
@property (assign, nonatomic) float progressValue;

+ (instancetype)projectOutputProgressView;
- (void)hide;

typedef void(^VoidBlock)(void);
@property(nonatomic, strong) VoidBlock didCanceled;

@end
