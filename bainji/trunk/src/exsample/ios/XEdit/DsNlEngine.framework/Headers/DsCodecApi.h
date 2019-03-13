#pragma once
#include "DsNleDef.h"
#include "DsUtil.h"
#include "DsPublicApi.h"
namespace DSNleLib {
// 只有对IDecoder端的接口定义
// {4EE0E714-CFF3-46b7-A658-9B9BBBE582CC}
DEFINE_GUID(IID_IDsVideoPipe, 
0x4ee0e714, 0xcff3, 0x46b7, 0xa6, 0x58, 0x9b, 0x9b, 0xbb, 0xe5, 0x82, 0xcc);

interface IDsVideoPipe : public IUnknown
{
	virtual HRESULT __stdcall GetWriteBuffer(int32_t bufsize ,char **out_pbuf) = 0;
	//
	virtual HRESULT __stdcall PostWriteBuffer(char *in_pbuf,int64_t frameindex,int32_t pitch, int dhr,SDsSurfaceDescription stSurfaceInfo) = 0;
	// 
	virtual HRESULT __stdcall GetReadSurface(int64_t frameindex,IDsSurface **out_ppSurface,uint32_t wait = INFINITE) = 0;

	virtual HRESULT __stdcall Abort() = 0;
	virtual HRESULT __stdcall Flush() = 0;
	// called by decoder to notify no more frame decoded.
	virtual HRESULT __stdcall Finalize() = 0;
};

// {C09A5545-4FB7-433f-B441-709FC119787F}
DEFINE_GUID(IID_IDsAudioPipe, 
0xc09a5545, 0x4fb7, 0x433f, 0xb4, 0x41, 0x70, 0x9f, 0xc1, 0x19, 0x78, 0x7f);

interface IDsAudioPipe : public IUnknown
{
	virtual HRESULT __stdcall GetWriteBuffer(int32_t bufsize ,char **out_pbuf) = 0;
	virtual HRESULT __stdcall PostWriteBuffer(char *in_pbuf, int32_t usesize,int hr, SDsaWaveFormatInfo stWaveInfo) = 0;
	virtual HRESULT __stdcall Abort() = 0;
	virtual HRESULT __stdcall Flush() = 0;
	// called by decoder to notify no more frame decoded.
	virtual HRESULT __stdcall Finalize() = 0;

};
/// 内部用，是stream decoder.包含rawDecoder和解码数据的缓存

// {845C31BE-7261-4BD6-8BD0-3B7EDB3961BC}
DEFINE_GUID(IID_IDsDecoder, 
0x845c31be, 0x7261, 0x4bd6, 0x8b, 0xd0, 0x3b, 0x7e, 0xdb, 0x39, 0x61, 0xbc);

interface IDsDecoder : public IUnknown
{
	//???
	virtual HRESULT __stdcall SetVideoFormat(EDsSurfaceFormat *io_SurfaceFormat,int width,int height) = 0;
	virtual HRESULT __stdcall SetAudioFormat(const SDsaWaveFormatInfo *in_pfmt) = 0;
	virtual HRESULT __stdcall SetVideoPipe(IDsVideoPipe* in_pIVideoPipe) = 0;
	virtual HRESULT __stdcall SetAudioPipe(IDsAudioPipe* in_pIAudioPipe) = 0;
	virtual HRESULT __stdcall SetAudioStreamIndex(unsigned long in_ulStreamIndex) = 0;
	virtual HRESULT __stdcall Open(TCHAR* in_pwzFileName, TCHAR* in_pwzInxName, uint64_t in_Duration, uint64_t in_OriginalDuration, EDsPlayListType in_eType,int subtype ) = 0;
	virtual HRESULT __stdcall Start( uint64_t ftFrom, bool bNormal,int  mode) = 0;
	virtual HRESULT __stdcall RenderBuffer( uint64_t ftPostion ) = 0;
	virtual HRESULT __stdcall Close() = 0;
	virtual HRESULT __stdcall GetSurface( IDsSurface* out_ppCurUsingSurface[],int &count,int64_t frame, unsigned long in_ulWait ) = 0;
	virtual HRESULT __stdcall Open( void* in_pMbuf,int in_size, uint64_t in_Duration, uint64_t in_OriginalDuration, EDsPlayListType in_eType,int subtype ) = 0;
	virtual HRESULT __stdcall Flush() = 0;
	virtual HRESULT __stdcall Abort() = 0;
	virtual HRESULT __stdcall Finalize() = 0;

	// subtitle special
	virtual HRESULT __stdcall SubtitleSwitch(BOOL onff) = 0;
	virtual HRESULT __stdcall SubtitleSetIndex(int index) = 0;

	// video convert.

	virtual HRESULT __stdcall SetAfd(int afd) = 0; //for video
	virtual HRESULT __stdcall SetOutputPosition(SDsPointF *in_pos) = 0;
};
#define DEC_FT_RGBA	0
#define DEC_FT_YUY2	1


// {845C31BE-7261-4BD6-8BD0-3B7EDB3961BD}
DEFINE_GUID(IID_IRawDecoder, 
0x845c31be, 0x7261, 0x4bd6, 0x8b, 0xd0, 0x3b, 0x7e, 0xdb, 0x39, 0x61, 0xbd);

interface IRawDecoder : public IUnknown
{
	// afd = 0 为原始大小.
	virtual HRESULT __stdcall DecOpen(TCHAR     * in_pszFileName,
						int format,
						int afd) = 0;
	virtual HRESULT __stdcall DecOpen(		void     * in_pBuffer,             // media in buffer.
						int		 in_bufsize,
						int format,
						int afd) = 0;

	virtual HRESULT __stdcall GetInfo(SDsAvFileInfo * io_psInfo) = 0;
	//@ play 后可连续getbuffer.
	virtual HRESULT __stdcall Play( uint64_t ftPosition ) = 0;
	//@ seek 后只能取一次.
	virtual HRESULT __stdcall Seek( uint64_t ftPosition ) = 0;
	//@ 视频目前只支持rgba，大小为原始大小
	virtual HRESULT __stdcall GetVideoBuffer( void * VideoBuffer,int in_bufSize,int StridePitch ) = 0;
	//@ 音频按帧取
	virtual HRESULT __stdcall GetAudioBuffer(void* AudioBuffer, int in_bufSize)=0;

	//@ 关闭.
	virtual HRESULT __stdcall DecClose() = 0;


};
// {C0939BEB-BB42-4EE7-8C76-B7B0CB0E6477}
DEFINE_GUID(IID_IDsCodecMgr, 
0xc0939beb, 0xbb42, 0x4ee7, 0x8c, 0x76, 0xb7, 0xb0, 0xcb, 0xe, 0x64, 0x77);

interface IDsCodecMgr :public IUnknown
{
	//
	// <Summary>:
	//    Retrieves the file header information.
	// </Summary>
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_INVALID_STRUCTURE_SIZE, if the size field of the structure does not match the expected size.
	//    - DS_E_INVALID_POINTER, if one of the pointer parameters is NULL.
	//    - DS_E_FILE_NOT_FOUND, if the file does not exist.
	virtual HRESULT __stdcall GetFileInfo
		(
		TCHAR     * in_pszFileName,             // Name of the file to retrieve information from.
		SDsAvFileInfo * io_psInfo,                  // Structure that will be filled with the file information. The size field must be initialized properly.
		const CLSID * in_pclsidClip,          // Optional: Class identifier of the  Clip Reader component to get the file information from.
		CLSID * out_pclsidPreferredClipReader     // Optional: If more than one  Clip Reader can read the specified file, this will specify the most efficient reader's CLSID
		) = 0;	
	//
	// <Summary>:
	//    Create a Decoder from the playelement infomation
	// </Summary>
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	///仅内部使用.
	virtual HRESULT __stdcall CreateDecoder(IDsPlayListElement *in_pJple,IDsDecoder ** out_ppDecoder) = 0;

	/////新增接口

	virtual HRESULT __stdcall GetMediaInfo
		(
		void     * in_pBuffer,             // media in buffer.
		int		 in_bufsize,
		SDsAvFileInfo * io_psInfo,         // Structure that will be filled with the file information. The size field must be initialized properly.
		const CLSID * in_pclsidClip,          // Optional: Class identifier of the  Clip Reader component to get the file information from.
		CLSID * out_pclsidPreferredClipReader     // Optional: If more than one  Clip Reader can read the specified file, this will specify the most efficient reader's CLSID
		) = 0;	

	virtual HRESULT __stdcall CreateDecoder(const CLSID * in_decid,IRawDecoder **out_ppDec) = 0;


};


// {C0939BEB-BB42-4EE7-8C76-B7B0CB0E6478}
DEFINE_GUID(IID_IDsVaCodecIndex, 
0xc0939beb, 0xbb42, 0x4ee7, 0x8c, 0x76, 0xb7, 0xb0, 0xcb, 0xe, 0x64, 0x78);
interface IDsVaCodecIndex :public IUnknown
{
	virtual BOOL IsSupportIndex(TCHAR *in_strfile) = 0;
	virtual HRESULT CreateIndexFile(TCHAR *in_strfile
		,TCHAR *in_inxfile
		,BOOL(*callback)(LPVOID pv,int step)
		,LPVOID pv) = 0;
	virtual HRESULT DecStartCache(int flag) = 0;
	virtual HRESULT DecIsCached(TCHAR *in_strfile) = 0;
	virtual HRESULT DecCacheFile(TCHAR *in_strfile) = 0;
	virtual HRESULT DecStopCache() = 0;
};

// {C0939BEB-BB42-4EE7-8C76-B7B0CB0E6479}

///// Index                               值
///// 0      字幕输出用gpu内存或者cpu内存   [BOOL 1,0] default 1 
DEFINE_GUID(IID_IDsCodecParams, 
0xc0939beb, 0xbb42, 0x4ee7, 0x8c, 0x76, 0xb7, 0xb0, 0xcb, 0xe, 0x64, 0x79);
interface IDsCodecParams :public IUnknown
{
	virtual void SetParam(int index,void *in_value) = 0;
	virtual void GetParam(int index,void *out_value) = 0;
};

DEFINE_GUID(CLSID_VideoDec, 
0x845c31be, 0xffff, 0x4bd6, 0x8b, 0xd0, 0x3b, 0x7e, 0xdb, 0x39, 0x61, 0x01);
DEFINE_GUID(CLSID_AudioDec, 
0x845c31be, 0xffff, 0x4bd6, 0x8b, 0xd0, 0x3b, 0x7e, 0xdb, 0x39, 0x61, 0x02);
DEFINE_GUID(CLSID_CgDec, 
0x845c31be, 0xffff, 0x4bd6, 0x8b, 0xd0, 0x3b, 0x7e, 0xdb, 0x39, 0x61, 0x03);
DEFINE_GUID(CLSID_FxCgDec, 
0x845c31be, 0xffff, 0x4bd6, 0x8b, 0xd0, 0x3b, 0x7e, 0xdb, 0x39, 0x61, 0x04);
DEFINE_GUID(CLSID_CgSeqDec, 
0x845c31be, 0xffff, 0x4bd6, 0x8b, 0xd0, 0x3b, 0x7e, 0xdb, 0x39, 0x61, 0x05);
DEFINE_GUID(CLSID_FxCgLyricDec, 
0x845c31be, 0xffff, 0x4bd6, 0x8b, 0xd0, 0x3b, 0x7e, 0xdb, 0x39, 0x61, 0x06);
DEFINE_GUID(CLSID_FxCgScrollDec, 
0x845c31be, 0xffff, 0x4bd6, 0x8b, 0xd0, 0x3b, 0x7e, 0xdb, 0x39, 0x61, 0x07);
DEFINE_GUID(CLSID_FxCgCrawlDec, 
0x845c31be, 0xffff, 0x4bd6, 0x8b, 0xd0, 0x3b, 0x7e, 0xdb, 0x39, 0x61, 0x08);

}
