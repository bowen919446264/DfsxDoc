//
//  XEditVideoRenderer.hpp
//  XEdit
//
//  Created by DFSX on 2019/2/22.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <ITimeLine.h>

using namespace xedit;

class XEditVideoRenderer: public IVideoRenderer {
    
    XEditTimelineViewController *mainVC;
    
public:
    
    XEditVideoRenderer(XEditTimelineViewController *mainViewController) {
        this->mainVC = mainViewController;
    }
    
    void releaseMainVC() {
        this->mainVC = NULL;
    }
    
    /**
     * 渲染
     * @param pBuffer 待渲染的buffer
     * @return 返回0表示成功；否则返回失败代码
     */
    StatusCode render(IBuffer *pBuffer) {
        IVideoBuffer *videoBuffer = dynamic_cast<IVideoBuffer *>(pBuffer);
        if (videoBuffer) {
            NSLog(@"didIVideoBufferReceived!");
            return [mainVC onReceivedVideoBuffer:videoBuffer];
        } else {
            return 0;
        }
    }
    
    /**
     * 初始化renderer
     * @param nWidth 目标宽度
     * @param nHeight 目标高度
     * @param ePixFormat 目标像素格式
     * @return 返回0表示成功；否则返回失败代码
     */
    StatusCode init(int nWidth, int nHeight, EPixFormat ePixFormat) {
        return 0;
    }
    
};
