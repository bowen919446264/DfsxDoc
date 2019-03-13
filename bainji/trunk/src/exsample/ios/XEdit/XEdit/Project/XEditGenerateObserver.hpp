//
//  XEditGenerateObserver.hpp
//  XEdit
//
//  Created by DFSX on 2019/3/11.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <IGenerateObserver.h>

using namespace xedit;

class XEditGenerateObserver : public IGenerateObserver {
    
    XEditProjectOutputViewController *mainVC;
    
public:
    
    XEditGenerateObserver(XEditProjectOutputViewController *mainViewController) {
        this->mainVC = mainViewController;
    }
    
    void releaseMainVC() {
        this->mainVC = NULL;
    }
    
    /**
     * 生成结束回调处理
     * @param param 生成参数
     * @param code
     */
    void onFinish(const GenerateSetting& param, StatusCode code) {
        NSLog(@"onFinished!");
        [mainVC onGenerateFinished:code];
    }
    
    /**
     * 更新生成进度回调处理
     * @param param 生成参数
     * @param rDuration 当前已经生成的时长(秒)
     */
    void onUpdateProcess(const GenerateSetting& param, Rational rDuration) {
        NSLog(@"onUpdateProcess!");
        [mainVC onGenerateUpdateProgress:rDuration];
    }
    
};


