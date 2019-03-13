//
// Created by wendachuan on 2018/6/27.
//

#include "xengine/IXEngine.h"
#include "XEngine.h"
#include "avpub/api.h"

using namespace libav;

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

#if __ANDROID__
#include <libavcodec/jni.h>
#endif
}

namespace xedit {
    static IXEngine* gEngine = NULL;

    /**
     * 获得全局共享引擎实例
     * @return
     */
    IXEngine* IXEngine::getSharedInstance() {
        if (!gEngine) {
            gEngine = new XEngine();
        }
        return gEngine;
    }

    /**
     * 销毁全局共享引擎实例
     */
    void IXEngine::destroySharedInstance() {
        SAFE_DELETE(gEngine);
    }

    /**
     * ffmpeg性能检测
     * @param path
     * @param bUseCpu
     * @return
     */
    static int checkFFMpegPerformace(const char* path, bool bUseCpu) {
        AVFormatContext *pFmtCtx = NULL;
        AVCodecContext *pCodecCtx = NULL;
        AVCodec *pCodec = NULL;
        AVFrame *pFrame = NULL;
        int nVideoStrmIdx = -1, ret = 0, packetPending = 0, nDecodeCount = 0;;
        AVPacket packet, pendingPacket;

        pCodecCtx = avcodec_alloc_context3(NULL);
        if (!pCodecCtx) goto end;

        ret = avformat_open_input(&pFmtCtx, path, NULL, NULL);
        if (ret < 0) goto end;

        ret = avformat_find_stream_info(pFmtCtx, NULL);
        if (ret < 0) goto end;

        for (int i = 0; i < pFmtCtx->nb_streams; i++) {
            if (pFmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                nVideoStrmIdx = i;
                break;
            }
        }
        if (nVideoStrmIdx < 0) goto end;

        ret = avcodec_parameters_to_context(pCodecCtx, pFmtCtx->streams[nVideoStrmIdx]->codecpar);
        if (ret < 0) goto end;

        pCodecCtx->pkt_timebase = pFmtCtx->streams[nVideoStrmIdx]->time_base;

        if (bUseCpu) {
            pCodec = avcodec_find_decoder(pFmtCtx->streams[nVideoStrmIdx]->codecpar->codec_id);
        } else {
            pCodec = avcodec_find_decoder_by_name("h264_mediacodec");
        }
        if (!pCodec) goto end;

        pCodecCtx->codec_id = pCodec->id;

        ret = avcodec_open2(pCodecCtx, pCodec, NULL);
        if (ret < 0) goto end;

        ret = av_seek_frame(pFmtCtx, nVideoStrmIdx, 0, AVSEEK_FLAG_BACKWARD);
        if (ret < 0) goto end;

        av_init_packet(&packet);
        av_init_packet(&pendingPacket);

        pFrame = av_frame_alloc();
        do {
            if (packetPending) {
                av_packet_move_ref(&packet, &pendingPacket);
                packetPending = 0;
            } else {
                // 读包
                ret = av_read_frame(pFmtCtx, &packet);
                if (ret < 0) {
                    if (ret == AVERROR_EOF || avio_feof(pFmtCtx->pb)) {
                        packet.stream_index = nVideoStrmIdx;
                        packet.data = NULL;
                        packet.size = 0;
                    } else if (pFmtCtx->pb && pFmtCtx->pb->error) {
                        break;
                    } else {
                        continue;
                    }
                } else if (packet.stream_index != nVideoStrmIdx) {
                    continue;
                }
            }

            // 包送解码器
            ret = avcodec_send_packet(pCodecCtx, &packet);
            if (ret == AVERROR(EAGAIN)) {
                packetPending = 1;
                av_packet_move_ref(&pendingPacket, &packet);
            }
            av_packet_unref(&packet);

            // 从解码器中拿数据
            do {
                ret = avcodec_receive_frame(pCodecCtx, pFrame);
                if (ret == AVERROR_EOF) {
                    avcodec_flush_buffers(pCodecCtx);
                    break;
                } else if (ret >= 0) {
                    nDecodeCount++;
                    av_frame_unref(pFrame);
                }
            } while (ret != AVERROR(EAGAIN));

        } while (ret == AVERROR(EAGAIN) || ret >= 0);

        end:
        if (pCodecCtx)
            avcodec_free_context(&pCodecCtx);
        if (pFmtCtx)
            avformat_free_context(pFmtCtx);
        return nDecodeCount;
    }

    /**
     * 检测是否GPU比CPU快
     * @param sampleFilePath 用于性能检测的视频文件
     * @return true - GPU比CPU快; false - CPU比GPU快
     */
    bool IXEngine::checkIfGpuFasterThanCpu(const char* sampleFilePath) {
#if __ANDROID__
        JavaVM *pJVM = (JavaVM*)av_jni_get_java_vm(NULL);
        if (!pJVM) {
            int nCode = av_jni_set_java_vm(g_pJVM, NULL);
            if (FAILED(nCode)) return false;
        }
#endif

        uint64_t nStartTime = AVGetCurrentTime();
        int nCode = checkFFMpegPerformace(sampleFilePath, true);
        uint64_t nCpuTime = AVGetCurrentTime() - nStartTime;
        if (nCode > 0) {
            nStartTime = AVGetCurrentTime();
            nCode = checkFFMpegPerformace(sampleFilePath, false);
            uint64_t nGpuTime = AVGetCurrentTime() - nStartTime;
            return nGpuTime < nCpuTime;
        }
        return false;
    }
}

#if __ANDROID__

JavaVM *g_pJVM = NULL;

/**
 *
 * @param vm
 * @param reserved
 * @return
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    g_pJVM = vm;
}
#endif