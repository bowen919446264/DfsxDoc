//
//  XEditTimelineCutView.h
//  XEdit
//
//  Created by DFSX on 2019/2/28.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface XEditTimelineCutView : UIView

+ (instancetype)timelineCutView;

typedef void(^VoidBlock)(void);
@property(nonatomic, strong) VoidBlock didEnterSplit;
@property(nonatomic, strong) VoidBlock didEnterSeparate;
@property(nonatomic, strong) VoidBlock didEnterCopy;

@end
