//
//  XEditProjectOutputTableViewCell.h
//  XEdit
//
//  Created by DFSX on 2018/8/2.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface XEditProjectOutputTableViewCell : UITableViewCell

@property (weak, nonatomic) IBOutlet UIView *selectBackgroundView;
@property (weak, nonatomic) IBOutlet UIImageView *selectImageView;
@property (weak, nonatomic) IBOutlet UILabel *titleLabel;
@property (weak, nonatomic) IBOutlet UIImageView *titleImageView;

@property (assign, getter=outputIsSelected, nonatomic) BOOL outputSelected;

- (void)setOutputType:(XEditOutputType *)outputType;

@end
