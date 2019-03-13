//
//  XEditDefine.h
//  XEdit
//
//  Created by DFSX on 2018/8/1.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#ifndef XEditDefine_h
#define XEditDefine_h

#define kSCREENWIDTH  [[UIScreen mainScreen] bounds].size.width
#define kSCREENHEIGHT [[UIScreen mainScreen] bounds].size.height
#define IOS_VERSION_LATER(version) (([[[UIDevice currentDevice] systemVersion] floatValue] >= version) ? (YES):(NO))
#define UIColorFromHexString(s) [UIColor colorWithRed:(((s & 0xFF0000) >> 16 )) / 255.0 green:((( s & 0xFF00 ) >> 8 )) / 255.0 blue:(( s & 0xFF )) / 255.0 alpha:1.0]
#define RGB(r,g,b,a) [UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:a]
#define kWeakSelf(type)  __weak typeof(type) weak##type = type;
#define kStrongSelf(type)  __strong typeof(type) type = weak##type;

#endif /* XEditDefine_h */
