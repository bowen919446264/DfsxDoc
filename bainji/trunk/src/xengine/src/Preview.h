//
// Created by wendachuan on 2018/12/4.
//

#ifndef PROJECT_MEDIAPREVIEW_H
#define PROJECT_MEDIAPREVIEW_H

#include "IInnerPreview.h"
#include <vector>
using namespace std;

namespace xedit {
    /**
     * 媒体预览
     */
    class CPreview: public IInnerPreview {
    public:
        CPreview(GSize size);
        virtual ~CPreview();

        /**
         * 获得预览图像尺寸
         * @return
         */
        virtual GSize getPreviewSize() const;

        /**
         * 获得预览帧数量
         * @return
         */
        virtual int getPreviewFrameCount() const;

        /**
         * 获得指定序号的预览帧
         * @param nIndex
         * @return
         */
        virtual PreviewFrame* getPreviewFrame(int nIndex) const;

        /**
         * 添加预览帧
         * @param previewFrame
         */
        virtual void addPreviewFrame(const PreviewFrame& previewFrame);

        /**
         * 获得指定位置附近的一个预览帧
         * @param rOffsetInMedia 在文件中的时间偏移量
         * @return
         */
        virtual PreviewFrame* getPreviewFrameNearBy(Rational rOffsetInMedia) const;

    private:
        GSize m_size;
        vector<PreviewFrame*>   m_previewFrames;
    };
}



#endif //PROJECT_MEDIAPREVIEW_H
