//
//  XEditTimelineObserver.hpp
//  XEdit
//
//  Created by DFSX on 2019/2/22.
//  Copyright © 2019 Lskyme. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <ITimeLine.h>

using namespace xedit;

class XEditTimelineObserver : public ITimeLineObserver {
    
    XEditTimelineViewController *mainVC;
    
public:
    
    XEditTimelineObserver(XEditTimelineViewController *mainViewController) {
        this->mainVC = mainViewController;
    }
    
    void releaseMainVC() {
        this->mainVC = NULL;
    }
    
    /**
     * 位置改变回调处理
     * @param rNewPos 新位置
     */
    void onPosDidChanged(Rational rNewPos) {
        NSLog(@"onPosDidChanged!");
        [mainVC onPositionDidChanged:rNewPos];
    }
    
    /**
     * 新建轨道事件
     * @param pTrack 新建的轨道
     */
    void onTrackCreated(ITrack *pTrack) {
        NSLog(@"onTrackCreated!");
        [mainVC onTrackCreated:pTrack];
    }
    
    /**
     * 轨道移除事件
     * @param trackId 被移除的轨道id
     */
    void onTrackRemoved(ID trackId) {
        NSLog(@"onTrackRemoved!");
        [mainVC onTrackRemoved:trackId];
    }
    
    /**
     * 时间线状态改变事件
     * @param newStatus 新的状态
     */
    void onTimeLineStatusChanged(ETimeLineStatus newStatus) {
        NSLog(@"onTimeLineStatusChanged!");
        [mainVC onTimeLineStatusChanged:newStatus];
    }
};
