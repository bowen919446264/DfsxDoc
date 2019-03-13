//
// Created by wendachuan on 2018/12/25.
//

#include "TimeLineGenerator.h"
#include "NLEHelper.h"
#include "avpub/StatusCode.h"
#include "TimeLine.h"
#include "xengine/IXEngine.h"
#include "avpub/Log.h"
using namespace xedit;

CTimeLineGenerator::CTimeLineGenerator(CTimeLine *pTimeLine): m_pTimeLine(pTimeLine)
{

}

CTimeLineGenerator::~CTimeLineGenerator() {

}

/**
 * 生成
 * 异步操作，调用后立即返回，并不代表生成已经完成
 * @param pTimeLine 时间线
 * @param param 生成参数
 * @param pObserver 生成观察者
 * @return 返回0表示成功；否则返回失败代码
 */
StatusCode CTimeLineGenerator::generate() {
    TDsSmartPtr<IDsPlayEngineManager> pEngineMgr = m_pTimeLine->getDsPlayEngineManager();
    assert(pEngineMgr);

    m_pCompiler = pEngineMgr->GetCompiler();
    assert(m_pCompiler);

    m_pCompiler->SetCompilerStatusNotify(generateCallback);

    const GenerateSetting& generateSetting = m_pTimeLine->getGenerateSetting();
    SDsExportFileInfo exportFileInfo = {0};
    exportFileInfo.m_bExportVideo = true;
    exportFileInfo.m_bExportAudio = true;
    exportFileInfo.m_frameStart = generateSetting.rStartTime.nNum * NLE_FPS / generateSetting.rStartTime.nDen;
    exportFileInfo.m_frameEnd = generateSetting.rDuration.nNum * NLE_FPS / generateSetting.rDuration.nDen;
    snprintf(exportFileInfo.m_wszVideoFile, sizeof(exportFileInfo.m_wszVideoFile) - 1, "%s/%s", generateSetting.strDestDir, generateSetting.strDestName);
    strcpy(exportFileInfo.m_wszAudioFile, "");

    string format;
    NLEHelper::generateSettingToDsFormat(generateSetting, &format);
    AVLOG(ELOG_LEVEL_INFO, "输出文件: %s, 编码参数: %s", exportFileInfo.m_wszVideoFile, format.c_str());
    StatusCode code = m_pCompiler->DoFastCompile(format.c_str(), &exportFileInfo, 1, 0);
    return code;
}

/**
 * 取消生成
 */
void CTimeLineGenerator::cancel() {
    m_pCompiler->Stop();
}

HRESULT CTimeLineGenerator::generateCallback(EDs_EngineStatus engineStatus, long operatorStatus) {
    CTimeLine *pTimeLine = dynamic_cast<CTimeLine*>(IXEngine::getSharedInstance()->getTimeLine());
    IGenerateObserver *pObserver = pTimeLine->getGenerateObserver();
    const GenerateSetting& generateSetting = pTimeLine->getGenerateSetting();
    switch (engineStatus) {
        // 生成的当前位置，返回已经生成的帧数
        case e_DS_COMPILER_SETP:
            pObserver->onUpdateProcess(generateSetting, Rational(operatorStatus, NLE_FPS));
            return AV_OK;

        // 生成完成
        case e_DS_COMPILER_FINISHED:
            pObserver->onFinish(generateSetting, AV_OK);
            return AV_OK;

        // 生成失败
        case e_DS_COMPILER_FAILED:
            pObserver->onFinish(generateSetting, AV_OTHER_ERROR);
            return AV_OK;

        // 写文件失败，返回已经生成的帧数
        case e_DS_COMPILER_WRITEFILE_FILED:
            pObserver->onFinish(generateSetting, AV_OTHER_ERROR);
            return AV_OK;

        // 目录文件已经存在，且不能删除
        case e_DS_COMPILER_FILE_EXIST_ERROR:
            pObserver->onFinish(generateSetting, AV_OTHER_ERROR);
            return AV_OK;

        default:
            return AV_OK;
    }
}
