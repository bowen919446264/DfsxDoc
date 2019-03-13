//
//  XEditTimelineVolumeView.m
//  XEdit
//
//  Created by DFSX on 2019/2/28.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import "XEditTimelineVolumeView.h"

@interface XEditTimelineVolumeView ()

@property (weak, nonatomic) IBOutlet UILabel *percentLabel;
@property (weak, nonatomic) IBOutlet UISlider *volumeSlider;

@end

@implementation XEditTimelineVolumeView

+ (instancetype)timelineVolumeView {
    NSArray *array = [[NSBundle xEditBundle] loadNibNamed:@"XEditTimelineVolumeView" owner:self options:nil];
    XEditTimelineVolumeView *view = (XEditTimelineVolumeView *)[array firstObject];
    [view.volumeSlider setThumbImage:[UIImage imageNamed:@"slider_point"] forState:UIControlStateNormal];
    [view.volumeSlider setThumbImage:[UIImage imageNamed:@"slider_point"] forState:UIControlStateHighlighted];
    
    return view;
}

- (void)setVolume:(float)volume {
    _volume = volume;
    [_volumeSlider setValue:volume];
    _percentLabel.text = [NSString stringWithFormat:@"%d%%", (int)(volume * 100)];
}

- (IBAction)volumeChanged:(id)sender {
    self.didVolumeChanged(_volumeSlider.value);
    _volume = _volumeSlider.value;
    _percentLabel.text = [NSString stringWithFormat:@"%d%%", (int)(_volumeSlider.value * 100)];
}

- (IBAction)didClickAutoChange:(id)sender {
    self.didEnterAutoChange();
}

@end
