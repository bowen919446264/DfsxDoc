//
// Created by wendachuan on 2018/11/30.
//

#ifndef PROJECT_GSIZE_H
#define PROJECT_GSIZE_H

namespace xedit {
    /**
     * 图像尺寸
     */
    struct GSize {
        int nWidth;
        int nHeight;

        GSize() {
            nWidth = nHeight = 0;
        }

        GSize(int nWidth, int nHeight) {
            this->nWidth = nWidth;
            this->nHeight = nHeight;
        }
    };
}

#endif //PROJECT_GSIZE_H
