//
//  XEditTimelineCutView.m
//  XEdit
//
//  Created by DFSX on 2019/2/28.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import "XEditTimelineCutView.h"

@implementation XEditTimelineCutView

+ (instancetype)timelineCutView {
    NSArray *array = [[NSBundle xEditBundle] loadNibNamed:@"XEditTimelineCutView" owner:self options:nil];
    XEditTimelineCutView *view = (XEditTimelineCutView *)[array firstObject];
    return view;
}

- (IBAction)didClickSplit:(id)sender {
    self.didEnterSplit();
}

- (IBAction)didClickSeparate:(id)sender {
    self.didEnterSeparate();
}

- (IBAction)didClickCopy:(id)sender {
    self.didEnterCopy();
}

@end
