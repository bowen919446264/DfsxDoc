#ifndef __DSPUBLICAPI_H__
#define __DSPUBLICAPI_H__

#pragma once

#include "DsNleDef.h"
#include "DsIoDef.h"
#include "DsCodecDef.h"
#include "IFxSurface.h"

namespace DSNleLib {
interface IDsAvContent;
interface IDsSurface;
interface IDsAudioSamples;
interface IDsPlayListElement;
interface IDsPlayListCreator;
interface IDsPlayList;
interface IDsLiveReader;
interface IDsAvReader;
interface IDsSurfaceCompletionObserver;
interface IDsAudioSamplesCompletionObserver;
interface IDsNleEngine;
interface IDsSurfacePoolManager;
interface IDsAudioSamplesPoolManager;
interface IDsCodecMgr;
interface IDsInputOutput;
interface IDsAudioMixer;
interface IDsCmd;
// {FBA752FB-735D-411C-8841-E94BABFC3689}
DEFINE_GUID(IID_IDsRawClock, 
0xfba752fb, 0x735d, 0x411c, 0x88, 0x41, 0xe9, 0x4b, 0xab, 0xfc, 0x36, 0x88);
interface IDsRawClock :public IUnknown
{
	virtual int64_t __stdcall GetTime() = 0;
};

DEFINE_GUID(IID_IDsClock, 
0xfba752fb, 0x735d, 0x411c, 0x88, 0x41, 0xe9, 0x4b, 0xab, 0xfc, 0x36, 0x89);

interface IDsClock :public IUnknown
{
	virtual HRESULT __stdcall GetTime(int64_t *pTime) = 0;
    /* Provide standard mechanisms for scheduling events */

    /* Ask for an async notification that a time has elapsed */
    virtual HRESULT __stdcall AdviseTime(
        int64_t lRefTime,        // base reference time
        HANDLE hEvent                  // advise via this event
    )=0;
    /* Ask for an asynchronous periodic notification that a time has elapsed */
    virtual HRESULT __stdcall AdvisePeriodic(
        int64_t StartTime,       // starting at this time
        int64_t PeriodTime,      // time between notifications
        HANDLE hSemaphore          // advise via a semaphore
    )=0;
    /* Cancel a request for notification(s) - if the notification was
     * a one shot timer then this function doesn't need to be called
     * as the advise is automatically cancelled, however it does no
     * harm to explicitly cancel a one-shot advise.  It is REQUIRED that
     * clients call Unadvise to clear a Periodic advise setting.
     */

    virtual HRESULT __stdcall Unadvise(HANDLE hEvt)=0;

	virtual HRESULT __stdcall SetRawClock(IDsRawClock *in_iclock)=0;
};
//////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Provides the user application with the methods to gain access to all of the modules and components.
//////////////////////////////////////////////////////////////////////////////////
// {070AF5E6-BA93-4d02-9670-05D7FBD218C8}
DEFINE_GUID(IID_IDsNleEngine, 
0x70af5e6, 0xba93, 0x4d02, 0x96, 0x70, 0x5, 0xd7, 0xfb, 0xd2, 0x18, 0xc8);

interface IDsNleEngine : public IUnknown
{
	//
	// Summary:
	//    Instantiates a  File Reader object and returns the IDsReader interface.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_INVALID_POINTER, the input pointer is NULL.
	//    - DS_E_FLEXREADER_CREATION_FAILED.
	//    - DS_E_OUT_OF_MEMORY.
	// Remarks:
	//    - An AddRef call is done on the interface. The  Engine does not keep a copy of the
	//      interface.
	//    - Multiple calls to this function can be made to instantiate multiple  File Reader objects. 
	//      The number of  File Reader objects that can be instantiated is not explicitly limited.
	virtual HRESULT __stdcall CreateAvReader
		(
		IDsAvReader ** out_ppIAvReader  // IAvReader interface pointer.
		) = 0;
	//
	// Summary:
	//    Retrieves the IDsPlayListCreator interface of the playlist creator.
	// Return value:
	//    - DS_NOERROR, if completed successfully. 
	//    - DS_E_DEVICE_NOT_PRESENT, if there's no playlist creator module controlled by the Engine. 
	// Remarks:
	//    - Calling this method increases the internal reference count on the IMvPlayListCreator
	//      interface. 
	virtual HRESULT __stdcall GetPlayListCreatorInterface
		(
		IDsPlayListCreator ** out_ppICreator   // Pointer that is initialized with the IMvPlayListCreator
		// interface pointer.
		) = 0;  
	// Summary:
	//    Allocates a surface based on the specified surface description and returns the IDsSurface
	//    interface.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_INVALID_PARAMETER, if in_psDescription or out_ppISurfaceare are NULL. 
	//    - DS_E_UNSUPPORTED, if the requested surface description is not supported and can't be 
	//      allocated.
	//    - DS_E_OUT_OF_MEMORY, if no more memory is available.
	//
	virtual HRESULT __stdcall CreateSurface
		(
		SDsSurfaceDescription * in_psDescription, // Pointer to the description of the surface 
		// to allocate. 
		IDsSurface                 ** out_ppISurface    // Pointer that receives the allocated 
		// IDsSurface interface pointer.
		) = 0; 
	//
	// Summary:
	//    Allocates an audio sample buffer based on the specified audio sample description and returns
	//    the IDsAudioSamples interface.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_INVALID_PARAMETER, if in_psDescription or out_ppIAudioSamplesare NULL. 
	//    - DS_E_UNSUPPORTED, if the requested audio samples description is not supported and cannot
	//      be allocated.
	//    - DS_E_OUT_OF_MEMORY, if no more memory is available.
	virtual HRESULT __stdcall CreateAudioSamples
		(
		SDsAudioSamplesDescription * in_psDescription,     // Pointer to the description of the
		// audio samples to allocate. 
		IDsAudioSamples                 ** out_ppIAudioSamples   // Pointer that receives the 
		// allocated IDsAudioSamples
		// interface pointer.
		) = 0;

	//
	// Summary:
	//    Creates an Surface Pool Manager.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_INVALID_POINTER, if the pointer is NULL
	//    - DS_E_OUT_OF_MEMORY, if the creation failed. 
	virtual HRESULT __stdcall CreateSurfacePoolManager
		(
		IDsSurfaceCompletionObserver * in_pISurfaceCompletionObserver, //  

		TCHAR in_wszPoolName[],  // String that contains the user-friendly name of the pool.  This 
		// is used when logging is enabled to help differentiate the many 
		// pools that can be running at the same time.
		// (for example, L"InputSurfacePool").
		SDsSurfaceDescription *  in_pSurfaceDesc,
		IDsSurfacePoolManager ** out_ppISurfacePoolManager,   // Pointer that will receive an A/V
		int	iMaxNbCount = 0, //@ 0 means no limit
		BOOL bPreAlloc=FALSE // @ 
		// Content Pool Manager object.
		) = 0;

	virtual HRESULT __stdcall CreateAudioSamplesPoolManager
		(
		IDsAudioSamplesCompletionObserver * in_pIAudioSamplesCompletionObserver, //  

		TCHAR in_wszPoolName[],  // String that contains the user-friendly name of the pool.  This 
		// is used when logging is enabled to help differentiate the many 
		// pools that can be running at the same time.
		// (for example, L"InputSurfacePool").
		SDsAudioSamplesDescription *  in_pSurfaceDesc,
		IDsAudioSamplesPoolManager ** out_ppIAudioSamplesPoolManager,   // Pointer that will receive an A/V
		int	iMaxNbCount = 0, //@ 0 means no limit
		BOOL bPreAlloc=FALSE // @ 
		// Content Pool Manager object.
		) = 0;

	virtual HRESULT __stdcall GetResolution(SDsResolutionInfo *io_resInfo) = 0;
	virtual HRESULT __stdcall CreateCodecMgr(IDsCodecMgr **ppCodecMgr) =0;
	virtual HRESULT __stdcall GetClock(IDsClock **ppClock)=0;
	virtual HRESULT __stdcall GetIOInterface(IDsInputOutput **ppIoInterface) =0;

//////////////////////////////////////////////////////////////////////////////////
	virtual HRESULT __stdcall CopySurface(IDsSurface *in_pSrc,RECT *in_prc0,
										  IDsSurface *in_pDst,RECT *in_prc1) =0;		

	virtual HRESULT __stdcall CopyAudioSamples(IDsAudioSamples *in_pSrc,IDsAudioSamples *in_pDst) =0;		
	//
	// Summary:
	//    Converts two mono audio samples to one stereo audio sample.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_INVALID_PARAMETER, if one or more parameters are invalid. 
	virtual HRESULT __stdcall CopyAudioSamplesMonoToStereo
		(
		IDsAudioSamples  *in_pIAudioSamplesSourceLeft,  // Pointer to the Left source (mono) audio samples.
		IDsAudioSamples  *in_pIAudioSamplesSourceRight, // Pointer to the Right source (mono) audio samples.
		IDsAudioSamples  *in_pIAudioSamplesDestination, // Pointer to the destination (stereo) buffer.
		unsigned long     in_ulThreadPoolIndex          // Index of the thread pool to use.
		) = 0;

	//
	// Summary:
	//    Converts a stereo audio sample to two mono audio samples.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_INVALID_PARAMETER, if one or more parameters are invalid.
	virtual HRESULT __stdcall CopyAudioSamplesStereoToMono
		(
		IDsAudioSamples  *in_pIAudioSamplesSource,            // Pointer to the source (stereo) audio samples.
		IDsAudioSamples  *in_pIAudioSamplesDestinationLeft,   // Pointer to left destination (mono) audio samples.
		IDsAudioSamples  *in_pIAudioSamplesDestinationRight,  // Pointer to right destination (mono) audio samples.
		unsigned long     in_ulThreadPoolIndex                // Index of the thread pool to use. 
		) = 0;

	//
	// Summary:
	//    Converts 4, 8, or 16 mono audio samples to one multi-track audio sample containing 4, 8,
	//    or 16 tracks.
	// Return value:
	//    - DS_NOERROR, if completed successfully. 
	//    - DS_E_INVALID_POINTER, if in_ppISrcBufferArray, or in_pIDestBuffer = NULL. 
	//    - DS_E_BUFFER_FORMAT_MISMATCH, if the channel type of the destination buffer does not match
	//      in_kulSrcArraySize.
	//    - DS_E_INVALID_PARAMETER, if in_kulSrcArraySize < 2. 
	//    - DS_E_INVALID_PARAMETER, if in_kulSrcArraySize does not match in_kulVolumeArraySize. 
	//    - DS_E_INVALID_PARAMETER, if in_psVolumeArray = NULL and in_kulVolumeArraySize > 0. 
	// Remarks:
	//    - If in_psVolumeArray = NULL, no volumes are applied in the destination buffer.
	//    - This method consumes the auto-fades specified in the source audio samples.
	//    - Auto-fades specified in the destination buffer are ignored.
	virtual HRESULT _stdcall CopyAudioSamplesMonoToMux
		(
		IDsAudioSamples       ** in_ppISrcBufferArray,  // Pointer to the source array of mono audio
		// samples 
		const unsigned long      in_kulSrcArraySize,    // Number of mono sources contained in the 
		// specified source array.
		IDsAudioSamples        * in_pIDestBuffer,       // Pointer to the destination multi-track 
		// audio sample.
		SDsAudioMonoVolumeInfo * in_psVolumeArray,      // Pointer to an array of SmvAudioMonoVolumeInfo
		// structures containing volume settings for
		// all mono audio samples in the specified
		// source array.
		const unsigned long      in_kulVolumeArraySize, // Number of SmvAudioMonoVolumeInfo structures
		// in volume array.
		unsigned long            in_ulThreadPoolIndex   // Index of the thread pool to use.
		) = 0;

	//
	// Summary:
	//    Converts 2, 4, or 8 stereo audio samples to one multi-track audio sample containing 4, 8,
	//    or 16 tracks.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_INVALID_POINTER, if in_ppISrcBufferArray, or in_pIDestBuffer = NULL
	//    - DS_E_BUFFER_FORMAT_MISMATCH, if the channel type of the destination buffer does not match 
	//      in_kulSrcArraySize * 2.
	//    - DS_E_INVALID_PARAMETER, if in_kulSrcArraySize < 2.
	//    - DS_E_INVALID_PARAMETER, if kulVolumeArraySize does not match in_kulSrcArraySize * 2.
	//    - DS_E_INVALID_PARAMETER, if in_psVolumeArray = NULL while in_kulVolumeArraySize > 0.
	// Remarks:
	//    - If you need to apply volume on the sources, you must set two SmvAudioMonoVolumeInfo
	//      structures for each stereo entry.
	//    - If in_psVolumeArray = NULL, no volumes are applied in the destination audio sample.
	//    - This method consumes the auto fades specified in the source audio sample.
	//    - Auto-fades specified in the destination buffers are ignored.
	virtual HRESULT _stdcall CopyAudioSamplesStereoToMux
		(
		IDsAudioSamples       ** in_ppISrcBufferArray,  // Pointer to the source array of stereo audio 
		// samples 
		const unsigned long      in_kulSrcArraySize,    // Number of stereo sources contained in the 
		// specified source array.
		IDsAudioSamples        * in_pIDestBuffer,       // Pointer to the destination multi-track 
		// audio sample.
		SDsAudioMonoVolumeInfo * in_psVolumeArray,      // Pointer to an array of SmvAudioMonoVolumeInfo
		// structures containing volume settings for
		// all audio samples in the specified source
		// array. The first SmvAudioMonoVolumeInfo
		// structure in the array specifies a left
		// channel whereas the second entry specifies 
		// a right channel.
		const unsigned long      in_kulVolumeArraySize, // Number of SmvAudioMonoVolumeInfo structures 
		// in volume array.
		unsigned long            in_ulThreadPoolIndex   // Index of the thread pool to use.
		) = 0;

	//
	// Summary:
	//    Converts one multi-track audio sample to 4, 8, or 16 mono audio samples.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_INVALID_POINTER, if in_pISrcBuffer, or in_ppIDestBufferArray = NULL
	//    - DS_E_BUFFER_FORMAT_MISMATCH, if the channel type of the source audio sample does not
	//      match in_kulDestArraySize.
	//    - DS_E_UNSUPPORTED_DATA_TYPE, if the data type of the source audio sample is not PCM data.
	//    - DS_E_INVALID_PARAMETER, if the channel type of the source audio sample does not match 
	//      in_kulVolumeArraySize.
	//    - DS_E_INVALID_PARAMETER, if in_psVolumeArray = NULL or in_kulVolumeArraySize = 0.
	// Remarks:
	//    - This method will consume the auto-fades specified in the source audio sample.
	//    - Auto-fades specified in the destination audio sample are ignored.
	virtual HRESULT _stdcall CopyAudioSamplesMuxToMono
		(
		IDsAudioSamples        * in_pISrcBufferArray,   // Pointer to the source multi-track audio 
		// sample.
		IDsAudioSamples       ** in_ppIDestBuffer,      // Pointer to the destination array of mono
		// audio samples.
		unsigned long            in_kulDestArraySize,   // Number of mono audio sample destinations 
		// contained in the specified destination 
		// array.
		SDsAudioMonoVolumeInfo * in_psVolumeArray,      // Pointer to an array of SmvAudioMonoVolumeInfo
		// structures containing volume settings
		// for all mono audio samples in the
		// specified destination array.
		const unsigned long      in_kulVolumeArraySize, // Number of SmvAudioMonoVolumeInfo structures
		// in volume array.
		unsigned long            in_ulThreadPoolIndex   // Index of the thread pool to use.
		) = 0;

	//
	// Summary:
	//    Converts one multi-track audio sample to 2, 4, or 8 stereo audio samples.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_INVALID_POINTER, if in_ppISrcBufferArray, or in_pIDestBuffer = NULL
	//    - DS_E_BUFFER_FORMAT_MISMATCH, if the channel type of the source audio sample does not 
	//      match in_kulDestArraySize / 2.
	//    - DS_E_OUTPUT_FORMAT_NOT_SUPPORTED, if the channel type of one destination is not stereo.
	//    - DS_E_INVALID_PARAMETER, if in_kulDestArraySize does not match in_kulVolumeArraySize * 2.
	//    - DS_E_INVALID_PARAMETER, if in_psVolumeArray = NULL while in_kulVolumeArraySize > 0.
	// Remarks:
	//    - If in_psVolumeArray = NULL, no volume are applied in the destination buffer.
	//    - This method consumes the auto-fades specified in the source audio sample.
	//    - Auto-fades specified in the destination buffers are ignored.
	virtual HRESULT _stdcall CopyAudioSamplesMuxToStereo
		(
		IDsAudioSamples        * in_pISrcBufferArray,   // Pointer to the source multi-track audio
		// sample.
		IDsAudioSamples       ** in_ppIDestBuffer,      // Pointer to the destination array of stereo 
		// audio samples.
		unsigned long            in_kulDestArraySize,   // Number of stereo audio sample destinations 
		// contained in the specified destination 
		// array.
		SDsAudioMonoVolumeInfo * in_psVolumeArray,      // Pointer to an array of SmvAudioMonoVolumeInfo
		// structures containing volume settings
		// for all audio samples in the specified 
		// destination array. The first 
		// SmvAudioMonoVolumeInfo structure in the 
		// array specifies a left channel whereas
		// the second entry specifies a right
		// channel.
		const unsigned long      in_kulVolumeArraySize, // Number of SmvAudioMonoVolumeInfo structures
		// in volume array.
		unsigned long            in_ulThreadPoolIndex   // Index of the thread pool to use.
		) = 0;

	virtual HRESULT __stdcall CreateLiveReader(IDsLiveReader **ppOut) = 0;
	virtual HRESULT __stdcall GetAudioMixer(IDsAudioMixer **ppOut) = 0;

	virtual HRESULT __stdcall ExecuteCmd(IDsCmd *in_pCmd,int ThreadIndex=0) = 0;  
	virtual HRESULT _stdcall ApplyVolume
		(
		IDsAudioSamples        * in_pSrcBuffer,         // Pointer to the source multi-track audio
		// sample.
		IDsAudioSamples        * in_pDestBuffer,        // Pointer to the destination multi-track 
		// audio sample.
		SDsAudioMonoVolumeInfo * in_psVolumeArray,      // Pointer to an array of SmvAudioMonoVolumeInfo
		// structures containing volume settings
		// for each track.
		const unsigned long      in_kulVolumeArraySize, // Number of SmvAudioMonoVolumeInfo structures 
		// in volume array.
		unsigned long            in_ulThreadPoolIndex   // Index of the thread pool to use.
		) = 0;

	//@ 
	virtual HRESULT __stdcall SlowMotionBlend( 
		IDsSurface *in_pSurface1,
		IDsSurface *in_pSurface2,
		IDsSurface *out_pSurface,
		float       in_fFactor,
		float       in_fFactor2,
		bool		in_bInvert
		) = 0;

	virtual HRESULT __stdcall InvertField(
		IDsSurface *in_pSurface,
		IDsSurface *out_pSurface
		) = 0;

	virtual HRESULT __stdcall Composite(
		IDsSurface          *in_pSurfaces[],
		int					in_iSurfaceCount,
		float				*in_fAlphas,
		IDsSurface			*out_pSurface) = 0;
};

//////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Main interface used to control and query the Surface pool manager
//
//////////////////////////////////////////////////////////////////////////
// {1BF91DEE-78F6-4B37-A79B-34378BA4D2AA}
DEFINE_GUID(IID_IDsSurfacePoolManager, 
0x1bf91dee, 0x78f6, 0x4b37, 0xa7, 0x9b, 0x34, 0x37, 0x8b, 0xa4, 0xd2, 0xaa);

interface IDsSurfacePoolManager :public IUnknown
{
	//
	// Summary:
	//    Synchronously retrieves an Surface from the Surface Pool. This call is blocking.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if the pointer is NULL or if it is not a valid pointer to a Surface.
	//    - DS_E_TIME_OUT, if all the Surface is not available.
	// Remarks:
	//    - This method will fail if the pool does not manage any surface, but if it does manage some 
	//      surface, this call will block and wait until an surface become available.

	virtual HRESULT __stdcall GetSurface
		(
		IDsSurface** out_ppISurface,  // Pointer that will receive the Surface.  
		unsigned long dwtimeout = INFINITE	  
		) = 0;	

};

// {1BF91DEE-78F6-4B37-A79B-34378BA4D2AA}
DEFINE_GUID(IID_IDsAudioSamplesPoolManager, 
0x1bf91def, 0x78f6, 0x4b37, 0xa7, 0x9b, 0x34, 0x37, 0x8b, 0xa4, 0xd2, 0xaa);

interface IDsAudioSamplesPoolManager :public IUnknown
{
	//
	// Summary:
	//    Synchronously retrieves an Surface from the Surface Pool. This call is blocking.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if the pointer is NULL or if it is not a valid pointer to a Surface.
	//    - DS_E_TIME_OUT, if all the Surface is not available.
	// Remarks:
	//    - This method will fail if the pool does not manage any surface, but if it does manage some 
	//      surface, this call will block and wait until an surface become available.

	virtual HRESULT __stdcall GetAudioSamples
		(
		IDsAudioSamples** out_ppIAudioSamples,  // Pointer that will receive the Surface.  
		unsigned long dwtimeout = INFINITE	  
		) = 0;	

};

//////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Provides the methods necessary to perform synchronized operations on audio 
//    sample buffers and video surfaces.
// Remarks:
//    This interface can be obtained with the QueryInterface() function on an IDsSurface or 
//    IDsAudioSamples interface.  
//////////////////////////////////////////////////////////////////////////////////
// {F39C31D9-98F7-40a8-8A7C-8FC444DB91F9}
DEFINE_GUID(IID_IDsAvContent, 
			0xf39c31d9, 0x98f7, 0x40a8, 0x8a, 0x7c, 0x8f, 0xc4, 0x44, 0xdb, 0x91, 0xf9);

interface IDsAvContent :public IUnknown
{
   //
   // Summary:
   //    Resets a surface.
   // Description:
   //    The followings steps are executed: 
   //    - Removes all attached custom data objects.
   //    - Releases surface alias references (if any) and invalidates the user buffer description.
   //    - Resets synchronization.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_FAIL, if the surface can't be reset. 
   //    - DS_E_AVCONTENT_IN_USE, if the read count and/or the write count is not zero.
   virtual HRESULT __stdcall Flush ( ) = 0;
   //
   // Summary:
   //    Sets the synchronization of the surface to a default state.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_FAIL, if it was unable to reset the synchronization. 
   //    - DS_E_AVCONTENT_IN_USE, if the read count and/or the write count is not zero.
   virtual HRESULT __stdcall ResetSynchronization ( ) = 0;

	virtual DWORD __stdcall WaitForReadCompletion(DWORD dwWait = INFINITE) = 0;
	/**
	 * @summary	wait for write completion of this surface
	 * @param	dwWait - how long this wait operation will last.
	 * @return	HRESULT code
	 */
	virtual DWORD __stdcall WaitForWriteCompletion(DWORD dwWait = INFINITE) = 0;

   //
   // Summary:
   //    Retrieves the event that is going to be signaled when the buffer has been read by all the
   //    units that were called for reading.
   // Remarks:
   //    - The caller must make sure that there is no thread waiting on the event before releasing
   //      the surface. 
virtual HANDLE __stdcall GetReadCompletionEvent() = 0;
   //
   // Summary:
   //    Retrieves the event to signal when all the units that were called for writing are 
   //    finished writing to the buffer
   // Remarks:
   //    - The caller must make sure that there is no thread waiting on the event before releasing
   //      the surface. 
virtual HANDLE __stdcall GetWriteCompletionEvent() = 0;
   //
   // Summary:
   //    Increments the number of surface users that will be called for a read operation by
   //    a value of one.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_FAIL, if the count could not be incremented. 
   virtual void __stdcall IncrementReadCount( ) = 0;
   //
   // Summary:
   //    Increments the number of surface users that will be called for a write operation by
   //    a value of one.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_FAIL, if the count could not be incremented. 
   // Remarks:
   //    - This method will fail if used with a ReadOnly Alias.
   virtual void __stdcall IncrementWriteCount ( ) = 0;
   //
   // Summary:
   //    Retrieves the number of surface users that will be called for a read operation.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_INVALID_PARAMETER, if the parameter is NULL. 
   virtual LONG __stdcall GetReadCount() = 0;
   //
   // Summary:
   //    Retrieves the number surface users that will be called for a write operation.
   // Return value:
   //    - DS_NOERROR, if completed successfully.
   //    - DS_E_INVALID_PARAMETER, if the parameter is NULL.
 	virtual LONG __stdcall GetWriteCount() = 0;
  //
   // Summary:
   //    Signals that the read operation on a buffer object has completed. It decrements the read 
   //    count by one.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_FAIL, if it was unable to signal the read operation. 
   virtual void __stdcall SignalReadCompletion  ( ) = 0;

   //
   // Summary:
   //    Signals the completion of a write operation that is performed by a user mode component.
   // Return value:
   //    - DS_NOERROR, if completed successfully.  
   //    - DS_E_FAIL, if unable to signal the write operation.  
   // Remarks:
   //    - This will decrement the write count by one. When the write count reaches zero, the write
   //      completion event is signaled.
   virtual void __stdcall SignalWriteCompletion ( ) = 0;
   //
   //
   // Summary:
   //    Sets the surface presentation time in nanotime, where 1 nanotime unit is equal to 100 
   //    nanoseconds. 
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   // Remarks:
   //    - The time information is used by some  components, such as the Input, Output, and 
   //      Live Window Manager to schedule operations.
   virtual HRESULT __stdcall SetTimeStampInfo
      (  
      int64_t * in_pui64Time // Pointer to the specified system time of the surface in nanotime.
      ) = 0;
   //
   // Summary:
   //    Retrieves the time stamp information associated with the surface. 
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_INVALID_PARAMETER, if the parameter is NULL. 
   // Remarks:
   //    - It usually represents a presentation time. The time stamp is represented in nanotime,
   //      where 1 nanotime unit is 100 nanoseconds.
   virtual int64_t __stdcall GetTimeStampInfo ()=0;
   //
   // Summary:
   //    Sets the error code associated with the surface.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_FAIL, if the error code could not be set. 
   virtual HRESULT __stdcall SetLastError
      (  
      HRESULT in_hrResult  // Specifies the error code to associate with the surface.
      ) = 0;
   //
   // Summary:
   //    Retrieves the error code associated with the surface.
   // Remarks:
   //    - A unit using a surface can signal the surface with an error code. Only the first error
   //      that occurred is retained and propagated. The synchronization object is signaled to 
   //      allow fast error catching. 
   //    - This method gives access to the error reporting mechanism in a surface.
   virtual HRESULT __stdcall GetLastError() = 0;
   //
   // Summary:
   //    Marks the writing process start time on the AVContent object (and its attached surface).    
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   virtual HRESULT __stdcall MarkWriteStartTime ( ) = 0;

   //
   // Summary:
   //    Stops the timer that is used to compute the writing process timing on the 
   //    AVContent object (and its attached surface). 
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   // Remarks:
   //    - The method GetElapsedTime() can be used to retrieve the elapsed time between the start
   //      and stop times.
   virtual HRESULT __stdcall MarkWriteStopTime ( ) = 0;
   //
   // Summary:
   //    Retrieves the elapsed time (in micro seconds) of the AVContent object's last write process to its 
   //    attached surface.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_MARKWRITE_ERROR, if the MarkWriteStartTime() and/or MarkWriteStopTime() methods 
   //      weren't called properly. 
   // Remarks:
   //    - To obtain a valid elapsed time, the processing modules must use the MarkWriteStartTime()
   //      and MarkWriteStopTime() methods. The elapsed time returned is the difference between the 
   //      first start time and the last stop time. 
   //    - The start and stop times are reset when the Flush() or ResetSynchronization() methods 
   //      are called. 
   virtual uint64_t __stdcall GetElapsedTime() = 0;
};

// {E2EB0682-9F9C-475a-A169-F40637A9D60C}
DEFINE_GUID(IID_IDsSurface, 
			0xe2eb0682, 0x9f9c, 0x475a, 0xa1, 0x69, 0xf4, 0x6, 0x37, 0xa9, 0xd6, 0xc);

interface IDsSurface : public IDsAvContent
{
   //
   // Summary:
   //    Retrieves the information about the surface.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_INVALID_STRUCTURE_SIZE, if the field Size is invalid. 
   // Remarks:
   //    - Field Size of the structure must be filled before calling the method. 
   virtual HRESULT __stdcall GetSurfaceDescription
      (
      SDsSurfaceDescription * io_psDescription  // Pointer to the SDsSurfaceDescription structure
                                                   // that will be filled with the surface
                                                   // description. 
      ) = 0;
   //
   // Summary:
   //    Changes a surface description without reallocating the associated memory.
   // Description:
   //    This method provides a way to reuse a surface for other needs without needing to allocate
   //    a new one. Apart from the allocated size and memory location, this method completely 
   //    ignores the former surface description, so it must be called before any other operation 
   //    on the surface (before a ChangePolarity call for instance). 
   //    The only restrictions are:
   //    - The new surface description needs to be in the same memory location.
   //    - This method will return an error if called on an alias.
   //    - The new surface description must describe a buffer that is lower than, or equal to the 
   //      size of the old buffer. For example, a 32x32 surface cannot be changed to accomodate a
   //      64x64 surface. But, a 64x64 surface can be changed to accomodate a 128x16 surface.
   //    - Only surfaces originally allocated in host memory or in the IO board memory, as well as
   //      user buffers can have their description changed. 
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_NOT_SUPPORTED, if any of the values in the surface description are not supported or
   //      if the method is called on an alias.
   //    - DS_E_UNSUPPORTED_MEMORY_LOCATION, if the surface description is not 
   //      keDsMemoryLocationHost or keDsMemoryLocationIOBoard or if the new memory location is not
   //      in the same location as the originally allocated one. 
   //    - DS_E_BUFFER_SIZE_MISMATCH, if the new surface description would result in a surface size
   //      bigger than the size allocated at creation time. 
   //    - DS_E_INVALID_STRUCTURE_SIZE, if the field Size is invalid. 
   // Remarks:
   //    - Field Size of the structure must be set before calling the method. 
   virtual HRESULT __stdcall ChangeSurfaceDescription
      (
      SDsSurfaceDescription * in_psDescription  // 
      ) = 0;

   //
   // Summary:
   //    Gets the surface memory pointer.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_INVALID_PARAMETER, if the mipmap level or the face type is invalid. 
   //    - DS_E_INVALID_STRUCTURE_SIZE, if the field size of the structure does not contain the
   //      right value. 
   // Remarks:
   //    - After retrieving a surface memory pointer, you can access the surface memory until a 
   //      corresponding Unlock() method is called. When the surface is unlocked, the pointer to 
   //      the surface memory is invalid. 
   //    - Field Size of the structure must be filled before calling the method. 
   virtual HRESULT __stdcall Lock
      (
      SDsLockSurfaceDescription * out_pDesc           // Pointer to the SDsLockSurfaceDescription 
                                                         // structure that will be filled with the 
                                                         // lock surface description.
      ) = 0;

   //
   // Summary:
   //    Notifies that the direct surface memory manipulations are completed.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_INVALID_PARAMETER, if the mipmap level or the face type is invalid. 
   virtual void __stdcall Unlock
      ( 
      ) = 0;

   //
   // Summary:
   //    Assigns the reference surface to a surface alias.
   // Description:
   //    The surface alias inherits all the properties of the reference surface, except for the 
   //    specified resolution scan mode. The memory buffer contained in the reference surface is
   //    also shared with the surface alias. This method can only be called on alias surfaces.
   // Return value:
   //    - DS_NOERROR, if completed successfully.  
   //    - DS_E_INVALID_PARAMETER, if one of the input parameter is invalid.  
   //    - DS_E_NOT_SUPPORTED, if the surface is not a surface alias or if the reference surface
   //      specified is an alias surface.  
   // Remarks:
   //    - The alias and the reference surfaces have separate synchronization objects that are 
   //      linked internally. Each time a component increments the synchronization read or write
   //      count of the alias, the read count of the reference surface is automatically incremented.
   //      Each SignalReadCompletion operation performed on the alias automatically calls the
   //      SignalReadCompletion() method on the reference. Also, each SignalWriteCompletion 
   //      operation performed on the alias automatically calls the SignalWriteCompletion() method
   //      on the reference.  
   //    - If the alias access type is read-only, the write completion on the alias event will be
   //      signaled automatically when the write completion event of the reference is signalled.  
   //    - It is possible to change the alias reference by calling this method more than once.
   //      However, the alias must be flushed before assigning a new reference.  
   //    - The reference surface must be usable at the assignment. Therefore, an empty user buffer
   //      or an unset alias can not be assigned as a reference of an alias. However, a user buffer
   //      that has been set with AssignUserBufferReference can be assigned as a reference of an
   //      alias, even if the data pointer is not determined yet. In this case, the data pointer 
   //      must be properly set with another AssignUserBufferReference() method call before doing
   //      any read operation on the reference or on its aliases.  
   //    - It is possible to assign an alias on an alias, as long as the reference alias has
   //      valid reference attached to it.  
   virtual HRESULT __stdcall GetAliasReference
      ( 
      EDsAliasAccessType in_eAccessType,        // Access type required for the alias. If the alias
                                                   // is not intended to be written to any modules,
                                                   // the access type 'ReadOnly' must be used.
                                                   // Otherwise, 'ReadWrite' access type should be used.
      EDsPolarity        in_ePolarity, // // Specifies the new polarity. 
      IDsSurface       ** out_ppIReferenceSurface // Address of an IDsSurface interface that is the 
      ) = 0;
   // Summary:
   //    Retrieves the access type of the alias.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_INVALID_PARAMETER, if the parameter is NULL. 
   // Remarks:
   //    - If it is not an alias, the alias access type returned is keDsAliasAccessTypeInvalid.
   virtual HRESULT __stdcall GetAliasType
      (
      EDsAliasAccessType * out_peAliasType   // Pointer that receives the alias access type.
      ) = 0;
   //
   // Summary:
   //    Clears the surface with the specified color and alpha values.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_NOT_SUPPORTED, if the clear operation cannot be performed on this type of surface. 
   //    - DS_E_MODULE_BUSY, if a previous Clear() call with the same thread pool ID is not yet 
   //      completed. This limitation is for host and user surface memory location only. 
   //    - DS_E_POINTER_NOT_ALIGNED, if the surface data pointer is not properly aligned. 
   virtual HRESULT __stdcall Clear
      (
      EDsColor      in_eColor,            // Pre-defined color value used to clear the surface.
      float         in_fAlphaValue       // Alpha value used to clear the alpha component of the
                                             // surface. Used only when the surface has an alpha 
                                             // channel. The valid range for Alpha value is [0, 1] 
                                             // inclusive, where 0 is transparent and 1 is opaque.
      ) = 0;

   virtual HRESULT __stdcall ChangePosition
      (
      SDsPointF * in_psDestinationPosition   // Pointer to the SDsPointF structure that specifies
                                             // the new destination position of the surface. 
      ) = 0;
   virtual HRESULT __stdcall ChangePolarity 
      (
      EDsPolarity in_ePolarity   // Specifies the new polarity. 
      ) = 0;

   virtual HRESULT __stdcall ChangeVideoShape(bool bShape) = 0;
};
//////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Allows the management of an audio sample container. The container encapsulates an audio data 
//    buffer and all related information. The IDsAudioSamples interface implements methods from the
//    IMediaBuffer interface, which is defined by Microsoft.
// See also:
//    For details on Microsoft-defined interfaces, see the DirectX 8/9 SDK documentation. 
// Remarks:
//    - This interface supports the following query interfaces: IID_IDsAVContent. 
//
// {E4BB8E92-A552-4ab4-960E-49C5FC2C8032}
DEFINE_GUID(IID_IDsAudioSamples, 
			0xe4bb8e92, 0xa552, 0x4ab4, 0x96, 0xe, 0x49, 0xc5, 0xfc, 0x2c, 0x80, 0x32);


interface IDsAudioSamples : public IDsAvContent
{
   //
   // Summary:
   //    Assigns the reference audio sample container to an audio sample container alias. 
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_INVALID_PARAMETER, if the input parameter is NULL. 
   //    - DS_E_NOT_SUPPORTED, if the surface is not a surface alias (created with CreateSurfaceAlias()). 
   // Remarks:
   //    - The container alias inherits all the properties of the reference container. The memory 
   //      contained in the reference surface is also shared with the surface alias. This method can
   //      be called only on alias audio samples.
   //    - The alias and the reference container have separate synchronization objects that are 
   //      linked internally. Each time a component increments the synchronization read count of the
   //      alias, the read or write count of the reference container is automatically incremented.
   //      Each SignalReadCompletion operation performed on the alias automatically calls the
   //      SignalReadCompletion() method on the reference. Also, each SignalWriteCompletion 
   //      operation performed on the alias automatically calls the SignalWriteCompletion() method
   //      on the reference. 
   //    - If the alias access type is ReadOnly, the write completion on the alias event will be 
   //      signaled automatically when the write completion event of the reference is signaled. 
   //    - It is possible to change the alias reference by calling this method more than once.
   //      However, the alias must be flushed before assigning a new reference. 
   //    - The reference audio samples buffer must be usable at the time of assignment. Therefore,
   //      an empty user buffer or an unset alias cannot be assigned as the reference of an alias.
   //      However, a user buffer that has been set with IDsAudioSamples::AssignUserBufferReferencemethod
   //      can be assigned as a reference of an alias, even if the data pointer is not yet 
   //      determined. In this case, the data pointer must be properly set with another 
   //      AssignUserBufferReference() call before doing any read operation on the reference or on
   //      its aliases. 
   //    - It is possible to assign an alias on an alias, as long as the reference alias has a valid
   //      reference attached to it.  
   virtual HRESULT __stdcall GetAliasReference
      (
      EDsAliasAccessType in_eAccessType,   // Access type required for the alias. If the alias is not 
                                          // intended to be written to any modules, the access type 
                                          // 'ReadOnly' must be used. Otherwise, 'ReadWrite' access 
                                          // type should be used.
	  EDsPolarity in_ePolarity,   // Specifies the new polarity desired.
      IDsAudioSamples  ** out_ppIReferenceAudioSamples  // Address of the IDsAudioSamples interface

      ) = 0;
   
   //
   // Summary:
   //    Provides the ability to change the wave format of the audio samples.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_INVALID_PARAMETER, if the container for the buffer pointer is NULL or the container
   //      for the length is NULL. 
   // Remarks:
   //    - The byte length of the buffer is not changed when calling Ds(). This means that 
   //      when a 16-track, 24 in 32-bit audio sample buffer with a length of 800 samples (51200 bytes) is
   //      changed into an 8-track, 24 in 32-bit audio sample buffer, the length of 51200 bytes stays the
   //      same but processes 1600 samples.
   virtual HRESULT __stdcall ChangeWaveFormat
      (
      SDsaWaveFormatInfo * in_sWaveFormat // Pointer to the SDsaWaveFormatInfo structure that
                                          // specifies the new wave format desired.
      )=0;

   //
   // Summary:
   //    Provides the ability to set the audio samples to zero.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_FAIL, if an internal error occurs. 
   // Remarks:
   //    - This method DOES NOT handle synchronization properly.  The IDsAVContent::SignalWriterCompletion
   //      method is not called at the end of the execution.
   //
   virtual HRESULT __stdcall Clear () = 0;
   
   //
   // Summary:
   //    Retrieves information about the audio sample container/buffer. 
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_INVALID_STRUCTURE_SIZE, if the field size of the structure does not contain the
   //      right value. 
   // Remarks:
   //    - Field Size of the structure must be filled before calling the method. 
   virtual HRESULT __stdcall GetAudioSamplesDescription 
      ( 
      SDsAudioSamplesDescription * io_psDescription   // Pointer to the SDsAudioSamplesDescription 
                                                      // structure that is filled with the audio
                                                      // sample container description. 
      ) = 0;  

   //
   // Summary:
   //    Returns the audio sample buffer pointer and the length of valid data.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_INVALID_PARAMETER, if the container for the buffer pointer is NULL or the
   //      container for the length is NULL. 
   // Remarks:
   //    - The length returned is the length of the valid data for the corresponding pointer. This
   //      value can be zero if there is no valid data in this buffer. 
   virtual HRESULT __stdcall GetBufferAndLength
      (
      void         ** out_ppBuffer, // Pointer to the buffer's pointer. 
      unsigned long * out_pulLength // Pointer to the size of valid data in the buffer.
      ) = 0;

  //
   // Summary:
   //    Sets the length of valid data for the buffer pointer.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   //    - DS_E_INVALID_PARAMETER, if the length is larger than the maximum length. 
   // Remarks:
   //    - The length of valid data must not be larger than the maximum allocated buffer length. 
   virtual HRESULT __stdcall SetLength 
      (
      unsigned long in_ulLength  // Variable to set the length of valid data for the buffer pointer.
      )= 0;
   //
   // Summary:
   //    Sets the fade-in status on the audio sample container.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   // Remarks:
   //    - If the fade-in flag has already been set, then nothing is done and only one fade-in will 
   //      be applied at the time of fade application. 
   virtual HRESULT __stdcall SetFadeIn 
      (
      bool in_bHasFadeIn   // Boolean value that sets the status of fade-in on the audio sample 
                           // container. TRUE, if the fade-in is to be applied. FALSE, otherwise.
      ) = 0;

   //
   // Summary:
   //    Sets the fade-out status on the audio sample container.
   // Return value:
   //    - DS_NOERROR, if completed successfully. 
   // Remarks:
   //    - If the fade-out flag has already been set then nothing is done and only one fade-out will
   //      be applied at the time of fade application. 
   virtual HRESULT __stdcall SetFadeOut
      (
      bool in_bHasFadeOut  // Boolean value that sets the status of the fade-out on the audio 
                           // sample container. TRUE, if the fade-out is to be applied. FALSE,
                           // otherwise.
      ) = 0;

   virtual HRESULT __stdcall SetUserBuffer(LPVOID pBuffer,unsigned long len) = 0;
   virtual HRESULT __stdcall ChangePolarity 
      (
      EDsPolarity in_ePolarity   // Specifies the new polarity. 
      ) = 0;
};
//////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Used by the Surface Pool Manager to notify a 'user' that an Surface has just been 
//    completed.
// Remarks:
//    - This interface must be implemented by the user.
//////////////////////////////////////////////////////////////////////////
// {E6AD5F76-64B4-45c8-944A-E8AD3932BDAE}
DEFINE_GUID(IID_IDsSurfaceCompletionObserver, 
			0xe6ad5f76, 0x64b4, 0x45c8, 0x94, 0x4a, 0xe8, 0xad, 0x39, 0x32, 0xbd, 0xae);

interface IDsSurfaceCompletionObserver : public IUnknown
{
   //
   // Summary:
   //    Allows an external object to examine the Surface upon its completion.
   // Return value:
   //    - Should not return any errors. 
   // Remarks:
   //    - The external module may keep the Surface by calling AddReference() on it, and then process
   //      and release it asynchronously. 
   //    - This function may be called simultaneously by many threads. The external module MUST be
   //      prepared to handle this case in its implementation. 
   virtual HRESULT __stdcall OnSurfaceCompletion
      (
      IDsSurface * in_pISurface // Pointer to the Surface that was just completed.    
      ) = 0;
};

// {E6AD5F77-64B4-45c8-944A-E8AD3932BDAE}
DEFINE_GUID(IID_IDsAudioSamplesCompletionObserver, 
			0xe6ad5f77, 0x64b4, 0x45c8, 0x94, 0x4a, 0xe8, 0xad, 0x39, 0x32, 0xbd, 0xae);

interface IDsAudioSamplesCompletionObserver : public IUnknown
{
   //
   // Summary:
   //    Allows an external object to examine the Surface upon its completion.
   // Return value:
   //    - Should not return any errors. 
   // Remarks:
   //    - The external module may keep the Surface by calling AddReference() on it, and then process
   //      and release it asynchronously. 
   //    - This function may be called simultaneously by many threads. The external module MUST be
   //      prepared to handle this case in its implementation. 
   virtual HRESULT __stdcall OnAudioSamplesCompletion
      (
      IDsAudioSamples * in_pIAudioSamples // Pointer to the Surface that was just completed.    
      ) = 0;
};

// {BD91EDBF-29CC-4565-A6BF-D137C042BD2F}
DEFINE_GUID(IID_IDsCmd, 
0xbd91edbf, 0x29cc, 0x4565, 0xa6, 0xbf, 0xd1, 0x37, 0xc0, 0x42, 0xbd, 0x2f);

interface IDsCmd :public IUnknown
{
public:
	
	virtual void __stdcall Prepare(IDsNleEngine *) = 0;
	virtual void __stdcall Execute(IDsNleEngine *) = 0;
	virtual void __stdcall Recyle(IDsNleEngine *) = 0;
};
#ifdef WIN32
interface __declspec (uuid("BD91EDBF-29CC-4565-A6BF-D137C042BD2F")) IDsCmd;
#endif

};
#ifdef WIN32
extern "C" __declspec (dllexport) HRESULT  CreateEngine(DSNleLib::SDsResolutionInfo *pInfo, DSNleLib::IDsNleEngine **ppOut);
#else
extern "C" HRESULT  CreateEngine(DSNleLib::SDsResolutionInfo *pInfo, DSNleLib::IDsNleEngine **ppOut);
#endif // WIN32






#endif //__DSPUBLICAPI_H__