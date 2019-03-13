//
//  XEditMainViewController.mm
//  XEdit
//
//  Created by DFSX on 2018/8/1.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "XEditMainViewController.h"
#import "XEditCreateProjectCollectionViewCell.h"
#import "XEditProjectCollectionViewCell.h"
#import "XEditProjectDetailViewController.h"

#include <IXEngine.h>

#define kPROJECTCELLID  @"projectCell"
#define kPROJECTCREATECELLID @"createProjectCell"

@interface XEditMainViewController () <UICollectionViewDelegate, UICollectionViewDataSource>

@property (weak, nonatomic) IBOutlet UIView *collectionContainer;
@property (strong, nonatomic) UICollectionView *collectionView;
@property (strong, nonatomic) UICollectionViewFlowLayout *flowLayout;

@property (strong, nonatomic) NSMutableArray *projects;

@property (assign, nonatomic) BOOL presentToAssetsPicker;

@end

@implementation XEditMainViewController {
    
    xedit::IXEngine *engine;
    
}

- (void)viewDidLoad {
    [super viewDidLoad];
    //test
//    _projects = [NSMutableArray new];
    _projects = @[@"1",@"2",@"3"];
    
    [self setupCollectionViews];
    [self initEngine];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    _presentToAssetsPicker = NO;
    UIApplication.sharedApplication.statusBarStyle = UIStatusBarStyleLightContent;
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    if (_presentToAssetsPicker) {
        UIApplication.sharedApplication.statusBarStyle = UIStatusBarStyleDefault;
    }
}

- (void)setupCollectionViews {
    _flowLayout = [[UICollectionViewFlowLayout alloc] init];
    _flowLayout.scrollDirection = UICollectionViewScrollDirectionVertical;
    _flowLayout.sectionInset = UIEdgeInsetsMake(9, 15, 9, 15);
    _flowLayout.minimumLineSpacing = 9;
    _flowLayout.minimumInteritemSpacing = 10;
    CGFloat width = (kSCREENWIDTH - 40) / 2;
    CGFloat height = width / 168 * 72;
    _flowLayout.itemSize = CGSizeMake(width, height);
    
    _collectionView = [[UICollectionView alloc] initWithFrame:CGRectZero collectionViewLayout:_flowLayout];
    _collectionView.delegate = self;
    _collectionView.dataSource = self;
    _collectionView.backgroundColor = UIColor.blackColor;
    _collectionView.showsVerticalScrollIndicator = NO;
    _collectionView.showsHorizontalScrollIndicator = NO;
    [_collectionView registerNib:[UINib nibWithNibName:@"XEditCreateProjectCollectionViewCell" bundle:[NSBundle xEditBundle]]forCellWithReuseIdentifier:kPROJECTCREATECELLID];
    [_collectionView registerNib:[UINib nibWithNibName:@"XEditProjectCollectionViewCell" bundle:[NSBundle xEditBundle]]forCellWithReuseIdentifier:kPROJECTCELLID];
    [_collectionContainer addSubview:_collectionView];
    [_collectionView autoPinEdgesToSuperviewEdges];
}

- (void)initEngine {
    xedit::EngineSetting engineSetting;
    memset(&engineSetting, 0, sizeof(engineSetting));
    NSString *cachePath = [self getEngineCachePath];
    NSString *logPath = [self getEngineLogPath];
    memcpy(engineSetting.cacheDir, [cachePath cStringUsingEncoding:NSUTF8StringEncoding], [cachePath length]);
    memcpy(engineSetting.logDir, [logPath cStringUsingEncoding:NSUTF8StringEncoding], [cachePath length]);
    
    engine = xedit::IXEngine::getSharedInstance();
    engine->initialize(engineSetting);
}

- (NSString *)getEngineCachePath {
    NSString* docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString* cachePath = [docPath stringByAppendingPathComponent:@"xedit/cache"];
    BOOL isDirectory = NO;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL existed = [fileManager fileExistsAtPath:cachePath isDirectory:&isDirectory];
    if ( !(isDirectory == YES && existed == YES) ) {//如果文件夹不存在
        [fileManager createDirectoryAtPath:cachePath withIntermediateDirectories:YES attributes:nil error:nil];
    }
    return cachePath;
}

- (NSString *)getEngineLogPath {
    NSString* docPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString* logPath = [docPath stringByAppendingPathComponent:@"xedit/log"];
    BOOL isDirectory = NO;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL existed = [fileManager fileExistsAtPath:logPath isDirectory:&isDirectory];
    if ( !(isDirectory == YES && existed == YES) ) {//如果文件夹不存在
        [fileManager createDirectoryAtPath:logPath withIntermediateDirectories:YES attributes:nil error:nil];
    }
    return logPath;
}

#pragma mark - UICollectionViewDelegate UICollectionViewDataSource

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return 1;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return _projects.count + 1;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.item == _projects.count) {
        XEditCreateProjectCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:kPROJECTCREATECELLID forIndexPath:indexPath];
        return cell;
    } else {
        XEditProjectCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:kPROJECTCELLID forIndexPath:indexPath];
        return cell;
    }
}

- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.item == _projects.count) {
        XEditProjectDetailViewController *detailVC = [[UIStoryboard storyboardWithName:@"XEditProject" bundle:[NSBundle xEditBundle]] instantiateViewControllerWithIdentifier:@"XEditProjectDetailViewController"];
        [self.navigationController pushViewController:detailVC animated:YES];
    } else {
        [UIAlertController showOneActionWithViewController:self title:@"温馨提示" message:@"打开工程开发中" actionTitle:@"确定" handler:nil];
    }
}

@end
