//
//  XEditExtern.h
//  XEdit
//
//  Created by DFSX on 2018/8/6.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import <Foundation/Foundation.h>

//每帧图片在轨道中的宽度
extern CGFloat const XEditFrameWidth;

//转场添加视图在轨道中的宽度
extern CGFloat const XEditAddTrasitionWidth;

//切片时长调整开始通知
extern NSString * const XEditClipDurationChangeBegan;
//切片时长调整中通知
extern NSString * const XEditClipDurationChanged;
//切片时长UI调整值Key
extern NSString * const kXEditClipDurationChangedValue;
//切片时长调整完毕通知
extern NSString * const XEditClipDurationChangeEnded;
//切片时长调整完毕值Key
extern NSString * const kXEditClipDurationChangeEndedValue;

//切片偏移量调整开始通知
extern NSString * const XEditClipOffsetChangeBegan;
//切片偏移量调整通知
extern NSString * const XEditClipOffsetChanged;
//切片偏移量调整值Key
extern NSString * const kXEditClipOffsetChangedValue;
//切片偏移量调整完毕通知
extern NSString * const XEditClipOffsetChangeEnded;
//切片偏移量调整完毕值Key
extern NSString * const kXEditClipOffsetChangeEndedValue;

//轨道上的一个切片被选中
extern NSString * const XEditAClipDidSelected;
//轨道上的一个切片被取消选中
extern NSString * const XEditAClipDidUnselected;
