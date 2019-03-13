//
//  XEditProjectOutputViewController.h
//  XEdit
//
//  Created by DFSX on 2018/8/2.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "ITimeLine.h"

@interface XEditProjectOutputViewController : UIViewController

@property(copy, nonatomic) NSString *firstFramePath;

- (void)onGenerateFinished:(StatusCode)code;
- (void)onGenerateUpdateProgress:(Rational)duration;

@end
