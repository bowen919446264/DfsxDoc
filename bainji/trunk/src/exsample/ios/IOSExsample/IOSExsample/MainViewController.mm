//
//  MainViewController.m
//  IOSExsample
//
//  Created by wendachuan on 2018/6/27.
//  Copyright © 2018年 成都东方盛行电子有限责任公司. All rights reserved.
//

#import "MainViewController.h"
#import "IXEngine.h"
using namespace xedit;

@interface MainViewController ()

@end

@implementation MainViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    IXEngine *pEngine = IXEngine::getSharedInstance();
    ITimeLine *pTimeLine = pEngine->getTimeLine();
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
