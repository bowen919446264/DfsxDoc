//
//  XEditProjectOutputTableViewCell.m
//  XEdit
//
//  Created by DFSX on 2018/8/2.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "XEditProjectOutputTableViewCell.h"

@implementation XEditProjectOutputTableViewCell

- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
}

- (void)setOutputType:(XEditOutputType *)outputType {
    _titleLabel.text = outputType.name;
    _titleImageView.image = nil;
}

- (void)setOutputSelected:(BOOL)outputSelected {
    if (_outputSelected != outputSelected) {
        _outputSelected = outputSelected;
        if (outputSelected) {
            [_selectBackgroundView setBorderWithWidth:2 color:UIColorFromHexString(0xFFC107)];
            [_selectImageView setImage:[UIImage imageNamed:@"choice_enter"]];
            _titleLabel.textColor = UIColorFromHexString(0xFFC107);
        } else {
            [_selectBackgroundView setBorderWithWidth:0 color:nil];
            [_selectImageView setImage:[UIImage imageNamed:@"choice"]];
            _titleLabel.textColor = UIColor.whiteColor;
        }
    }
}

@end
