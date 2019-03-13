//
// Created by wendachuan on 2018/12/4.
//

#include "Preview.h"
#include "xutil/StdUtil.h"

using namespace xedit;

CPreview::CPreview(GSize size):
        m_size(size) {

}

CPreview::~CPreview() {
    destoryVector(&m_previewFrames);
}

/**
 * 获得预览图像尺寸
 * @return
 */
GSize CPreview::getPreviewSize() const {
    return m_size;
}

/**
 * 获得预览帧数量
 * @return
 */
int CPreview::getPreviewFrameCount() const {
    return m_previewFrames.size();
}

/**
 * 获得指定序号的预览帧
 * @param nIndex
 * @return
 */
PreviewFrame* CPreview::getPreviewFrame(int nIndex) const {
    if (nIndex < m_previewFrames.size())
        return m_previewFrames[nIndex];
    else
        return NULL;
}

static bool comparePreviewFrame(const PreviewFrame *a, const PreviewFrame *b){
    return a->rTimeOffset < b->rTimeOffset;
}

/**
 * 添加预览帧
 * @param previewFrame
 */
void CPreview::addPreviewFrame(const PreviewFrame& previewFrame) {
    PreviewFrame *pPreviewFrame = new PreviewFrame();
    *pPreviewFrame = previewFrame;
    m_previewFrames.push_back(pPreviewFrame);
    sort(m_previewFrames.begin(), m_previewFrames.end(), comparePreviewFrame);
}

/**
 * 获得指定位置附近的一个预览帧
 * @param rOffsetInMedia 在文件中的时间偏移量
 * @return
 */
PreviewFrame* CPreview::getPreviewFrameNearBy(Rational rOffsetInMedia) const {
    int nCurrentIndex = -1;
    Rational rCurrentMinDistance(INT64_MAX, 1);
    for (int i = 0; i < m_previewFrames.size(); i++) {
        Rational rDistance = (m_previewFrames[i]->rTimeOffset - rOffsetInMedia).absValue();
        if (nCurrentIndex < 0 || rDistance < rCurrentMinDistance) {
            nCurrentIndex = i;
            rCurrentMinDistance = rDistance;
        }
    }
    if (nCurrentIndex < 0)
        return NULL;
    return m_previewFrames[nCurrentIndex];
}