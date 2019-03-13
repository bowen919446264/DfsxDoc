//
//  XEditTimelineVolumeView.h
//  XEdit
//
//  Created by DFSX on 2019/2/28.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface XEditTimelineVolumeView : UIView

+ (instancetype)timelineVolumeView;

@property(nonatomic, assign) float volume;

typedef void(^FloatBlock)(float);
typedef void(^VoidBlock)(void);
@property(nonatomic, strong) VoidBlock didEnterAutoChange;
@property(nonatomic, strong) FloatBlock didVolumeChanged;

@end
