//
// Created by wendachuan on 2018/12/6.
//

#include "media/ImageMedia.h"
#include "media/AVMedia.h"
#include "media/SVGMedia.h"

using namespace xedit;

/**
 * 创建媒体
 * @param mediaType
 * @return
 */
IInnerMedia* xedit::createMedia(EMediaType mediaType) {
    switch (mediaType) {
        case EMediaType_Image:
            return new CImageMedia();
        case EMediaType_AV:
            return new CAVMedia();
        case EMediaType_SVG:
            return new CSVGMedia();
        default:
            return NULL;
    }
}