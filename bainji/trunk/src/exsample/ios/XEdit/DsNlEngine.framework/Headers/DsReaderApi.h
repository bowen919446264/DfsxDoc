#pragma once
#include "DsNleDef.h"
#include "DsUtil.h"
#include "DsPublicApi.h"
namespace DSNleLib {
//////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    This interface is used to control the IDsAvReader playlist reader module.
//
//////////////////////////////////////////////////////////////////////////////////
// {5451FE56-7BA9-4e71-BCB0-85F61A0FE5E8}
DEFINE_GUID(IID_IDsAvReader, 
		0x5451fe56, 0x7ba9, 0x4e71, 0xbc, 0xb0, 0x85, 0xf6, 0x1a, 0xf, 0xe5, 0xe8);

interface IDsAvReader : public IUnknown
{
	virtual HRESULT __stdcall GetVideoBuffer
		(
		IDsSurface*       out_apIVideoSurfaces[],       // IDsSurface* video surfaces array that
		// receives any of the video surfaces
		//  corresponding to 
		// out_pdPositionInsidePlayListInFrames.
		unsigned long*    io_pulVideoSurfacesArraySize, // Video surfaces array size specifying how many 
		// video surfaces the caller expects [in], 
		// and how many video surfaces the  

		double*           out_pdPositionInsidePlayListInFrames,  // Position inside the cutlist, in 
		// frames, corresponding to the
		// returned buffer(s).
		uint64_t          in_ui64TimeStampInNanoTime,   // Time stamp, in nanotime, to be used
		// internally by the  File Reader for 
		// asynchronous buffering sequencing.
		bool              in_bWait             // Specifies the  File Reader behavior if it runs 
		// out of internal buffers to process GetBuffer().
		) = 0;

	virtual HRESULT __stdcall GetAudioBuffer
		(
		// File Reader returned [out].
		IDsAudioSamples*  out_apIAudioSamples[],        // IDsAudioSamples* audio samples 
		// array that receives any of the audio 
		// samples corresponding to
		// out_pdPositionInsidePlayListInFrames.
		unsigned long*    io_pulAudioSamplesArraySize,  // Audio samples array size specifying how many
		// audio samples buffers the caller expect
		// [in], and how many audio samples buffers 
		// the  File Reader returned [out].
		unsigned long     in_aulNumberOfSamplesPerBuffer[],   // Number of audio samples the caller 
		// expects per IDsAudioSamples buffer [in].
		double*           out_pdPositionInsidePlayListInFrames,  // Position inside the cutlist, in 
		// frames, corresponding to the
		// returned buffer(s).
		uint64_t          in_ui64TimeStampInNanoTime,   // Time stamp, in nanotime, to be used
		// internally by the  File Reader for 
		// asynchronous buffering sequencing.
		bool              in_bWait             // Specifies the  File Reader behavior if it runs 
		// out of internal buffers to process GetBuffer().
		) = 0;

	virtual HRESULT __stdcall SetPlayList
		(
		IDsPlayList*               in_pIPlayList,             // Pointer to the IDsPlayList interface
		// to be read by the  File Reader.
		SDsPlaylistConfiguration*  in_psPlayListConfiguration // Pointer to a configuration structure 
		// used by the File Reader to 
		// determine how it should process data.
		) = 0;

	//
	// Summary:
	//    Retrieves the playlist currently in use.
	// Return value:
	//    - DS_NOERROR, if completed successfully. 
	//    - DS_E_PLAYLIST_NOT_SET, if the  File Reader does not hold a playlist. 
	virtual HRESULT __stdcall GetPlayList
		(
		IDsPlayList** out_ppIPlayList    // Pointer to the address of the IDsPlayList interface.
		) = 0;

	//
	// Summary:
	//    Controls the amount of buffering to sustain.
	// Return value:
	//    - DS_NOERROR, if completed successfully. 
	//    - DS_E_OUT_OF_MEMORY, if there is no more memory available for allocation, or an error
	//      occurred while allocating the memory. 
	//    - DS_E_OUT_OF_POOL_MEMORY, if no more pool memory is available, or an error occurred 
	//      during pool allocations. 
	//    - DS_E_INVALID_PIPELINE_SIZE, if the pipeline size in frames is zero. 
	// Remarks:
	//    - This method is used to balance file reading efficiency and performance. 
	//    - Increasing the pipeline size can improve file reading reliability at the expense of a 
	//      larger memory footprint. 
	virtual HRESULT __stdcall SetPipelineSize
		(
		unsigned long in_ulPipelineSizeInFrames   // Maximum number of positions, in frames, to be
		// buffered by the  File Reader.
		) = 0;
	//
	// Summary:
	//    Specifies the playlist to be read.
	// Return value:
	//    - DS_NOERROR, if completed successfully. 
	//    - DS_E_INVALID_POINTER, the provided pointer is NULL. 
	virtual HRESULT __stdcall GetPipelineSize
		(
		unsigned long* out_pulPipelineSizeInFrames   // Maximum number of positions, in frames,
		// buffered by the  File Reader.
		) = 0;

	//
	// Summary:
	//    Specifies the next GetBuffer position and specifies Seek behavior.
	// Return value:
	//    - DS_NOERROR, if completed successfully. 
	//    - DS_E_OUT_OF_RANGE, if the specified position is outside the playlist range. 
	// Remarks:
	//    - DS_E_OUT_OF_RANGE is validated against the playlist duration.
	virtual HRESULT __stdcall Seek
		(
		uint64_t in_ui64PositionInsidePlayListInFrames  // Position to seek to inside the playlist, in frames.
		) = 0;

	//
	// Summary:
	//    Specifies the next GetBuffer position and Play behavior.
	// Return value:
	//    - DS_NOERROR, if completed successfully. 
	//    - DS_E_OUT_OF_RANGE, if the specified position is outside the playlist range. 
	// Remarks:
	//    - DS_E_OUT_OF_RANGE is validated against the playlist duration. 
	//    - The caller can ask Play() to wait until the desired position is back from the disk before
	//      returning. This guarantees that the next IDsReader::GetBuffer() call will not incur
	//      a disk I/O penalty. 
	virtual HRESULT __stdcall Play
		(
		uint64_t in_ui64PositionInsidePlayListInFrames, // Position inside the playlist, in frames, 
		// to start the play from.
		bool     in_bWaitUntilFrameIsReady              // If set to true, the play blocks until 
		// in_ui64PositionInsidePlayListInFrames is 
		// back from disk.
		) = 0;

	//
	// Summary:
	//    Specifies to the  File Reader that it should not expect to be called with GetBuffer.
	// Return value:
	//    - DS_NOERROR, if completed successfully. 
	// Remarks:
	//    - The  File Reader will forward this information to internal modules that need to know whether they should be streaming or not.
	//    - This method applies to the latest codec generation only such as the AVC codec. If the user application's playlist includes 
	//       AVC elements it should make sure to pause the  Reader when appropriate (usually based on the user interacting with the application).
	//    - Pausing the  Reader is not required on other codec platforms.
	//    - The Pause will take effect immediately. All positions already returned in GetBuffer calls remain valid until they are consumed.
	//    - The user application should not call IDsReader::Play to resume from a paused state since this would temporarily negatively affect disk IO performance.
	//    - To resume from a paused state the user application only needs to resume calling GetBuffer.
	virtual HRESULT __stdcall Pause() = 0;

	//
	// Summary:
	//    Specifies that the application wants all  File Reader activities to stop.
	// Return value:
	//    - DS_NOERROR, if completed successfully. 
	// Remarks:
	//    - This will cause the  File Reader to free any internal buffering it currently holds.
	//    - This is the equivalent to calling IDsReader::Seek( n ) where n is a position outside the playlist range.
	//    - The  Reader will release all cached playlist elements. The user application can safely modify the playlist following a Stop.
	//    - The Stop will take effect immediately. All positions already returned in GetBuffer calls remain valid until they are consumed.
	//    - To resume from a stopped state the user application needs to call IDsReader::Play or IDsReader::Seek.
	virtual HRESULT __stdcall Stop() = 0;

	//
	// Summary:
	//    Specifies the increment between each GetBuffer() call.
	// Return value:
	//    - DS_NOERROR. This method always succeeds. 
	// Remarks:
	//    - Setting a shuttle rate of 1.0 implies a standard playback scenario. 
	//    - Rate is limited by the application and can be negative, zero, or positive. 

	virtual HRESULT __stdcall Shuttle
		(
		double in_dShuttleRate  // Increment applied by the  File Reader to determine the position
		// to retrieve between each GetBuffer() call.
		) = 0;

	//
	// Summary:
	//    Retrieves the configured shuttle rate.
	// Return value:
	//    - DS_NOERROR. This method always succeeds. 
	virtual HRESULT __stdcall GetShuttleRate
		(
		double* out_pdShuttleRate  // Increment currently applied by the  File Reader to determine
		// the position to retrieve between each GetBuffer() call.
		) = 0;

};

// {5451FE56-7BA9-4e71-BCB0-85F61A0FE5d7}
DEFINE_GUID(IID_IDsAvReader2,
	0x5451fe56, 0x7ba9, 0x4e71, 0xbc, 0xb0, 0x85, 0xf6, 0x1a, 0xf, 0xe5, 0xd7);

interface IDsAvReader2 : public IUnknown
{
	virtual HRESULT __stdcall Seek
	(
		uint64_t in_ui64PositionInsidePlayListInFrames
		, int flag
	) = 0;

};
//////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Creates the different playlist types.
//
//////////////////////////////////////////////////////////////////////////////////
// {219E8194-00CE-4985-809E-898A2FF7E6F7}
DEFINE_GUID(IID_IDsPlayListCreator, 
			0x219e8194, 0xce, 0x4985, 0x80, 0x9e, 0x89, 0x8a, 0x2f, 0xf7, 0xe6, 0xf7);


interface IDsPlayListCreator : public IUnknown
{
	//
	// Summary:
	//    Creates a playlist that contains video elements only.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if a pointer is NULL.
	//    - DS_E_OUT_OF_MEMORY, if there is no more memory available for allocation.
	virtual HRESULT __stdcall CreatePlayList
		(
		EDsPlayListType in_eType,
		IDsPlayList ** out_ppIPLayList   // Pointer that receives the new playlist interface pointer.
		) = 0;
};
//////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    This interface is used to set up a playlist.
//
//////////////////////////////////////////////////////////////////////////////////
// {A94B1350-1A11-413b-AE43-668536B61310}
DEFINE_GUID(IID_IDsPlayList, 
			0xa94b1350, 0x1a11, 0x413b, 0xae, 0x43, 0x66, 0x85, 0x36, 0xb6, 0x13, 0x10);

interface IDsPlayList : public IUnknown
{
public:
	//
	// Summary:
	//    Creates an AVI (audio/video interleaved) playlist element with index.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if a pointer is NULL.
	//    - DS_E_OUT_OF_MEMORY, if there is no memory available for allocation.
	//    - DS_E_INVALID_PATH_SIZE, if the filename is bigger then MAX_PATH.

	virtual HRESULT  __stdcall CreateElement
		(
		TCHAR                           * in_pwszFileName,  // File name associated with the element.
			TCHAR                           * in_pwszFileNameVideoIndex,  // File name associated with the element's video index.
			TCHAR                           * in_pwszFileNameAudioIndex,  // File name associated with the element's audio index.
		SDsAVElementProperties * in_pProperties,   // Pointer to the properties of the element to create.
		IDsPlayListElement               ** out_ppNewElement  // Pointer to the newly created element interface.
		) = 0;

	//
	// Summary:
	//    Retrieves the total playlist duration.
	// Return value:
	//    - Length of the playlist in frames.

	virtual uint64_t __stdcall GetDuration(void) = 0;

	//
	// Summary:
	//    Returns a playlist element according to a specified frame number.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if a pointers is NULL.
	//    - DS_E_ELEMENT_NOT_FOUND, if there is no element at the specified frame number.
	// Remarks:
	//    - The returned element reference count is incremented by one.

	virtual HRESULT  __stdcall GetSpecifiedElement
		(
		uint64_t              in_ui64FramePos,    // Frame position of the required element.
		IDsPlayListElement ** out_ppIElement      // Pointer that receives the element corresponding
		// to the frame number.
		) = 0;

	//
	// Summary:
	//    Returns the playlist type (audio, AVI, graphic, or video).
	// Return value:
	//    - Returns the playlist type. 
	virtual EDsPlayListType __stdcall GetPlayListType(void) = 0;

	//
	// Summary:
	//    Indicates whether or not the seamless looping playlist property is enabled.
	// Return value:
	//    - TRUE, if seamless looping is desired.
	//    - FALSE, if seamless looping is not desired.
	// Remarks:
	//    - This property is implemented in the  Reader.
	virtual bool     __stdcall IsSeamlessLoopingEnabled(void) = 0;

	//
	// Summary:
	//    Enables or disables the seamless looping playlist property of the playlist. 
	// Remarks:
	//    This property is implemented in the  Reader.
	virtual void     __stdcall EnableSeamlessLooping
		(
		bool in_bEnable   // State of the seamless looping feature.  The value TRUE indicates that
		// seamless looping is desired, while FALSE indicates that it isn't.
		) = 0;

	//
	// Summary:
	//    Inserts an element in the playlist at a specified frame number.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if a pointer is NULL.
	//    - DS_E_ALREADY_IN_USE, if the file reader is accessing the element.
	//    - DS_E_ELEMENT_FOUND, if there already is an element at the specified location in the playlist.
	//    - DS_E_INVALID_ELEMENT_TYPE, if the element is not of the right type for the playlist.
	virtual HRESULT  __stdcall InsertElement
		(
		IDsPlayListElement * in_pElement,      // Element to add to the playlist.
		uint64_t             in_ui64FramePos   // Frame position of the element in the playlist.
		) = 0;

	//
	// Summary:
	//    Appends an element to the end of the playlist.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_ALREADY_IN_USE, if the file reader is accessing the element.
	//    - DS_E_INVALID_ELEMENT_TYPE, if the element type is incorrect for the playlist.
	virtual HRESULT  __stdcall AppendElement
		(
		IDsPlayListElement * in_pElement // Inserts an element at the end of the playlist.
		) = 0;

	//
	// Summary:
	//    Removes an element from the playlist.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if a pointer is NULL.
	//    - DS_E_ALREADY_IN_USE, if the file reader is accessing the element.
	//    - DS_E_ELEMENT_NOT_FOUND, if the element is not in the playlist.
	virtual HRESULT  __stdcall RemoveElement(
		IDsPlayListElement * in_poElement   // Element to remove.
		) = 0;

	//
	// Summary:
	//    Removes all elements from the playlist.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	// Remarks:
	//    - The element used by the reader is cached so it is used until a new element is needed.
	virtual HRESULT  __stdcall RemoveAllElements(void) = 0;

	//
	// Summary:
	//    Moves an element from its current position to a new position.(只能平行移动，不能跨素材移动)
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_INVALID_POINTER, if a pointer is NULL.
	//    - DS_E_ALREADY_IN_USE, if the file reader is accessing the element.
	//    - DS_E_ELEMENT_FOUND, if there already is an element at the specified location in the playlist.
	//    - DS_E_ELEMENT_LOST, an error occurred and the element is no longer in the playlist.
	virtual HRESULT  __stdcall MoveElement
		(
		IDsPlayListElement * in_poElement,        // Element to move.
		uint64_t             in_ui64NewPosition   // New position of the element.
		) = 0;

	//
	// Summary:
	//    Retrieves the next element of the playlist.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if a pointer is NULL.
	//    - DS_E_ELEMENT_NOT_FOUND, if there is no element in the playlist.
	// Remarks:
	//    - The returned element reference count is incremented by one.
	virtual HRESULT  __stdcall GetNextElement
		(
		IDsPlayListElement *  in_pIFromElement,   // Pointer to the interface of the element preceding
		// to the required one.
		IDsPlayListElement ** out_ppINextElement  // Pointer to the required element interface.
		) = 0;

	//
	// Summary:
	//    Retrieves the previous element of the playlist.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if a pointer is NULL.
	//    - DS_E_ELEMENT_NOT_FOUND, if there is no element in the playlist.
	// Remarks:
	//    - The returned element reference count is incremented by 1.
	//    - If the in_pPrevElement parameter is NULL, the last element of the play list is returned.
	virtual HRESULT  __stdcall GetPreviousElement
		(
		IDsPlayListElement *  in_pIFromElement,   // Pointer to the interface of the element next to
		// the required one.
		IDsPlayListElement ** out_ppIPrevElement  // Pointer to the required element interface.
		) = 0;

	//
	// Summary:
	//    Sets the time in playlist frames before the next played element in the playlist is opened. 
	// Return value:
	//    - DS_NOERROR, if completed successfully
	// Remarks:
	//		- A preroll of at least 2 seconds is recommended.
	//    - A higher preroll will ensure a smooth transition between two elements of the playlist,
	//		  preventing dropped frames between elements.
	//		- It will be impossible to change the next playlist element if it is already opened, so the higher
	//		  the preroll, the more time in advance is needed to make changes to the next playlist element.
	//    - An infinite preroll will open the next element as soon as possible after playback
	//		  of the current element has begun.
	//	   - A preroll of 0 will open the next playlist element only when the reader is ready to read from that element.
	virtual HRESULT  __stdcall SetNextElementOpeningPreroll
		(
		uint64_t in_ui64PrerollInFrames	// Number of frames before the next played element in the playlist is opened (default = 0)
		) = 0;

	//
	// Summary:
	//    Returns the currently set time in playlist frames before the next played element in the playlist is opened. 
	// Return value:
	//    - Currently set time in playlist frames before the next played element in the playlist is opened.
	virtual uint64_t  __stdcall GetNextElementOpeningPreroll() = 0;
	//
	// Summary:
	//    Inserts new elements into an existing playlist at a specified frame position.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if a pointer is NULL.
	//    - DS_E_INVALID_ELEMENT_TYPE, if the new playlist doesn't match the playlist type of the existing playlist.

	virtual HRESULT  __stdcall InsertPlayList
		(
		IDsPlayList * in_pIPlayList,    // New playlist to be inserted.
		uint64_t      in_ui64FramePos,  // Frame position of the insertion.
		bool          in_bOverwrite     // Specifies whether or not the inserted playlist will overwrite any elements it overlaps.
		) = 0;


	virtual HRESULT __stdcall InsertElementAndUnit
		(
		IDsPlayListElement * in_pElement,      // Element to add to the playlist.
		uint64_t             in_ui64FramePos   // Frame position of the element in the playlist.
		) = 0;

	virtual HRESULT __stdcall SplitElement
		(
		uint64_t			 in_ui64StartPos,	// Split from this point.
		uint64_t             in_ui64Dur			// Split end this point.
		) = 0;

	//add at 2013-6

	virtual HRESULT  __stdcall CreateMemElement
		(
		void                           * in_pBuffer,  // File name associated with the element.
		int                              size,  // File name associated with the element's video index.
		SDsAVElementProperties * in_pProperties,   // Pointer to the properties of the element to create.
		IDsPlayListElement               ** out_ppNewElement  // Pointer to the newly created element interface.
		) = 0;

};

//////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Initializes the common parameters of an element.
//
//////////////////////////////////////////////////////////////////////////////////
///redefine in DsNleDef.h
//#ifndef CG_SINGLE
//
//#define CG_SINGLE			0
//#define CG_UROLL			1
//#define CG_DROLL			2
//#define CG_LCRAWL			3
//#define CG_RCRAWL			4
//#define CG_SONG			5
//#define CG_MULTI			6
//#define CG_SEQ			7
//#define	CG_FX				8
//
//#endif

// {86AC47A2-FA7B-45ec-9E8D-E68458796CB9}
DEFINE_GUID(IID_IDsPlayListElement, 
			0x86ac47a2, 0xfa7b, 0x45ec, 0x9e, 0x8d, 0xe6, 0x84, 0x58, 0x79, 0x6c, 0xb9);

interface IDsPlayListElement : public IUnknown
{
public:    
	//
	// Summary:
	//    Retrieves the file name associated with the element.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if a pointer is NULL.
	// Remarks:
	//    - The size of the parameter can have a maximum of MAX_PATH characters.
	virtual HRESULT  __stdcall GetFileName
		(
			TCHAR * out_pwszFileName    // Pointer to a string that receives the file name.
		) = 0;

	//
	// Summary:
	//    Indicates that the file is being written to by another process.
	// Return value:
	//    - True, if the file is incomplete and there is another process writing to it. 
	//    - False, if the file is complete and no other process is writing to it.
	// Remarks:
	//    - This option indicates to the file reader that the file header is changing and that it 
	//      must be re-read at regular intervals.
	virtual bool __stdcall IsRemoteTimeDelay(void) = 0;

	//
	// Summary:
	//    Set start position to the element in the playlist.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_ALREADY_IN_USE, if the file reader is accessing the element.
	//    - DS_E_ELEMENT_FOUND, if there already is an element at the specified location in the playlist.
	// Remarks:
	//    - When changing the start position of an element, the application must ensure that there is enough
	//     space in the playlist.
	virtual HRESULT  __stdcall SetStartPosition
		(
		uint64_t in_ui64StartPosition // Start Position.
		) = 0;

	//
	// Summary:
	//    Returns the element start position in the playlist.
	// Return value:
	//    - Frame position in the playlist.
	virtual uint64_t __stdcall GetStartPosition(void) = 0;

	//
	// Summary:
	//    Retrieves the number of play frames of the element.
	// Return value:
	//    - Number of frames that the file reader returns for this element.
	// Remarks:
	//    - The frame count is for interlaced or progressive files.
	virtual uint64_t __stdcall GetDuration(void) = 0;

	//
	// Summary:
	//    Sets the frame duration of the element.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_ALREADY_IN_USE, if the file reader is accessing the element.
	//    - DS_E_ELEMENT_FOUND, if there already is an element at the specified location in the playlist.
	// Remarks:
	//    - When changing the duration of an element, the application must ensure that there is enough
	//     space in the playlist.
	virtual HRESULT  __stdcall SetDuration
		(
		uint64_t in_ui64FrameDuration // Frame count.
		) = 0;

	//
	// Summary:
	//    Retrieves the element start frame inside the file.
	// Return value:
	//    - Trim-in frame position in the file.
	virtual uint64_t __stdcall GetTrimInPosition(void) = 0;

	//
	// Summary:
	//    Sets the element start frame inside the file.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_ALREADY_IN_USE, if the file reader is accessing the element. 
	// Remarks:
	//    - When the Trim-in position is greater than the Trim-out position, the file is accessed in
	//      reverse.
	//    - The Trim-out position can be past the end of the file.
	virtual HRESULT  __stdcall SetTrimInPosition
		(
		uint64_t in_ui64FramerTrimIn  // Frame number.
		) = 0;

	//
	// Summary:
	//    Retrieves the element's end frame inside the file.
	// Return value:
	//    - Trim-out frame position in the file.
	virtual uint64_t __stdcall GetTrimOutPosition(void) = 0;

	//
	// Summary:
	//    Sets the element's end frame inside the file.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_ALREADY_IN_USE, if the file reader is accessing the element.
	// Remarks:
	//    - When the Trim-out position is less than the Trim-in position, the file is accessed in
	//      reverse.
	//    - The Trim-out position can be past the end of the file.
	virtual HRESULT  __stdcall SetTrimOutPosition
		(
		uint64_t in_ui64FramerTrimOut // Frame number.
		) = 0;

	//
	// Summary:
	//    Prevents modifications to the element.
	// Return value:
	//       - DS_NOERROR, if completed successfully.
	// - DS_E_ALREADY_IN_USE, if the  File Reader is accessing the element.
	// Remarks:
	//    - This method prevents the  File Reader, or any other thread to access the element until a
	//      call to IDsPlayListElement::ReleaseLock is made.
	//    - If the file reader tries to access the element that is being modified, it returns the 
	//      DS_E_ELEMENT_IN_USE error.
	virtual HRESULT  __stdcall AcquireLock(void) = 0;

	//
	// Summary:
	//    Releases the lock so that the element can be modified.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_ELEMENT_NOT_LOCKED, if the element is not in the modification state.
	virtual HRESULT  __stdcall ReleaseLock(void) = 0;

	//
	// Summary:
	//    Retrieves the  clip reader CLSID if it exists.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	virtual HRESULT  __stdcall GetCLSID
		(
		CLSID * out_pclsid   // Pointer that receives the value of the CLSID.
		) = 0;

	//
	// Summary:
	//    Retrieves the file name associated with the video index file.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if a pointer is NULL.
	// Remarks:
	//    - The size of the parameter can have a maximum of MAX_PATH characters.
	virtual HRESULT  __stdcall GetFileNameVideoIndex
		(
			TCHAR * out_pwszFileName    // Pointer to a string that receives the file name.
		) = 0;

	//
	// Summary:
	//    Retrieves the file name associated with the audio index file.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if a pointer is NULL.
	// Remarks:
	//    - The size of the parameter can have a maximum of MAX_PATH characters.
	virtual HRESULT  __stdcall GetFileNameAudioIndex
		(
			TCHAR * out_pwszFileName    // Pointer to a string that receives the file name.
		) = 0;

	//
	// Summary:
	//    Tells the  File Reader which codec should be used to decompressed the media.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	// Remarks:
	//    - This method allows the user application to choose which codec should be used to decompress the media and is 
	//       used only when more than one codecs are available for a specific compression format.
	//    - For example, if the profile supports an H.264 hardware codec you could use this method to choose between the hardware and software H.264 implementation.
	virtual HRESULT __stdcall SetVideoDecompressionCodec( const CLSID* in_pclsidCodec ) = 0;
	virtual HRESULT __stdcall SetAudioDecompressionCodec( const CLSID* in_pclsidCodec ) = 0;

	//
	// Summary:
	//    Retrieves the codec that will be used to decompress the media.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	// Remarks:
	//    - This will return the value programmed by SetDecompressionCodec.
	//    - Internally the CLSID will be set to GUID_NULL if SetDecompressionCodec was not previously called.
	virtual HRESULT __stdcall GetVideoDecompressionCodec( CLSID* out_pclsidCodec ) = 0;
	virtual HRESULT __stdcall GetAudioDecompressionCodec( CLSID* out_pclsidCodec ) = 0;

	//
	// Summary:
	//    Sets the image position of the video surface.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if a pointers is NULL.
	//    - DS_E_ALREADY_IN_USE, if the file reader is accessing the element.
	// Remarks:
	//    - The file reader calls the IDsSurface::SetGraphicPosition() method with the position, so 
	//      that the surface returned is set to the proper position.
	virtual HRESULT __stdcall SetOutputPosition
		(
		SDsPointF * in_psPosition  // Pointer to the position structure.
		) = 0;

	//
	// Summary:
	//    Retrieves the image position of the video surfaces.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_INVALID_POINTER, if a pointer is NULL.
	//    - DS_E_ALREADY_IN_USE, if the file reader is accessing the element.
	// Remarks:
	//    - See the remarks for IDsAVInterleavedPlayListElement::SetOutputPosition.
	virtual HRESULT __stdcall GetOutputPosition
		(
		SDsPointF * out_psPosition // Pointer to the position structure.
		) = 0;

	//
	// Summary:
	//    Enables/disables the modification of the audio sample buffer fade-in attribute.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_ALREADY_IN_USE, if the file reader is accessing the element. 
	// Remarks:
	//    - The first audio sample buffer of the element has the fade-in attribute enabled.
	virtual HRESULT __stdcall EnableFadeIn
		(
		bool in_bEnable   // Indicates whether fade-in is enabled or disabled.
		) = 0;

	//
	// Summary:
	//    Indicates whether fade-in attribute modification is applied.
	// Return value:
	//    - True, if the fade-in modification is applied. 
	//    - False, if the fade-in modification is not applied.
	// Remarks:
	//    - See the remarks for IDsAudioPlayListElement::EnableFadeIn.
	virtual bool    __stdcall HasFadeIn(void) = 0;

	//
	// Summary:
	//    Enables/disables the modification of the audio sample buffer fade-out attribute.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	//    - DS_E_ALREADY_IN_USE, if the file reader is accessing the element. 
	// Remarks:
	//    - The last audio sample buffer of the element has the fade-out attribute enabled.
	virtual HRESULT __stdcall EnableFadeOut
		(
		bool in_bEnable   // Indicates whether the fade-out attribute is enabled or disabled.
		) = 0;

	//
	// Summary:
	//    Indicates whether fade-out attribute modification is applied.
	// Return value:
	//    - True, if fade-out modification is applied. 
	//    - False, if fade-out modification is not applied.
	// Remarks:
	//    - See the remarks for IDsAudioPlayListElement::EnableFadeOut.
	virtual bool    __stdcall HasFadeOut(void) = 0;

	//
	// Summary:
	//    Gets the zero-based stream ID to read - in the case of multiple stream files
	// Return value:
	//    - Returns the ID of the stream to read.
	virtual unsigned long __stdcall GetVideoStreamToRead() = 0;

	//
	// Summary:
	//    Gets the zero-based stream ID to read - in the case of multiple stream files
	// Return value:
	//    - Returns the ID of the stream to read.
	virtual unsigned long __stdcall GetAudioStreamToRead() = 0;

	//
	// Summary:
	//    Sets whether or not to apply aspect ratio compensation.
	// Return value:
	//    - DS_NOERROR, if completed successfully.
	// Remarks:
	virtual HRESULT  __stdcall SetAspectRatioCompensation
		(
		bool in_bSet                  // Flag that signifies whether or not to apply
		// aspect ratio compensation.                                     
		) = 0;

	//
	// Summary:
	//    Gets the setting for whether or not to apply aspect ratio compensation.
	// Return value:
	//    - True, if aspect ratio compensation is to be applied.
	//    - False, if aspect ratio compensation is not to be applied.
	// Remarks:
	//    - The size of the parameter can have a maximum of MAX_PATH characters.
	virtual bool  __stdcall GetAspectRatioCompensation(void) = 0;

	//
	// Summary:
	//    Sets the element's original duration of the file.
	// Return value:
	//    - DS_NOERROR, if completed successfully
	//    - DS_E_ALREADY_IN_USE, if the file reader is accessing the element.
	virtual HRESULT  __stdcall SetOriginalDuration(
		uint64_t in_ui64OriginalDuration			// Original duration of the file.
		) = 0;

	//
	// Summary:
	//    Retrieves the element's original duration of the file.
	// Return value:
	//    - Original duration of the file.
	virtual uint64_t  __stdcall GetOriginalDuration(void) = 0;

	// Summary:
	//    Retrieves the type of the file.
	// Return value:
	//    - type of file
	virtual int  __stdcall GetFileType(void) = 0;

	//@ add at 2013-06
	// Summary:
	// Retrieves new cg buffer 
	virtual void* _stdcall GetMediaBuffer(int *out_size) = 0;

	// @add at 2013-12
	///Summary: Retrieves afd
	virtual int _stdcall GetAfd() = 0;
};

//////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    字幕推送显示
//
/////////////////////////////////////////////////////////////////////
// {9C4B3F11-2ABD-4C5E-ABEA-806AE48C84ED}
DEFINE_GUID(IID_IDsLiveReader, 
0x9c4b3f11, 0x2abd, 0x4c5e, 0xab, 0xea, 0x80, 0x6a, 0xe4, 0x8c, 0x84, 0xed);

interface IDsLiveReader :public IDsAvReader
{
public:
	virtual HRESULT __stdcall SetLiveFileName(LPCTSTR strFile) = 0;
	virtual HRESULT __stdcall SetVideoBuffer(IDsSurface *in_pSurface) = 0;
	virtual HRESULT __stdcall SetAudioBuffer(IDsAudioSamples *in_pAudioSamples) = 0;
	//@ add for newcg.
	virtual HRESULT __stdcall SetIndex(int index) = 0;
};

//////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Retrieves file information on any type of audio, video
//
/////////////////////////////////////////////////////////////////////
// {BFF22151-A2CC-4e19-807F-B9FC6BABBE0A}
DEFINE_GUID(IID_IDsFileInfo, 
			0xbff22151, 0xa2cc, 0x4e19, 0x80, 0x7f, 0xb9, 0xfc, 0x6b, 0xab, 0xbe, 0xa);

interface IDsFileInfo: public IUnknown
{
public:
	//
	// Summary:
	//    Retrieves the file header information.
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

};


};
