//
// Created by wendachuan on 2018/11/7.
//

#include "Project1.h"
#include "avpub/Log.h"
#include "avpub/AVMemory.h"
#include "avpub/StatusCode.h"

using namespace xedit;
using namespace libav;

#define CurrentProject xedit::CProject1

/**
 * 创建工程
 * @param setting 工程参数
 * @param version 工程版本，<= 0 表示使用最新版本
 * @return
 */
IInnerProject* xedit::createProject(const ProjectSetting& setting, int version) {
    IInnerProject *project = NULL;
    if (version <= 0) {
        project = new CurrentProject();
    } else {
        if (version == 1)
            project = new CProject1();
        else {
            AVLOG(ELOG_LEVEL_ERROR, "Project version %d not supported", version);
            return NULL;
        }
    }

    project->setSetting(setting);
    return project;
}

/**
 * 分析工程版本
 * @param projectBuffer
 * @param bufferCount
 * @return <= 0表示失败，否则返回工程版本号
 */
static int parseProjectVersion(const uint8_t *projectBuffer, int bufferCount) {
    xmlDocPtr doc = NULL;
    xmlNodePtr projectNode = NULL;
    xmlChar* xmlTmpChar = NULL;
    char tmp[21] = {0};
    int version = 0;

    // 分析xml文档
    doc = xmlParseDoc(BAD_CAST projectBuffer);
    if (!doc) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlParseDoc失败!");
        goto failure;
    }

    projectNode = xmlDocGetRootElement(doc);
    if (!projectNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlDocGetRootElement失败!");
        goto failure;
    }

    if (xmlStrcmp(projectNode->name, BAD_CAST "project")) {
        AVLOG(ELOG_LEVEL_ERROR, "缺少project节点!");
        goto failure;
    }

    xmlTmpChar = xmlGetProp(projectNode, BAD_CAST "version");
    if (!xmlTmpChar) {
        AVLOG(ELOG_LEVEL_ERROR, "project节点缺少version属性!");
        goto failure;
    }

    xmlFreeDoc(doc);

    version = strtol((char*)xmlTmpChar, NULL, 10);
    return version;

failure:
    if (doc)
        xmlFreeDoc(doc);
}

/**
 * 载入工程
 * @param pInputStream 输入流
 * @return NULL表示载入失败，否则返回工程对象
 */
API_IMPORT_EXPORT
IInnerProject* xedit::loadProject(IInputStream *pInputStream) {
    int64_t size = 0;
    uint8_t *buffer = NULL;
    StatusCode code = AV_OK;
    int version = 0, count = 0;
    IInnerProject *project = NULL;

    // 读取数据
    size = pInputStream->available();
    buffer = (uint8_t *)AVMalloc(size + 1);
    if (!buffer) {
        code = AV_NULL_PTR;
        goto failure;
    }

    count = pInputStream->read(buffer, size);
    if (count < 0) {
        AVLOG(ELOG_LEVEL_ERROR, "从流中读取数据失败!");
        goto failure;
    }

    buffer[count] = '\0';

    // 解析工程版本号
    version = parseProjectVersion(buffer, count);
    if (version <= 0) {
        AVLOG(ELOG_LEVEL_ERROR, "不能识别的工程，没有发现工程版本号");
        goto failure;
    }


    if (version == 1)
        project = new CProject1();
    else {
        project = new CurrentProject();
    }

    // 载入工程
    code = project->load((char*)buffer, count);
    if (code < 0) {
        AVLOG(ELOG_LEVEL_ERROR, "载入工程失败!");
        goto failure;
    }

    if (buffer) {
        AVFree((void**)&buffer);
    }
    return project;

failure:
    if (buffer) {
        AVFree((void**)&buffer);
    }
    if (project)
        delete project;
    return NULL;
}