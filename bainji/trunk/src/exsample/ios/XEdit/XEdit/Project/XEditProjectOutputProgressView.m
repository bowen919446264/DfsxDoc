//
//  XEditProjectOutputProgressView.m
//  XEdit
//
//  Created by DFSX on 2019/3/11.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import "XEditProjectOutputProgressView.h"

@interface XEditProjectOutputProgressView ()

@property (weak, nonatomic) IBOutlet UIButton *cancelButton;
@property (weak, nonatomic) IBOutlet UILabel *progressLabel;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *progressViewTrailingConstraint;

@end

@implementation XEditProjectOutputProgressView

+ (instancetype)projectOutputProgressView {
    NSArray *array = [[NSBundle xEditBundle] loadNibNamed:@"XEditProjectOutputProgressView" owner:self options:nil];
    XEditProjectOutputProgressView *view = (XEditProjectOutputProgressView *)[array firstObject];
    view.backgroundColor = [[UIColor blackColor] colorWithAlphaComponent:0.7];
    [view.cancelButton setBorderWithWidth:2 color:[UIColor whiteColor]];
    return view;
}

- (void)setProgressValue:(float)progressValue {
    if (_progressValue != progressValue) {
        _progressValue = progressValue;
        if (_progressValue < 0) {
            _progressValue = 0;
        }
        if (_progressValue > 1) {
            _progressValue = 1;
        }
        _progressLabel.text = [NSString stringWithFormat:@"正在输出  %d%%", (int)(_progressValue * 100)];
        _progressViewTrailingConstraint.constant = 215 * (1 - _progressValue);
    }
}

- (void)hide {
    [self setProgressValue:0];
    [self removeFromSuperview];
}

- (IBAction)clickCancel:(id)sender {
    self.didCanceled();
    [self hide];
}

@end
