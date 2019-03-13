//
//  XEditTimelineTotalTimeCollectionViewCell.m
//  XEdit
//
//  Created by DFSX on 2018/8/1.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "XEditTimelineTotalTimeCollectionViewCell.h"

@interface XEditTimelineTotalTimeCollectionViewCell ()

@property (weak, nonatomic) IBOutlet UILabel *timeLabel;

@end

@implementation XEditTimelineTotalTimeCollectionViewCell

- (void)setTime:(long)time {
    if (_time != time) {
        _time = time;
        int remainder = time % 5;
        if (remainder == 0 || time == 1) {
            _timeLabel.text = [NSString stringWithFormat:@"%ld", time];
        } else {
            _timeLabel.text = @"●";
        }
    }
}

@end
