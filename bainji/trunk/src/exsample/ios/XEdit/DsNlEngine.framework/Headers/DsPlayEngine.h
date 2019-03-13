#pragma  once

#include <list>
using std::list;
#include "basicTypes.h"
#include "fxSettings.h"

//#include "IFxEffectSettings.h"
class IFxEffectSettings;

//#include "tech_review/DSTecCheck.h"

namespace PlayEngine
{

	extern "C" typedef BOOL (*pCreateIndexFileCallBack)(int step);

	// {13F4D2EC-3D9A-45BF-889C-635BF61D4B47}
	DEFINE_GUID(IID_IDsImporter, 
		0x13f4d2ec, 0x3d9a, 0x45bf, 0x88, 0x9c, 0x63, 0x5b, 0xf6, 0x1d, 0x4b, 0x47);

	interface IDsImporter: public IUnknown
	{
		virtual HRESULT  OpenFile(TCHAR *in_pszFileName, BOOL bSequence = FALSE) = 0;
		virtual HRESULT  CloseFile() = 0;
		virtual HRESULT  GetFileInfo(SDsFileInfo *SDsFileInfo) = 0;
		virtual HRESULT  GetFrame(uint64_t ui64Position, BYTE *pBuffer, int bufferSize, int videoAFD) = 0;
		virtual HRESULT  GetAudioFrame(uint64_t framePosition, BYTE *pBuffer, int bufferSize) = 0;

		virtual HRESULT  StreamOpen(TCHAR *in_pszFileName, uint64_t framePosition, BOOL bAudioFile = FALSE, int videoAFD = 0) = 0;
		virtual HRESULT  GetVideoFrame(BYTE *pBuffer, int bufferSize) = 0;
		virtual HRESULT  GetAudioFrame(BYTE *pBuffer, int bufferSize) = 0;
		virtual HRESULT  StreamClose() = 0;

		// ��Ƶ�����ļ�
		virtual BOOL IsSupportVideoIndexFile(TCHAR *in_pszFilePath) = 0;
		virtual HRESULT CreateVideoIndexFile(TCHAR *in_pszFilePath, TCHAR *in_pszIndexFile, pCreateIndexFileCallBack pCallback) = 0;
	};

	enum EPEFxEffectType
	{
		ePEFxEffectTypeVideo,
		ePEFxEffectTypeAudio
	};


	// {5F1136BF-33EA-477C-A9C9-5CF590C1D869}
	DEFINE_GUID(IID_IDsFxEffect, 
		0x5f1136bf, 0x33ea, 0x477c, 0xa9, 0xc9, 0x5c, 0xf5, 0x90, 0xc1, 0xd8, 0x69);

	interface IDsFxEffect: public IUnknown
	{
		// �����ؼ�����
		virtual EPEFxEffectType GetFxEffectType() = 0;
		// ����ؼ��ؼ�֡������Ϊ��λ��
		virtual HRESULT InsertGroupItemKeyFrame(int groupIndex, SDsKeyFrameFxGroupSettings &in_sFxGroupSettings) = 0;
		// ɾ���ؼ��ؼ�֡������Ϊ��λ��
		virtual HRESULT RemoveGroupItem(int groupIndex, float fPercentOfDuration) = 0;
		// �ƶ��ؼ�֡
		virtual HRESULT MoveGroupItemKeyFrame(int groupIndex, SDsKeyFrameFxGroupSettings &srcKeyFrame, SDsKeyFrameFxGroupSettings &dstKeyFrame) = 0;
		// �ƶ��ؼ�֡ʱ��ʼ��ȡ�̶��ؼ�֡���ؼ�����ֵ
		virtual HRESULT UseFixGroupKeyFrame(int groupIndex, int keyframeIndex) = 0;

		// �õ���ǰλ�õ��ؼ����ã����з��飩
		virtual HRESULT GetCurrentKeyFrameFxSettings(float fPercentDuration,  SDsSingleKeyFrameFxSettings ** out_ppCurFxSettings) = 0;

		// ��ӹ����ؼ����ƹؼ�֡
		virtual HRESULT InsertTransitionFxControlKeyFrame(SDsTransitionFxControlKeyFrame &sFxControlKeyFrame) = 0;
		// ɾ�������ؼ����ƹؼ�֡
		virtual HRESULT RemoveTransitionFxControlKeyFrame(float fKFPercentOfDuration) = 0;
		// ��ת��������ؼ�
		virtual HRESULT ReverseTransitionFx(bool bReverse) = 0;
		virtual HRESULT GetCurrentTransitionFxControlSettings(float fPercentOfDuration, SDsTransitionFxControlKeyFrame &sFxControlKeyFrame) = 0;
		virtual HRESULT GetCurrentEffectSettings(float fPercentOfDuration, IFxEffectSettings **pFxSetttings) = 0;
	};

	typedef std::vector<IDsFxEffect *> CEGFxEffectVector;

	struct SDsTransitionFxEffect
	{
		uint64_t m_framePosition; // �����ؼ���ʼλ��
		uint64_t m_frameDuration; // �����ؼ�����
		uint64_t frameFxPosition; // �����ؼ������ڹ���ϵı�ʶλ��
		EDsTransitionDirection m_eTransitionDirection; // �����ؼ��ķ���
		DSNleLib::TDsSmartPtr<IDsFxEffect> m_pJDsFxEffect; // �����ؼ�
	};
	typedef list<SDsTransitionFxEffect > TransitionFxEffectList;

	
	// {E1E4BC76-562A-4C34-94CC-9CF886E4A300}
	DEFINE_GUID(IID_IDsClip, 
	0xe1e4bc76, 0x562a, 0x4c34, 0x94, 0xcc, 0x9c, 0xf8, 0x86, 0xe4, 0xa3, 0x0);

	interface IDsClip: public IUnknown
	{
		virtual EClipType GetClipType() = 0;
		virtual SClipInfo GetClipInfo() = 0;
		virtual void SetClipInfo(SClipInfo &clipInfo) = 0;

		// �ز��Ƿ���framePositionλ����
		virtual bool In(uint64_t framePosition) = 0;

		virtual uint64_t GetFrameEndPosition() = 0;

		// ����ؼ�
		virtual HRESULT AddFxEffect(IDsFxEffect *pFxEffect, bool bVideoEffect) = 0;
		// ɾ���ؼ�
		virtual HRESULT RemoveFxEffect(IDsFxEffect *pFxEffect, bool bVideoEffect) = 0;

		// ��Ƶ �������
		virtual HRESULT EnableAudioChannel(int index, bool bEnable) = 0;

		// �����ؼ�����
		virtual int GetFxEffectCount(bool bVideoEffect) = 0;

		virtual void UpdateAudioFxMixer(SDsKFAudioFxSettings &audioMixerFxSettings) = 0;
		virtual void UpdateAudioChannelFx(int iAudioChannelIndex, SDsKFAudioFxSettings &audioFxSettings) = 0;
	};

	typedef list<IDsClip*> DsClipList;
	
	// {543BA1EB-EA35-44D8-A514-914FAEB72DFD}
	DEFINE_GUID(IID_IDsFileClip, 
	0x543ba1eb, 0xea35, 0x44d8, 0xa5, 0x14, 0x91, 0x4f, 0xae, 0xb7, 0x2d, 0xfd);

	interface IDsFileClip: public IDsClip
	{
		virtual EFileType GetFileClipType() = 0;
		virtual SFileClipInfo GetFileClipInfo() = 0;
		virtual void SetFileClipInfo(const SFileClipInfo &fileClipInfo) = 0;
		virtual void UpdateFxCompositorSettings(SDsKFCompositorSettingsList  &fxCompositorSettings) = 0;
		virtual void UpdateFxCompositorSettings(SDsKFCompositorSettings *fxCompositorSettings, int nSize) = 0;
		virtual void SetVideoAFDType(int nAFDType) = 0;

		virtual void UpdateMuteAreaList(MuteAreaList &muteAreaList) = 0;
	};


	// {60986619-BD8E-49F9-A979-B00A958B57EA}
	DEFINE_GUID(IID_IDsTrack, 
	0x60986619, 0xbd8e, 0x49f9, 0xa9, 0x79, 0xb0, 0xa, 0x95, 0x8b, 0x57, 0xea);

	interface IDsTrack: public IDsClip
	{
	public:
		// �õ�������ͣ���Ƶ�������Ƶ��� 
		virtual EDsTrackType GetTrackType() = 0;

		// �õ��������
		virtual int GetTrackIndex() = 0;

		// ���ù������
		virtual void SetTrackIndex(int index) = 0;

		// �����ز�
		virtual HRESULT InsertClip(IDsClip *pClip, EDsTrackPlayListIndex eTrackPlayListIndex = eDsTrackPlayListA) = 0;

		// ɾ���ز�
		virtual HRESULT RemoveClip(IDsClip *pClip) = 0;

		// �õ�ָ�����ָ��λ�õ��ز�
		// ��������Ҫ����Release() ����
		virtual IDsClip * FindIndexClip(EDsTrackPlayListIndex eTrackPlayListIndex, uint32_t index) = 0;
		virtual IDsClip * FindPositionClip(EDsTrackPlayListIndex eTrackPlayListIndex, uint64_t framePosition) = 0;

		// �ƶ��ز�
		// pClip         ָ��Ҫ�ƶ����ز�
		// frameDuration ָ���ƶ���ƫ����
		virtual HRESULT MoveClip(IDsClip *pClip, __int64 frameDuration) = 0;


		// �Ƿ���ù��: ֻ�����Ƶ���
		virtual void EnableVideoDisplay(bool bEnable) = 0;

		// ��ӹ����ؼ�
		virtual HRESULT AddTransitionFxEffect(SDsTransitionFxEffect sTransitionFxEffect) = 0;

		// ɾ�������ؼ� 
		virtual HRESULT RemoveTransitionFxEffect(uint64_t framePosition) = 0;

		// ƽ�ƹ����ؼ�
		virtual HRESULT MoveTransitionFxEffect(uint64_t framePosition, int64_t frameDuration) = 0;

	};
	typedef list<IDsTrack*> DsTrackList;


	// {D2658194-8958-4DB9-8A39-E7CA69560BD6}
	DEFINE_GUID(IID_IDsTimeline, 
	0xd2658194, 0x8958, 0x4db9, 0x8a, 0x39, 0xe7, 0xca, 0x69, 0x56, 0xb, 0xd6);

	interface IDsTimeline: public IDsClip
	{
		// Ĭ�ϲ��뵽������
		virtual HRESULT InsertTrack(IDsTrack *pTrack) = 0;

		// ��������ָ��λ��
		virtual HRESULT InsertTrack(int position, IDsTrack *pTrack) = 0;

		// ɾ�����
		virtual HRESULT RemoveTrack(IDsTrack *pTrack) = 0;

		//��ȡ�������
		virtual int GetTrackCount(EDsTrackType eType) = 0;

		//��ȡ���
		virtual HRESULT GetTrack(EDsTrackType eType, int position, IDsTrack **out_ppTrack) = 0;
		// 
		virtual HRESULT SetVideoMaskTrack(IDsTrack *pTrack) = 0;
	};


	// {AF5E51E9-0E6B-47CE-96A1-FCBA21E6C64A}
	DEFINE_GUID(IID_IDsClipFactory, 
	0xaf5e51e9, 0xe6b, 0x47ce, 0x96, 0xa1, 0xfc, 0xba, 0x21, 0xe6, 0xc6, 0x4a);

	interface IDsClipFactory: public IUnknown
	{
		virtual HRESULT CrateFileClip(EFileType eFileType, IDsFileClip **pFileClip) = 0;
		virtual HRESULT CreateTrack(EDsTrackType eTrackType, int index, IDsTrack **pTrack) = 0;
		virtual HRESULT CreateTimeline(IDsTimeline **ppTimeline) = 0;
		virtual HRESULT SetMainTimeline(IDsTimeline *pTimeline) = 0;
	};


	// ��ʾ��Ƶ�Ļص�����
	extern "C" typedef HRESULT (*pPreviewVideoFunc)(BYTE* pBuffer, unsigned long bufferSize, void* lpParam);

    extern "C" typedef HRESULT (*pD3dSurfaceCallback)(LPVOID pD3dSurface);

    enum EDs_EngineStatus
    {
        e_DS_PLAY_DROPFRAME            = 0,
        e_DS_PLAY_END	               = 1,
        e_DS_PLAY_FAILED               = 2,
        e_DS_PLAY_GETFRAME_FAILED      = 3,

        e_DS_COMPILER_SETP			   = 20,  // ���ɵĵ�ǰλ�ã������Ѿ����ɵ�֡��
        e_DS_COMPILER_FINISHED		   = 21,  // �������
        e_DS_COMPILER_FAILED		   = 22,  // ����ʧ��
        e_DS_COMPILER_WRITEFILE_FILED  = 23,  // д�ļ�����: �����Ѿ����ɵ�֡��
        e_DS_COMPILER_FILE_EXIST_ERROR = 24, // Ŀ¼�ļ��Ѿ����ڣ��Ҳ���ɾ��

        e_DS_CAPTURE_GETFRAME_FAILED   = 30,  // �ɼ�ȡ֡ʧ��
        e_DS_CAPTURE_NO_PAPARED_FRAME  = 31,  // û���Ѿ��ɼ��õ�����
        e_DS_CAPTURE_WRITEFILE_FAILED  = 32,  // �ɼ�д�ļ�ʧ��
        e_DS_CAPTURE_STEP			   = 33,  // ��ǰ���ڴ��������֡
        e_DS_CAPTURE_QUALITY_CHANGE    = 34,  // �ɼ���֡
        e_DS_CAPTURE_PACKET_FAILED	   = 35,  // �ɼ�ȡ��ʧ��
        e_DS_CAPTURE_FINISHED	   	   = 36,  // �ɼ���ɣ������ɼ����Զ�ֹͣ��

        e_DS_CODEC_FAILED              = 40  // ���÷���ʦ�ı���ʧ�ܣ�����׼ȷ֪��ԭ����洢�ռ䲻�㣩
    };


	// ������������ص�����
	// iEngineStatus : ����ĵ�ǰ����
	// iOperatorStatus : ����״̬ (δʹ�ã�ȫ������0ֵ��
	extern "C" typedef HRESULT (*pStatusCallBackFunc)(EDs_EngineStatus eEngineStatus, long lOperatorStatus);



	// �ɼ�ģ��ӿڶ���
	// {6CAEF63A-7F72-45F6-AF80-75162CE7D862}
	DEFINE_GUID(IID_IDsComplier, 
		0x6caef63a, 0x7f72, 0x45f6, 0xaf, 0x80, 0x75, 0x16, 0x2c, 0xe7, 0xd8, 0x62);

	interface IDsComplier : public IUnknown
	{
		// ����
		virtual HRESULT DoFastCompile(LPCTSTR pstrFormatInfo, SDsExportFileInfo *pExportFileInfo, int nFiles, int nAFDID) = 0;
		virtual HRESULT Stop() = 0;
		virtual void SetCompilerStatusNotify(pStatusCallBackFunc pCallback) = 0;
		virtual void EnablePreviewWindow(bool bEnable) = 0;
		virtual HRESULT GetSupportEncAFDType(bool bHDFormat, SDsEncAFDList **pEnList) = 0;
	};


	// �ɼ�ģ��ӿ�
	// {C577EC06-EE4A-4F06-A9EF-5923C1B3743B}
	DEFINE_GUID(IID_IDsCapture, 
		0xc577ec06, 0xee4a, 0x4f06, 0xa9, 0xef, 0x59, 0x23, 0xc1, 0xb3, 0x74, 0x3b);
	interface IDsCapture : public IUnknown
	{
		virtual HRESULT InitCapture() = 0;
		// ���֧��������ʽ
		// nFormatCount ���ֵΪ 2 �ֱ�Ϊ�����ʺ͵�����
		virtual HRESULT Preroll(SDsCaptureFileDescription *in_pCaptureFileDesc[], int nFormatCount, int nAFDID) = 0;

		virtual void SetCaptureStatusCallback(pStatusCallBackFunc pCallback) = 0;
		virtual HRESULT SetLiveWindowCallback(pPreviewVideoFunc pCallback, LPVOID lpParam) = 0;
		virtual uint64_t GetPosition() = 0;
		virtual uint64_t GetCurrentSegmentPosition() = 0;

		virtual HRESULT DoCapture() = 0;
		virtual HRESULT StopCapture() = 0; // ֹͣ�ɼ��ļ���Ԥ�����ڲ�ֹͣ
		virtual HRESULT Stop() = 0;	       // ֹͣ�ɼ�


		virtual void SetCaptureVolume(float fLeftVoume, float fRightVolume) = 0;
		virtual HRESULT GetMainVUMeter(SDsAudioVuMeterValueMono & sLeftChannelVuMeter, 
										SDsAudioVuMeterValueMono &sRightChannelVuMeter) = 0;

		virtual HRESULT GetChannelVUMeter(unsigned int uiIndex, SDsAudioVuMeterValueMono &sTrackVuMeter) = 0;

		// �ֶβɼ�
		// ���÷ֶβɼ����ȣ���֡Ϊ��λ
		// frameLength == 0 ��ִ�зֶβɼ�
		virtual void SetSegmentLimitLength(uint64_t frameLenght) = 0; 
		// ���طֶβɼ��ֶγ���
		virtual uint64_t GetSegmentLimitLength() = 0;
		// ���ص�ǰ�ֶβɼ�������
		virtual int GetCaptureSegmentIndex() = 0;

		virtual HRESULT GetCaptureSignalSource(EDsCaptureSourceItem eCaptureSource, SCaptureSignalList **pSourceList) = 0;
		virtual HRESULT SetCaptureSiganlSource(EDsCaptureSourceItem eCaptureSource, SDsCaptureSignalItem &sCaptureSignal) = 0;
		virtual HRESULT GetCurrentCaptureSiganlSource(EDsCaptureSourceItem eCaptureSource, SDsCaptureSignalItem &sCaptureSignal) = 0;

		virtual unsigned int GetCaptureChannelsCount() = 0; 

		virtual HRESULT ControlVideoParameter(EDSVideoParameterControl eControlType, int videoParameterType, SDsFloatItem &sDsFloatItem) = 0;

		// Զ�̿���
		//virtual bool bSupportRemoteControl() = 0;
		//virtual HRESULT RemoteControlSetDeviceMode(EDsRemoteMode eDsRemoteMode) = 0;
		//virtual HRESULT RemoteControlSetShuttleRate(double dbRate) = 0;
		//virtual HRESULT RemoteControlSeekPosition(uint64_t framePosition) = 0;
		//virtual uint64_t RemoteControlGetTimeCode() = 0;

		//virtual HRESULT SetComRemoteControl(bool bComRemoteControl) = 0;
		//virtual HRESULT SetComInfo(LPCTSTR strComAddr, LPCTSTR strRecordType, LPCTSTR strTimecodeType) = 0;

		virtual HRESULT GetSupportEncAFDType(bool bHDFormat, SDsEncAFDList **pEnList) = 0;
	};

	
	// ���λ�༭����ʵʱѡ���track�Ļص�����
	extern "C" typedef int (*pGetCurrentTrackIndexCallback)(uint64_t position);

	// {871B0573-BB0E-4DD1-AA53-8C672C3CD1FB}
	DEFINE_GUID(IID_IDsPlayer, 
		0x871b0573, 0xbb0e, 0x4dd1, 0xaa, 0x53, 0x8c, 0x67, 0x2c, 0x3c, 0xd1, 0xfb);

	// ����ģ��ӿ�
	interface IDsPlayer : public IUnknown
	{
		// ����
		virtual HRESULT Play(uint64_t frameStart, uint64_t frameEnd, bool bLoop) = 0;
		// ��λ
		virtual HRESULT Seek(uint64_t frameSeekPosition) = 0;
		// ��ͣ
		virtual HRESULT Pause() = 0;
		// ֹͣ
		virtual HRESULT Stop() = 0;
		// ���ص�ǰλ��
		virtual uint64_t GetPosition() = 0;
		// ȡ��֡ 
		virtual HRESULT GetSingleFrame(uint64_t framePosition, BYTE *pBuffer, int bufferSize) = 0;


		// ���õڶ���������ȫ����ʾ
		virtual HRESULT SetSecondMonitorDisplay(BOOL bEnable) = 0;


		virtual HRESULT CreateLiveWindow(HANDLE hwnd, RECT &rect) = 0;
		virtual HRESULT DestoryLiveWindow() = 0;
		virtual HRESULT SetLiveWindowCallback(pPreviewVideoFunc pCallback, LPVOID lpParam) = 0;
        virtual HRESULT SetD3DSurfaceCallback(pD3dSurfaceCallback pD3dCallback) = 0;

		virtual HRESULT SetMainVolume(float fLeftVolume, float fRightVolume) = 0;
		virtual HRESULT GetMainVUMeter(SDsAudioVuMeterValueMono & sLeftChannelVuMeter, 
									   SDsAudioVuMeterValueMono &sRightChannelVuMeter) = 0;
		virtual HRESULT GetTrackVUMeter(bool bVideoTrack, int nTrackIndex, 
										SDsAudioVuMeterValueMono & sLeftChannelVuMeter, 
									    SDsAudioVuMeterValueMono &sRightChannelVuMeter) = 0;


		virtual void SetPlayerStatusNotify(pStatusCallBackFunc pCallback) = 0;
		virtual TCHAR * GetLastError() = 0;

		// ���ʲ���
		virtual HRESULT LibrettoStart(const TCHAR *pwszFilePath,bool onff) = 0;     // ��ʼ����ģʽ �� strFilePath Ϊ�����ļ�·��
		virtual HRESULT LibrettoIn(int32_t iLibrettoIndex) = 0;     // iLibrettoIndex Ϊ�������� 
		virtual HRESULT LibrettoOut() = 0;
		virtual HRESULT LibrettoSeek(uint64_t framePosition) = 0;
		virtual HRESULT LibrettorEnd() = 0;  // ���ʱ༭����

		virtual HRESULT editStart() = 0;
		virtual HRESULT editEnd() = 0;

		// ���λ�༭
		virtual HRESULT multiCameraStart(int startTrackIndex, int trackCount) = 0;
		virtual HRESULT multiCameraEnd() = 0;
		virtual HRESULT SetAllTrackSurfaceCallback(pPreviewVideoFunc pCallback) = 0;
		virtual HRESULT SetSelectedTrackIndexCallback(pGetCurrentTrackIndexCallback pCallback) = 0;
	};

	// {9A9CB75F-035F-4CDB-88AA-1CD013CE5ABF}
	//DEFINE_GUID(IID_IDsMotionTracer, 
	//	0x9a9cb75f, 0x35f, 0x4cdb, 0x88, 0xaa, 0x1c, 0xd0, 0x13, 0xce, 0x5a, 0xbf);

	//interface IDsMotionTracer : public IUnknown
	//{
	//	virtual HRESULT SetUpdateCurrentFrameRectCallback(GetCurrentResultRect * pCallback) = 0;
	//	virtual HRESULT StartTracer(RECT startRect, uint64_t frameStartPosition, uint64_t frameEndPosition, long limageWidth, long limageHeight) = 0;
	//	virtual HRESULT StopTracer() = 0;
	//	virtual HRESULT GetTracerResult(std::vector<RECT> & resultRects) = 0;
	//};


	// {C9508688-7C6B-4299-A8E2-A46F685511EC}
	//DEFINE_GUID(IID_IDsTechReview, 
	//	0xc9508688, 0x7c6b, 0x4299, 0xa8, 0xe2, 0xa4, 0x6f, 0x68, 0x55, 0x11, 0xec);
	//// ����Ԥ��ӿ�
	//interface IDsTechReview : public IUnknown
	//{
	//	virtual HRESULT SetTechReviewCallback(LPVOID pv, LPTECCHECKCALLBACK callback) = 0;
	//	virtual HRESULT SetTechReviewParameters(S_DS_TchCheckParams params) = 0;
	//	virtual HRESULT StartTechReview(uint64_t frameStart, uint64_t frameEnd) = 0;
	//	virtual HRESULT StopTechReview() = 0;
	//	virtual uint64_t GetCurrentPosition() = 0;
	//};




	// {895D066D-DC5E-4025-97DB-9CB1D083F1E7}
	DEFINE_GUID(IID_IDsEngineTools, 
		0x895d066d, 0xdc5e, 0x4025, 0x97, 0xdb, 0x9c, 0xb1, 0xd0, 0x83, 0xf1, 0xe7);

	interface IDsEngineTools: public IUnknown
	{
		virtual HRESULT CopyARGBSurface( BYTE *pSrcBuffer, BYTE *pDstBuffer, unsigned long ulRowBits, unsigned long ulHeight) = 0;
		virtual HRESULT GetLastError(TCHAR **ppError) = 0;
		virtual HRESULT FxSetColorBackGround(IDsFxEffect *pFx, LPVOID pBuffer, DWORD dwPitch, int nWidth, int nHeight) = 0;
		virtual HRESULT GetCrossConverterAFDType(bool bHDFormat, SDsEncAFDList **pEncList) = 0;
	};


	// {83592015-C335-490C-8A12-AD7AB7E39C8E}
	DEFINE_GUID(IID_IDsFxEffectManager, 
	0x83592015, 0xc335, 0x490c, 0x8a, 0x12, 0xad, 0x7a, 0xb7, 0xe3, 0x9c, 0x8e);

	interface IDsFxEffectManager : public IUnknown
	{
		virtual HRESULT CreateEffect(const GUID &guidEffect, IDsFxEffect **ppEffect) = 0;

		// �õ��ؼ��б�
		virtual HRESULT GetSupportFxList(SDsFxInfo arFxArray[], int &count) = 0;

		virtual HRESULT SetFxFileDirectory(LPCWSTR in_pszFileDic) = 0;
		virtual HRESULT GetFxFileDirectory(LPWSTR in_pszFileDict, int *pLength) = 0;
	};

	// {03B9540E-9E11-417A-A217-42DBC1036D67}
	DEFINE_GUID(IID_IEGAudioFxEffectManager, 
		0x3b9540e, 0x9e11, 0x417a, 0xa2, 0x17, 0x42, 0xdb, 0xc1, 0x3, 0x6d, 0x67);
	interface IEGAudioFxEffectManager : public IUnknown
	{
		virtual HRESULT CreateAudioEffect(const GUID &fxGuid, IDsFxEffect **ppEffect) = 0;
		virtual HRESULT GetAudioFxList(SDsFxInfo arFxArray[], int &count) = 0;
	};

	interface IDsTimeline;

	// {FFDFD8BD-C737-4455-B601-FC5E31D612F5}
	DEFINE_GUID(IID_IDsPlayEngine,
		0xffdfd8bd, 0xc737, 0x4455, 0xb6, 0x1, 0xfc, 0x5e, 0x31, 0xd6, 0x12, 0xf5);

	interface IDsPlayEngine : public IUnknown
	{
		// �л���ʱ����
		virtual void UpdateMainTimeline(IDsTimeline *pTimeline) = 0;
		// ��ʱ����ָ��λ�õ�֡������֡�ϳ�
		virtual HRESULT CompositionWithInputFrames(uint64_t framePosition,
			IFxSurface *in_pSurfaces[],
			int in_nSurfaceCount,
			IFxSurface **out_pSurfaces) = 0;
		virtual uint64_t GetPosition() = 0;
	};

	// {36EA4909-D6C0-405B-9423-7E16595D15C8}
	DEFINE_GUID(IID_IDsPlayEngineManager,
		0x36ea4909, 0xd6c0, 0x405b, 0x94, 0x23, 0x7e, 0x16, 0x59, 0x5d, 0x15, 0xc8);

	interface IDsPlayEngineManager : public IUnknown
	{
		virtual HRESULT CreateEngine(DSNleLib::SDsResolutionInfo *in_pResolution) = 0;
		virtual HRESULT DestroyEngine() = 0;

		// ���ز�������
		virtual IDsPlayEngine * GetPlayEngine() = 0;

		// ʱ�����زĹ���
		virtual IDsClipFactory* GetClipFactory() = 0;

		// �ؼ�����ģ��
		virtual IDsFxEffectManager *GetFxEffectManager() = 0;

		virtual HRESULT GetAudioFxEffectManager(IEGAudioFxEffectManager **pAudioFxManager) = 0;

		// �ļ�����ģ��
		virtual IDsImporter *CreateImporter() = 0;

		// ����ģ��
		virtual IDsComplier *GetCompiler() = 0;

		// �ɼ�ģ��
		virtual IDsCapture  *GetCapture() = 0;

		// ����ģ��
		virtual IDsPlayer  *GetPlayer() = 0;

		virtual IDsEngineTools *GetEngineTools() = 0;

		virtual HRESULT GetControlInterface(REFIID riid, void **ppv) = 0;
	};

}

extern "C"  HRESULT GetPlayEngineManager(PlayEngine::IDsPlayEngineManager **ppEngine);
