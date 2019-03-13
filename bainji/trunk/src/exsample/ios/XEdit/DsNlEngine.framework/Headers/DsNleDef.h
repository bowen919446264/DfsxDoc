#ifndef __DSNLEDEF_H__
#define __DSNLEDEF_H__
#pragma once

#ifdef _MSC_VER
#include <windows.h>
#include <objbase.h>
#ifndef DS_ANSI_TYPES
#define DS_ANSI_TYPES

typedef signed char int8_t;              // Signed 8-bit integer value.
typedef __int16 int16_t;            // Signed 16-bit integer value.
typedef __int32 int32_t;            // Signed 32-bit integer value.
typedef __int64 int64_t;            // Signed 64-bit integer value.

typedef unsigned __int8 uint8_t;    // Unsigned 8-bit integer value.
typedef unsigned __int16 uint16_t;  // Unsigned 16-bit integer value.
typedef unsigned __int32 uint32_t;  // Unsigned 32-bit integer value.
typedef unsigned __int64 uint64_t;  // Unsigned 64-bit integer value.

#ifndef _WCHAR_T_DEFINED
typedef uint16_t wchar_t;           // Unicode character.  Unsigned 16-bit value.
#define _WCHAR_T_DEFINED
#endif

#endif // DS_ANSI_TYPES

#elif __GNUC__
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
typedef struct _GUID {
	unsigned int  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} GUID;
typedef GUID CLSID;
typedef void *HANDLE;
typedef void *HMODULE;
typedef int32_t HRESULT;
typedef GUID IID;
#define REFIID const IID &
#define REFGUID const GUID &
__inline int IsEqualGUID(REFGUID rguid1, REFGUID rguid2)
{
	return (
		((unsigned int *)&rguid1)[0] == ((unsigned int *)&rguid2)[0] &&
		((unsigned int *)&rguid1)[1] == ((unsigned int *)&rguid2)[1] &&
		((unsigned int *)&rguid1)[2] == ((unsigned int *)&rguid2)[2] &&
		((unsigned int *)&rguid1)[3] == ((unsigned int *)&rguid2)[3]);
}
// Same type, different name
#define IsEqualIID(riid1, riid2) IsEqualGUID(riid1, riid2)
#define IsEqualCLSID(rclsid1, rclsid2) IsEqualGUID(rclsid1, rclsid2)
__inline int operator==(REFGUID guidOne, REFGUID guidOther)
{
	return IsEqualGUID(guidOne, guidOther);
}

__inline int operator!=(REFGUID guidOne, REFGUID guidOther)
{
	return !(guidOne == guidOther);
}

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        /*extern "C"*/ const static GUID  name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

#define __stdcall
#define _stdcall
#define WINAPI
#define APIENTRY
#define TRUE 1
#define FALSE 0
#define INVALID_HANDLE_VALUE 0
#define STDMETHODIMP            HRESULT
typedef int64_t __int64;
typedef void * LPVOID;
typedef long LONG;
typedef unsigned long ULONG;
typedef int INT;
typedef unsigned int UINT;
typedef unsigned int  DWORD;
typedef uintptr_t DWORD_PTR;

// iOS objc.h has been defined (typedef bool BOOL)
#ifndef OBJC_BOOL_IS_BOOL
typedef int BOOL;
#endif

typedef unsigned char BYTE;
typedef int64_t INT_PTR;
typedef int64_t LONG_PTR;
typedef int64_t LONGLONG;
typedef const char* LPCTSTR;
typedef const char* LPCWSTR;
typedef char* LPTSTR;
typedef char* LPWSTR;
#define TEXT(x) x
#define _T(x) x
#define _tcslen     strlen
#define _tcscpy     strcpy
#define _tcscpy_s   strcpy
#define _tcsncpy    strncpy
#define _tcsncpy_s  strncpy_s
#define _tcscat     strcat
#define _tcscat_s   strcat
#define _tcsupr     strupr
#define _tcsupr_s   strupr_s
#define _tcslwr     strlwr
#define _tcslwr_s   strlwr_s
#define _tcsrchr    strchr

#define _stprintf_s sprintf_s
#define _stprintf   sprintf
#define _tprintf    printf

#define _vstprintf_s    vsprintf
#define _vstprintf      vsprintf

#define _tscanf     scanf

#define TCHAR char
#define OutputDebugString printf

typedef struct tagRECT
{
	long    left;
	long    top;
	long    right;
	long    bottom;
} RECT, *PRECT;

__inline BOOL SetRect(PRECT lprc, int xLeft, int yTop, int xRight, int yBottom)
{
	lprc->left = xLeft;
	lprc->top = yTop;
	lprc->right = xRight;
	lprc->bottom = yBottom;

    return TRUE;
}
__inline BOOL EqualRect(const RECT *lprc1, const RECT *lprc2)
{
	return (lprc1->left == lprc2->left
		&& lprc1->top == lprc2->top
		&& lprc1->right == lprc2->right
		&& lprc1->bottom == lprc2->bottom);
}


#define INFINITE            (DWORD)-1  // Infinite timeout
#define _HRESULT_TYPEDEF_(_sc) ((HRESULT)_sc)
#define MAKE_HRESULT(sev,fac,code) \
    ((HRESULT) (((unsigned int)(sev)<<31) | ((unsigned int)(fac)<<16) | ((unsigned int)(code))) )
#define S_OK 0
#define NOERROR 0
#define S_FALSE 1
#define E_FAIL -1
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define E_POINTER                        _HRESULT_TYPEDEF_(0x80004003L)
#define E_NOINTERFACE                    _HRESULT_TYPEDEF_(0x80004002L)
#define E_NOTIMPL                        _HRESULT_TYPEDEF_(0x80004001L)
#define E_OUTOFMEMORY                    _HRESULT_TYPEDEF_(0x8007000EL)
#define E_INVALIDARG                     _HRESULT_TYPEDEF_(0x80070057L)
#endif

// ANSI C/C++ not defined into VC++
namespace DSNleLib {

//#define _FACDS   0x026

#define MAKE_DSHRESULTERROR(code) MAKE_HRESULT( 1, 0x026, code )
#define MAKE_DSHRESULTWARNING(code) MAKE_HRESULT( 0, 0x026, code )


//
// Summary:
//    Describes a two-dimensionnal point. Axis values are floating point.
//
struct SDsPointF
{
   float fX;   // X coordinate of the point.
   float fY;   // Y coordinate of the point.
};

//
// Summary:
//    Describes a color.
//
enum EDsColor
{
   keDsColorInvalid                = 0x00000000,   // Invalid value.
   keDsColorBlack                  = 0x00000001,   // Black color. Y component = 16.
   keDsColorSuperBlack             = 0x00000002,   // Black color. Y component = 0.
   keDsColorWhite                  = 0x00000003,   // White color. Y component = 235.
   keDsColorRed                    = 0x00000004,   // Red color.
   keDsColorGreen                  = 0x00000005,   // Green color.
   keDsColorBlue                   = 0x00000006,   // Blue color. 
   keDsColorLast                   = 0x00000007,   // End of list indicator.
   keDsColorForceDWord             = 0x7FFFFFFF    // Force the enumerated values to be 32 bits.
};

//
// Summary:
//    Specifies the resolution aspect ratio.
//
enum EDsAspectRatio
{
   keDsAspectRatioInvalid, // Invalid value.
   keDsAspectRatio_4_3,    // Specifies that the resolution has an aspect ratio of 4:3.
   keDsAspectRatio_16_9,   // Specifies that the resolution has an aspect ratio of 16:9.
   keDsAspectRatio_2_1,    // Specifies that the resolution has an aspect ratio of 2:1.   
   keDsAspectRatioLast     // End of list indicator.
};
//
// Summary:
//    Describes the number of frames per second.
//
enum EDsFrameRate
{
   keDsFrameRateInvalid,   // Invalid value.
   keDsFrameRate24,        // 24 frames per second.
   keDsFrameRate24M,       // 23.98 frames per second (24/1.001).
   keDsFrameRate25,        // 25 frames per second.
   keDsFrameRate30,        // 30 frames per second.
   keDsFrameRate30M,       // 29.97 frames per second (30/1.001).
   keDsFrameRate50,        // 50 frames per second.
   keDsFrameRate60,        // 60 frames per second.
   keDsFrameRate60M,       // 59.94 frames per second (60/1.001).
   keDsFrameRateVariable,  // Indicates that the frame rate is variable. This flag is only valid at
                              // the input resolution and it enables the Varicam frame detection 
                              // circuit.
   keDsFrameRateLast	      // End of list indicator.
};
//
// Summary:
//    Indicates whether the video is interlaced or progressive. For interlaced video, the field 
//    containing the top line is also indicated.
//
enum EDsScanMode
{
   keDsScanModeInvalid,                // Invalid value.
   keDsScanModeFirstFieldTop,          // Indicates that the video is interlaced and that the first 
                                          // field contains the top line (for example, PAL video).
   keDsScanModeSecondFieldTop,         // Indicates that the video is interlaced and that the second 
                                          // field contains the top line (for example, NTSC video).
   keDsScanModeProgressive,            // Indicates that the video is progressive (not interlaced).
   keDsScanModeProgressiveSegmented,   // Indicates that the video format is PsF (Progressive 
                                          // Segmented Frame).
   keDsScanModeInterlacedFieldsInAFrame,  // Indicates that the video format is interlaced fields-in-a-frame. This should be used to instruct
                                             // the  File Writer or  File Reader to provide surfaces with the IFF polarity.
   keDsScanModeLast                    // End of list indicator.
};
//
// Summary:
//    Specifies all information that defines a resolution.
//
struct SDsResolutionInfo
{
   uint32_t       size;                // Structure size in bytes.
   unsigned int  ulWidth;             // Resolution width in pixels.
   unsigned int  ulHeight;            // Resolution height in pixels.
   unsigned int  ulComponentBitCount; // Number of bits per component of a pixel.
   EDsAspectRatio eAspectRatio;        // Aspect ratio of the output monitor. 
   EDsFrameRate   eFrameRate;          // Number of frames per second.
   EDsScanMode    eScanMode;           // Indicates if the scan mode is interlace or progressive.
   // It also indicates which field is first in interlace mode.
};
//
// Summary:
//    Describes the pixel organization of a surface.
//
enum EDsSurfaceFormat
{
   keDsSurfaceFormatInvalid,     // Invalid value.
   keDsSurfaceFormatARGBGraphic, // This kind of surface usually holds graphics.  Each pixel is 
                                 // composed of four components: alpha, red, green and blue (in 
                                 // this order). The bit depth of this surface is 4 x 
                                 // ComponentBitCount. Can be allocated in host and graphics 
                                 // engine memory. Black is 0, white is 255.

   keDsSurfaceFormatRGBGraphic,  // Same as ARGB surface, but without the alpha component. The bit 
                                 // depth of this surface is 3 x ComponentBitCount. Can be 
                                 // allocated in host memory only. Black is 0, white is 255.

   keDsSurfaceFormatARGBVideo,   // This kind of surface usually holds graphics. Each pixel is 
                                 // composed of four components: alpha, red, green and blue (in 
                                 // this order). The bit depth of this surface is 4 x 
                                 // ComponentBitCount. Can be allocated in host and graphics 
                                 // engine memory. Black is 16, white is 235. Can contain super 
                                 // white and/or super black.

   keDsSurfaceFormatRGBVideo,    // Same as ARGB surface, but without the alpha component. The bit 
                                 // depth of this surface is 3 x ComponentBitCount. Can be 
                                 // allocated in host memory only. Black is 16, white is 235.
                                 // Can contain super white and/or super black.

   keDsSurfaceFormatYUYV422,     // Also called YUY2, this kind of surface holds video or ancillary data.  
                                 // For each two pixels, the data organization is YU YV. The bit depth of
                                 // this surface is 2 x ComponentBitCount. Can be allocated in 
                                 // host memory, I/O local memory, and graphics engine 
                                 // memory.

   keDsSurfaceFormatYUAYVA4224,  // This kind of surface holds video. For each two pixels, the data 
                                 // organization is YUA YVA. The bit depth of this surface is 3 x
                                 // ComponentBitCount. Can be allocated in host memory only.

   keDsSurfaceFormatA,     // This kind of surface holds alpha information only. The bit depth of 
                           // this surface is 1 x ComponentBitCount. Can be allocated in host and 
                           // graphics engine memory.

   keDsSurfaceFormatDepthStencil,   // This kind of surface holds a Z-buffer and a Stencil-buffer 
                                    // required for some 3D processing. This surface can be 
                                    // allocated only with a ComponentBitCount of 8. Each pixel has
                                    // 24 bits of Z and 8 bits of Stencil. Can be allocated in 
                                    // graphics engine memory only.

   keDsSurfaceFormatDuDv,  // This kind of surface is used by the bump mapping (distortion) effects. 

   keDsSurfaceFormatDuDvL, // This kind of surface is used by the bump mapping (distortion) effects 
                           // using luminance. 

   keDsSurfaceFormatDvCam_411,   // This kind of surface contains NTSC YUV411 video data compressed 
                                 // by a DV codec at 25 Mb/sec. 

   keDsSurfaceFormatDvCam_420,   // This kind of surface contains PAL YUV420 video data compressed 
                                 // by a DV codec at 25 Mb/sec.

   keDsSurfaceFormatDvCPro_411,  // This kind of surface contains NTSC or PAL YUV411 video data 
                                 // compressed by a DV codec at 25 Mb/sec.

   keDsSurfaceFormatDv50_422,    // This kind of surface contains YUV422 video data compressed by a 
                                 // DV codec at 50 Mb/sec.

   keDsSurfaceFormatD12_1080_3to2_422,       // This kind of surface contains DV100 data for resolution
                                             // 1080i at 23.98 fps and 29.97 fps (scaling ratio of 2/3).

   keDsSurfaceFormatD12_1080_4to3_422,       // This kind of surface contains DV100 data for resolution
                                             // 1080i at 50 fps (scaling ratio of 3/4).

   keDsSurfaceFormatD12_720_4to3_422_Fr1,    // This kind of surface contains DV100 data for resolution
                                             // 720p at 23.98 fps, 29.97 fps and 59.94 fps (scaling ratio
                                             // of 3/4) frame 0, 2, 4...

   keDsSurfaceFormatD12_720_4to3_422_Fr2,    // This kind of surface contains DV100 data for resolution
                                             // 720p at 23.98 fps, 29.97 fps and 59.94 fps (scaling ratio
                                             // of 3/4) frame 1, 3, 5...

   keDsSurfaceFormatMpegIBPMainProfileMainLevel_420,     // This kind of surface contains YUV420 video 
                                                         // data compressed by an MPEG codec. The 
                                                         // surface can be an I, B, or P-frame. The 
                                                         // specific information of this MPEG surface
                                                         // is described by the MPEG_IBP_Specific structure 
                                                         // in the surface description. This surface 
                                                         // supports SD (standard definition) video 
                                                         // resolutions.

   keDsSurfaceFormatMpegIBPMainProfileHighLevel_420,     // This kind of surface contains YUV420 video
                                                         // data compressed by an MPEG codec. The surface
                                                         // can be an I, B, or P-frame. The specific 
                                                         // information of this MPEG surface is described
                                                         // by the MPEG_IBP_Specific structure in the
                                                         // surface description. This surface supports
                                                         // HD (high definition) video resolutions.

   keDsSurfaceFormatMpegIBP422ProfileMainLevel_422,      // This kind of surface contains YUV422 video
                                                         // data compressed by an MPEG codec. The surface
                                                         // can be an I, B, or P-frame. The specific 
                                                         // information of this MPEG surface is described
                                                         // by the MPEG_IBP_Specific structure in the 
                                                         // surface description. This surface supports
                                                         // SD video resolutions.

   keDsSurfaceFormatMpegIBP422ProfileHighLevel_422,      // This kind of surface contains YUV422 video 
                                                         // data compressed by an MPEG codec. The
                                                         // surface can be an I, B, or P-frame. The
                                                         // specific information of this MPEG surface
                                                         // is described by the MPEG_IBP_Specific structure
                                                         // in the surface description. This surface 
                                                         // supports HD video resolutions.

   keDsSurfaceFormatMpegIFrameMainProfileMainLevel_420,  // This kind of surface contains YUV422 video
                                                         // data compressed by an MPEG codec. The surface
                                                         // is an I-frame that supports SD video resolutions.

   keDsSurfaceFormatMpegIFrameMainProfileHighLevel_420,  // This kind of surface contains YUV422 video
                                                         // data compressed by an MPEG codec. The surface
                                                         // is an I-frame that supports HD video resolutions.

   keDsSurfaceFormatMpegIFrame422ProfileMainLevel_422,   // This kind of surface contains data of type
                                                         // MPEG-2 I-frame 422 Profile @ Main
                                                         // Level (SD resolutions).

   keDsSurfaceFormatMpegIFrame422ProfileHighLevel_422,   // This kind of surface contains data of type
                                                         // MPEG-2 I-frame 422 Profile @ High
                                                         // Level (HD resolutions).

   keDsSurfaceFormatMpegD10_422,                         // This kind of surface contains YUV422 video data 
                                                         // compressed by an MPEG-D10 codec. The surface is an 
                                                         // I-frame.

   keDsSurfaceFormatRLE,                                 // This kind of surface contains a bitmap compressed by a 
                                                         // run-length encoding (RLE) algorithm. The 
                                                         // ComponentBitCount can be 8, 10, or 16.
 
   keDsSurfaceFormatEffectPatternData,    // This kind of surface contains raw data that is used to 
                                          // store private effect data associated with a GFX 
                                          // (Matrox effect pattern) file.

   keDsSurfaceFormatMJpegBaseline_422,    // This kind of surface contains YUV422 video data 
                                          // compressed by a Motion-JPEG lossy (baseline) codec. 

   keDsSurfaceFormatMJpegLossless_422,    // This kind of surface contains YUV422 video data 
                                          // compressed by a Motion-JPEG lossless 
                                          // (uncompressed quality) codec. 

   keDsSurfaceFormatHDOffline,   // This kind of surface contains data for the HD offline codec. 

   keDsSurfaceFormatAYUV4444,    // This kind of surface contains AYUV4444 video data. Each pixel 
                                 // is composed of four components: alpha, Y, U and V (in this 
                                 // order). The bit depth of this surface is 4 x ComponentBitCount. 
                                 // Can be allocated in host memory (any profile) and graphics engine 
                                 // memory (GPU FX card only). 

   keDsSurfaceFormatDigiserverVBI,     // This kind of surface contains Matrox DigiServer VBI 
                                       // information. Used to maintain backward compatibility
                                       // with DigiServer VBI files.

   keDsSurfaceFormatMpegIFrame422ProfileMainLevel_WithAlpha,   // This kind of surface contains data 
                                                               // of type MPEG-2 I-frame 422 Profile 
                                                               // @ Main Level (SD resolution)
                                                               // with alpha.

   keDsSurfaceFormatMpegIFrame422ProfileHighLevel_WithAlpha,   // This kind of surface contains data
                                                               // of type MPEG-2 I-frame 422 Profile
                                                               // @ High Level (HD resolution)
                                                               // with alpha.

   keDsSurfaceFormatAlphaLuminance,    // This kind of surface holds alpha and luminance information
                                       // and is used for 3D effect processing. The component bit
                                       // count supported for this surface is 8 bits. Can be
                                       // allocated in graphics engine memory only.
   keDsSurfaceFormatBGRAGraphic,       
   keDsSurfaceFormatBGRAVideo,       

   keDsSurfaceFormatAVClip,        // This kind of surface contains compressed data used by the 
                                       //  Clip Reader/Writer.

   keDsSurfaceFormatD12_720p50_4to3_422_Fr1,    // This kind of surface contains DV100 data for 
                                                // resolution 720p at 25 fps and 50 fps (scaling 
                                                // ratio of  3/4) frame 0, 2, 4...

   keDsSurfaceFormatD12_720p50_4to3_422_Fr2,    // This kind of surface contains DV100 data for 
                                                // resolution 720p at 25 fps and 50 fps (scaling 
                                                // ratio of  3/4) frame 1, 3, 5...

   keDsSurfaceFormatAVCIntraClass50,            // This kind of surface contains AVC-Intra Class 50 data.
   keDsSurfaceFormatAVCIntraClass100,           // This kind of surface contains AVC-Intra Class 100 data.
   keDsSurfaceFormatAVCIntra,                   // This kind of surface contains AVC-Intra Class 50 or Class 100 data.

   keDsSurfaceFormatProRes,                  // This kind of surface contains ProRes 422 (standard or HQ) data.
   
   keDsSurfaceFormatAVC,                        // This kind of surface contains AVC data.

   keDsSurfaceFormatRLEAnimation,               // This kind of surface contains video data compressed by 
                                                //Apple RLE. It will be decoded to RGB.

   keDsSurfaceFormatRLEAnimation_WithAlpha,     // This kind of surface contains video data compressed by 
                                                //Apple RLE 32-bit. It will be decoded to ARGB.

   keDsSurfaceFormatA2R10G10B10Graphic,         // This kind of surface holds ARGB graphics. Each pixel is 
                                                // composed of four components: alpha, red, green and blue (in 
                                                // this order). The bit depth of this surface is 32 bits. Alpha 
                                                // has two bits, and the R, G, and B components each have 10 bits. 
                                                // Can be allocated in host memory. 
                                                // Use this format for graphic card surfaces.

   keDsSurfaceFormatA2R10G10B10Video,           // This kind of surface holds ARGB video. Each pixel is 
                                                // composed of four components: alpha, red, green and blue (in 
                                                // this order). The bit depth of this surface is 32 bits. Alpha 
                                                // has two bits, and the R, G, and B components each have 10 bits. 
                                                // Can be allocated in host and in I/O local memory. 
                                                // R, G, and B values should be between 0x040 and 0x3AC, and must 
                                                // never be lower than 0x04 or higher than 0x3FC.
                                                // Use this format for SDI RGB dual-link surfaces.

   keDsSurfaceFormatMatroxAncillaryData,        // This kind of surface holds digital ancillary data.
                                                // Can be allocated in host memory only.

   keDsSurfaceFormatV210,                       // This kind of surface holds YUV 10-bit data in the Mac v210 format.
                                                // Can be allocated in host memory only. Only valid in the Matrox SDK for Mac.
   
   keDsSurfaceFormat2VUY,        // This kind of surface holds video in the 2vuy 8 bit Mac format. For each 
                                 // two pixels, the data organization is YU YV. The bit depth of
                                 // this surface is 2 x 8 bits. Can be allocated in 
                                 // host memory only. Only valid in the Matrox SDK for Mac.
   keDsSurfaceFormatMpeg2Transport,  // This kind of surface contains MPEG-2 transport stream data. 
                                 // It can contain any number of audio and/or video streams, optionally 
                                 // compressed. It must follow the ISO 13818-1 specification.

   keDsSurfaceFormatDNxHD,       // This kind of surface contains Avid DNxHD data.

   keDsSurfaceFormatLast                        // End of list indicator.
};

//
// Summary:
//    Indicates in what format to output NTSC closed captioning.
//
enum EDsNTSCClosedCaptionFormat
{
   keDsNTSCClosedCaptionFormatInvalid,
   keDsNTSCClosedCaptionFormatLine21   = 1,  // Output Line 21 CEA608 captions
   keDsNTSCClosedCaptionFormatDigital  = 2,  // Output CEA708 digital captions
   keDsNTSCClosedCaptionFormatBoth     = 3,  // Output both Line 21 CEA608 and CEA708 digital captions
   keDsNTSCClosedCaptionFormatLast
};

enum EDsVANCFormatType
{
   keDsVancFormatTypeInvalid,
   keDsVancFormatTypeLegacy,           // Use the legacy surface format: YUV422
   keDsVancFormatTypeMatroxAncillary,  // Use the new surface format: MatroxAncillaryDataFormat
   keDsVancFormatTypeLast
};


//
// Summary:
//    Describes VANC specific playlist initialization settings for the  File Reader.
//
struct SDsVANCPlaylistOptions
{
   uint32_t                   size;
   EDsNTSCClosedCaptionFormat eNTSCCaptionsFormat; // Defaults to Line 21.
   EDsVANCFormatType          eVANCFormatType;     // Defaults to legacy.
};

//
// Summary:
//    Describes the playlist initialization settings for the  File Reader.
//
struct SDsPlaylistConfiguration
{
   uint32_t       size;                // Specifies the size of the structure in bytes.
   unsigned int  ulWidth;             // Maximum expected resolution width to be present in the playlist.
   // Used by the  File Reader to compute memory allocations.
   unsigned int  ulHeight;            // Maximum expected resolution height to be present in the playlist.
   // Used by the  File Reader to compute memory allocations.
   unsigned int  ulComponentBitCount; // Maximum expected bit depth to be present in the playlist.
   // Used by the  File Reader to compute memory allocations.
   bool           bVideo;              // Specifies whether or not the playlist contains video elements.
   bool           bAudio;              // Specifies whether or not the playlist contains audio elements.
   bool           bVBI;                // Specifies whether or not the playlist contains VBI elements.
   bool           bKey;                // Specifies whether or not the playlist can contain 4:2:2:4 elements, 
   bool           bTimeCode;           // Specifies whether or not the playlist contains TimeCode elements
   // which increases memory usage.
   EDsFrameRate   eFrameRate;          // Frame rate associated with the playlist. All elements must respect 
   // this frame rate.
   EDsScanMode    eScanMode;           // Scan mode associated with the playlist. All elements must respect 
   // this scan mode.
   EDsSurfaceFormat eConsolidationSurfaceFormat;   // Compressed format to return in IDsReader::GetBuffer, 
   SDsVANCPlaylistOptions sVANCOptions; // Options related to VANC and Vbi. Unused if the bVbi flag is set to false.
};

//
// Summary:
//    Specifies the playlist type.
//
// Remarks:
//    - The enumeration values are made to be ORed together.
//
enum EDsPlayListType
{
   keDsPlayListTypeInvalid       = 0,  // The first element is 0 and invalid.
   keDsPlayListTypeVideo         = 1,  // Specifies a video-only playlist.
   keDsPlayListTypeAudio         = 2,  // Specifies an audio-only playlist.
   keDsPlayListTypeAVInterleaved = 3,   // Specifies an AVI-only playlist.
   keDsPlayListTypeCg = 4               // Specifies an graphic-only playlist.
};

//
// Summary:
//    This structure describes the AVI playlist element properties.
//
struct SDsAVElementPropertiesV1
{
   uint32_t				   size;                   // Structure size in bytes.
   uint64_t                ui64FrameDuration;      // Length, in frames, of the element.
   uint64_t                ui64FrameTrimIn;        // First frame of the element in the file.
   uint64_t                ui64FrameTrimOut;       // Last frame of the element in the file.  The value
   // is exclusive, meaning that a trim from 0 to 200
   // will play field 0 to 199.
   uint64_t				   ui64FrameOriDuration;		   // Original duration of the file.
   bool                    bRemoteTimeDelay;       // Specifies that the file is still being written
   // to and that the file reader must update its
   // internal file header periodically.
   SDsPointF            ptOutputPosition;       // Specifies the image position set on the video 
   // surface by the file reader.
   bool                 bHasFadeIn;             // Specifies that the first audio sample buffer of 
   // the element has the fade-in attribute set to true.
   bool                 bHasFadeOut;            // Specifies that the last audio sample buffer of 
   // the element has the fade-out attribute set to true.
   unsigned int        ulVideoStreamIDToRead;  // zero-based stream ID to read inside file.
   unsigned int        ulAudioStreamIDToRead;  // zero-based stream ID to read inside file.
   int					subtype;				// fileformat.
   CLSID                   clsidClipReader;    // Class ID of a COM DLL that implement file custom

};

struct SDsAVElementProperties
{
   uint32_t				   size;                   // Structure size in bytes.
   uint64_t                ui64FrameDuration;      // Length, in frames, of the element.
   uint64_t                ui64FrameTrimIn;        // First frame of the element in the file.
   uint64_t                ui64FrameTrimOut;       // Last frame of the element in the file.  The value
   // is exclusive, meaning that a trim from 0 to 200
   // will play field 0 to 199.
   uint64_t				   ui64FrameOriDuration;		   // Original duration of the file.
   bool                    bRemoteTimeDelay;       // Specifies that the file is still being written
   // to and that the file reader must update its
   // internal file header periodically.
   SDsPointF            ptOutputPosition;       // Specifies the image position set on the video 
   // surface by the file reader.
   bool                 bHasFadeIn;             // Specifies that the first audio sample buffer of 
   // the element has the fade-in attribute set to true.
   bool                 bHasFadeOut;            // Specifies that the last audio sample buffer of 
   // the element has the fade-out attribute set to true.
   unsigned int        ulVideoStreamIDToRead;  // zero-based stream ID to read inside file.
   unsigned int        ulAudioStreamIDToRead;  // zero-based stream ID to read inside file.
   int					subtype;				// fileformat.
   CLSID                   clsidClipReader;    // Class ID of a COM DLL that implement file custom

   //V2
   int					Afd;
};

//
// Summary:
//    Specifies the type of the alias surface allocated.
//
enum EDsAliasAccessType
{
   keDsAliasAccessTypeInvalid,   // Invalid value.
   keDsAliasAccessTypeReadWrite, // Specifies that the alias surface can be read and written to, like 
                                    // any normal surface.
   keDsAliasAccessTypeReadOnly,  // Specifies that the alias surface is read-only. The write 
                                    // completion is signaled when the reference surface write event 
                                    // is signaled.  
   keDsAliasAccessTypeLast       // End of list indicator.
};
//
// Summary:
//    Determines the colorimetry standard of a surface.
//
enum EDsColorimetry
{
   keDsColorimetryInvalid,       // Invalid value.
   keDsColorimetryITUR_BT_601,   // Specifies that the colorimetry standard is ITUR-BT-601.
                                    // This is the standard used for SD (standard definition) video
                                    // resolutions.
   keDsColorimetryITUR_BT_709,   // Specifies that the colorimetry standard is ITUR-BT-709.
                                    // This is the standard used for HD (high definition) video
                                    // resolutions.
   keDsColorimetryLast           // End of list indicator.
};
//
// Summary:
//    Specifies the memory location of a surface.
//
enum EDsMemoryLocation
{
   keDsMemoryLocationInvalid,       // Invalid value.
   keDsMemoryLocationHost,          // Indicates that the surface is in the host (system) memory.
   keDsMemoryLocationGraphicIn,     // Indicates that the surface is in the local memory of the  
                                       // graphics engine, and will be used to input video through 
                                       // the Video Input Port (VIP). 
   keDsMemoryLocationGraphicOut,    // Indicates that the surface is in the local memory of the
                                       // graphics engine, and will be used to output video through 
                                       // the Digital Video Output (DVO) port.
   keDsMemoryLocationGraphic,       // Indicates that the surface is in the local memory of the
                                       // graphics engine, and will not be used to input video 
                                       // through the VIP or output video through the DVO port.
   keDsMemoryLocationIOBoard,       // Indicates that the surface is in the local memory of a 
                                       // DSX card with I/O capabilities.
   keDsMemoryLocationUser,          // Indicates that the memory buffer has been allocated by the 
                                       // user, and the surface has been allocated with the method 
                                       // CreateUserDataSurface.
   keDsMemoryLocationHostForGPU,    // Indicates that the surface is in the host (system) memory, but
                                       // can be accessed by the GPU (GPU FX card only).
   keDsMemoryLocationGraphicDXIn,   // Indicates that the surface is in the local memory of the
                                       // graphics engine of the GPU FX card.  This memory will be 
                                       // used to input video to the DSX card.  It can be used as 
                                       // a texture, but not as a render target.
   keDsMemoryLocationGraphicDXOut,  // Indicates that the surface is in the local memory of the
                                       // graphics engine of the GPU FX card.  This memory will be 
                                       // used to output video from the DSX card.  It can be used as 
                                       // a render target and as a texture (but the latter is not 
                                       // recommended).
   keDsMemoryLocationGraphicDX,     // Indicates that the surface is in the local memory of the
                                       // graphics engine of the GPU FX card.  It can be used as a
                                       //  texture and render target.
   keDsMemoryLocationLast           // End of list indicator.
};
//
// Summary:
//    Specifies how the image data is stored in the surface: top-down or bottom-up.
//
enum EDsImageOrigin
{
   keDsImageOriginInvalid,    // Invalid value.
   keDsImageOriginTopLeft,    // Indicates that the image data is stored top-down in the surface and
                                 // its origin is the upper left corner. 
   keDsImageOriginBottomLeft, // Indicates that the image data is stored bottom-up in the surface 
                                 // and its origin is the lower left corner.
   keDsImageOriginLast        // End of list indicator.
};
//
// Summary:
//    Specifies the polarity of a surface. 
//
enum EDsPolarity
{
   keDsPolarityInvalid,                   // Invalid value.
   keDsPolarityFirstField,                // Indicates that the surface is a first field.
   keDsPolaritySecondField,               // Indicates that the surface is a second field.
   keDsPolarityProgressiveFrame,          // Indicates that the surface is a progressive frame.
   keDsPolarityInterlacedFieldsInAFrame,  // Indicates that the surface is a frame containing two interlaced fields.
   keDsPolarityBackToBackFieldsInAFrame,  // Indicates that the surface is a frame containing two back-to-back fields.
   keDsPolarityLast                       // End of list indicator.
};
//
// Summary:
//    Specifies all information that describes a surface.
//
struct SDsSurfaceDescription
{
   uint32_t          size;                   // Structure size in bytes.
   unsigned int     ulWidth;                // Usable width of the surface in pixels.
   unsigned int     ulHeight;               // Usable height of the surface in pixels.
                                            // can have a depth greater than one.
   unsigned int     ulRowPitchInBytes;      // Indicates in bytes the true width of the memory buffer 
                                                // that holds the surface.
   unsigned int     ulComponentBitCount;    // Indicates the number of bits per component of a pixel.
   unsigned int     ulMipmapCount;          // Indicates how many mipmaps this surface has.
   unsigned int     ulBufferSizeInBytes;    // Indicates in bytes the size of the memory buffer that
   void				 *pBuffer;                                          // holds the surface. 
   EDsColorimetry    eColorimetry;           // Specifies the color range standard used in this surface.
   EDsSurfaceFormat  eFormat;                // Indicates the format of the surface.
   EDsMemoryLocation eMemoryLocation;        // Indicates in which memory location the surface has been allocated.
   EDsPolarity       ePolarity;              // Indicates the polarity of the surface. Not applicable 
                                                // for a compressed surface format.
   SDsPointF         ptDestinationPosition;  // Point that specifies how to position the surface in the
                                                // destination of an effect or a compositor.
   bool              bIsSurfaceOfTopLine;    // Determines if the first line of this field (it must be
                                                // a field surface) is the first to be displayed.
   bool              bUseAlpha;              // Specifies if the alpha of the surface must be taken 
                                                // into account by the effect.
   bool              bIsVideoShaped;         // Specifies if video is shaped or unshaped. Shaped video
                                                // means that the video has been pre-multiplied by the alpha plane.
   bool              bIsAligned;             // Specifies if the surface memory buffer is 128 bytes 
                                                // and 16 pixels aligned.
};

//
// Summary:
//    Used to get information on how to access a locked surface.
//
struct SDsLockSurfaceDescription
{
   uint32_t      size;                 // Structure size in bytes.
   unsigned int ulWidth;              // Usable width of the surface in pixels.
   unsigned int ulHeight;             // Usable height of the surface in pixels.
   unsigned int ulDepth;              // Depth of the locked surface.
   unsigned int ulRowPitchInBytes;    // Indicates in bytes the true width of the memory buffer that
                                          // holds the surface.
   void 			* pBuffer;            // Pointer to the beginning of the memory where the surface data is located.
};

//
// Summary:
//    Specifies the channel type of an audio buffer.
//
enum EDsChannelType
{
   keChannelTypeInvalid	 = 0,    // Invalid value.
   keChannelTypeMono     = 1,    // Mono channel (one channel only).
   keChannelTypeStereo   = 2,    // Stereo channel (two interleaved channels).
   keChannelType1Track   = 1,    // Same as keChannelTypeMono.
   keChannelType2Tracks  = 2,    // Same as keChannelTypeStereo.
   keChannelType4Tracks  = 4,    // Four interleaved audio tracks.
   keChannelType6Tracks  = 6,    // Six interleaved audio tracks.       Added for mixed capture AES/EBU + Embbeded Audio.
   keChannelType8Tracks  = 8,    // Height interleaved audio tracks.
   keChannelType10Tracks = 10,   // Ten interleaved audio tracks.       Added for mixed capture AES/EBU + Embbeded Audio.
   keChannelType12Tracks = 12,   // Twelve interleaved audio tracks.    Added for mixed capture AES/EBU + Embbeded Audio.
   keChannelType14Tracks = 14,   // Fourteen interleaved audio tracks.  Added for mixed capture AES/EBU + Embbeded Audio.
   keChannelType16Tracks = 16,   // Sixteen interleaved audio tracks.
   keChannelTypeLast             // End of list indicator.
};

//
// Summary:
//    Describes the audio data types that can be used.
//
enum EDsAudioDataType
{
   keAudioDataTypeInvalid           = 0,  // Invalid value.
   keAudioDataTypePCM               = 1,  // Audio data is of type Pulse Code Modulation.
   keAudioDataTypeIEEEFLoat         = 2,  // Audio data is of type is a IEEE floating point value.
   keAudioDataTypeMatroxMultiFormat = 3,  // Audio data type is not interpreted and contains raw bits.
   keAudioDataTypeAAC               = 4,  // Audio data is of type AAC.
   keAudioDataTypeLast                    // End of list indicator.
};
//
// Summary:
//    Describes a buffer's audio data format. 
//
struct SDsaWaveFormatInfo
{
   uint32_t         size;                 // Structure size in bytes. 
   EDsChannelType	  eChannelType;         // Type of channels supported (mono, stereo, etc.). 
   EDsAudioDataType eDataType;            // Type of audio data samples (PCM, float etc.). 
   unsigned int	  ulSamplesPerSec;      // Sample rate, in samples per second (hertz). 
   unsigned int	  ulBitsPerSample;      // Bits per sample for the data type. This is the container 
                                             // size of the data if the actual data size is different from
                                             // the container size. This field must be equal to or larger than 
                                             // the ulValidBitsPerSample. 
   unsigned int	  ulValidBitsPerSample; // Valid bits per sample for the data type. This is the 
                                             // actual size of the data inside the container. This 
                                             // field must be equal to or smaller than the 
                                             // ulBitsPerSample. 
};

//
// Summary:
//    Specifies different MPEG-2 profiles. 
//
enum EDsMPEG2Profile
{
   keDsMPEG2ProfileInvalid = 0 ,
   keDsMPEG2Profile_Simple ,
   keDsMPEG2Profile_Main ,
   keDsMPEG2Profile_SNRScalable ,
   keDsMPEG2Profile_SpatiallyScalable ,
   keDsMPEG2Profile_High ,
   keDsMPEG2Profile_422 ,
   keDsMPEG2Profile_Last
};

//
// Summary:
//    Specifies different MPEG-2 levels. 
//
enum EDsMPEG2Level
{
   keDsMPEG2LevelInvalid = 0 ,
   keDsMPEG2Level_Low ,
   keDsMPEG2Level_Main ,
   keDsMPEG2Level_High1440 ,
   keDsMPEG2Level_High ,
   keDsMPEG2Level_Last
};

//
// Summary:
//    MPEG specific file information used by the mvfFileInformation module.
//
struct SDsMpegFileInfo
{
   uint32_t size;                        // Structure size in bytes.
   double dBitRate;
   uint32_t ui32NumberOfFramesBetweenI;
   uint32_t ui32NumberOfFramesBetweenIandP;
   uint32_t ui32VBVBufferSize;
   EDsMPEG2Profile eProfile;
   EDsMPEG2Level eLevel;
   bool bTopFieldFirst;
   bool bConcealmentMotionVectors;
   bool bProgressiveFrame;
   double dSampleAspectRatio;
};
//
// Summary:
//    Describes the GOP structure type.
enum EDsGOPStructureType
{
   keDsGOPStructureTypeInvalid,      // Not initialized.  Invalid value.
   keDsGOPStructureTypeSimple,       // Simple GOP model.  MPEG-2 alike.
   keDsGOPStructureTypeAdvanced,     // Advance GOP model.  AVC/H.264 alike.
   keDsGOPStructureTypeLast          // Last value.  Invalid value.
};

// Used for XDCam EX pull down and also Z1U pulldown. 
enum EDsdPulldownType
{
   PullDownTypeNone = 0,
   PullDownType2_3,
   PullDownType3_2,
   PullDownType1_1,
   PullDownType2_2,
   PullDownType2_3_3_2,
   PullDownType24_25,
   PullDownTypeOther,
   PullDownTypeLast
};
enum EDsMXFFileType
{
   MXFFileTypeNone = 0,
   MXFFileTypeAtomAudio,
   MXFFileTypeP2Video,
   MXFFileTypeXdcamD10,
   MXFFileTypeXdcamDvCam,
   MXFFileTypeXdcamDvCPro,
   MXFFileTypeXdcamHD420,
   MXFFileTypeXdcamHD422,
   MXFFileTypeGenericMpeg2,
   MXFFileTypeGenericMpeg2SparseIndex,
   MXFFileType_GC,
   MXFFileTypeOP1B,
   MXFFileTypeAS02VBIType,
   MXFFileTypeLast
};
#ifndef CG_SINGLE
enum EDsCgFileType
{
	CG_SINGLE = 100,
	CG_UROLL,
	CG_DROLL,
	CG_LCRAWL,
	CG_RCRAWL,
	CG_SONG,
	CG_MULTI,
	CG_SEQ,
	CG_FX,
	CG_LAST
};
#else
enum EDsCgFileType
{
};

#endif
//
// Summary:
//    MPEG specific file information used by the mvfFileInformation module.
//
struct SDsAVCFileInfo
{
   uint32_t             size;                                  // Structure size in bytes.
   uint32_t             ui32GOPSizeInFrames;
   uint32_t             ui32DistanceBetweenReferencesInFrames;
   uint32_t             ui32Profile;
   uint32_t             ui32Level;
   uint32_t             ui32AverageBitRateInBitsPerSec;        // Averate bit rate in bits per seconds of the stream.
   EDsGOPStructureType  eGOPStructureType;                     // GOP structure type of the stream.   
};

//ProRes file info
struct SDsProResFileInfo
{
   uint32_t             size;
   uint32_t             ui32Profile;                              //ProRes profile: 422, HQ, LT, Proxy,4444
   EDsSurfaceFormat     eDecodingFormat;                          //decoding format 
   bool                 bHasAlpha;                                // Has Alpha 
};

//VC-3 or DNxHD file info
struct SDsDNxHDFileInfo
{
   uint32_t             size;
   uint32_t             ui32Profile;   // DNxHD or VC-3 profile: 36, 145, 220 or 220x
};


enum EDsFileInfo                                  
{                                                 
   keDsFileInfoInvalid = 0,                       
	keDsFileInfoAV,                                
	keDsFileInfoCg,                               
	keDsFileInfoLast                               
}; 

union FileSubType
{
	 EDsMXFFileType    eMXFFileType;           // Specifies the Mxf file types.
	 EDsCgFileType     eCgFileType; 
};
//
// Summary:
//    Contains the audio and video information of a media file. 
//
struct SDsAvFileInfo
{
   size_t               size;                   // Specifies the structure size in bytes.
   unsigned int        ulVersion;              // Specifies the version information.
   EDsFileInfo eFileInfo;
   unsigned int        ulNumVideoStreams;      // Specifies the number of video streams.
   unsigned int        ulAudioStreams;         // Specifies the number of audio streams. 
   EDsSurfaceFormat     eSurfaceFormat;         // Specifies the surface format enumerator. 
   SDsResolutionInfo    sResInfo;               // Specifies the resolution information. 
   EDsPolarity          ePolarity;              // Specifies the polarity. 
   SDsaWaveFormatInfo   sWaveInfo;              // Specifies the audio format information. 
   uint64_t             ui64VideoFrameCount;    // Specifies the number of frames in the file. 
                                                // This value does not include broken B frames 
                                                // or the start offset resulting from the time code. 
                                                // See the uiActualVideoFrameCount parameter to get 
                                                // all the frames physically stored in the file.
   uint64_t             ui64AudioSampleCount;   // Specifies the number of audio samples in the file. 
                                                // In case of multiple audio streams that have different 
                                                // durations, use the one with the shortest duration. See 
                                                // the uiActualAudioSampleCount parameter to get the sample 
                                                // count physically stored in the file.
   unsigned int        ulDataRate;             // Specifies the data rate in bits-per-second.
   uint64_t             ui64FileSize;           // Specifies the file size in bytes. 
   unsigned int        ulPitch;                // Specifies the pitch size of the surface. 
   bool                 bIsAligned;             // Specifies whether or not the start address of the data in the file is aligned to a boundary of 128 bytes.
   uint64_t             ui64ActualVideoFrameCount;    // Specifies the actual number of frames in the file. This value is greater than or equal to 
                                                // ui64VideoFrameCount because it takes into account broken Open GOPs at the beginning of 
                                                // the file, or is offset to the highest time code suggested in the file.
   uint64_t             ui64ActualAudioSampleCount;   // Specifies the actual number of audio samples in the file. This value is greater than or equal 
                                                // to ui64AudioSampleCount because it takes into account broken Open GOPs at the beginning of 
                                                // the file, or is offset to the highest time code suggested in the file.
   uint64_t             ui64OffsetFromActualStart;    // Specifies the amount that the read positions can be safely offset from the beginning (zero). 
                                                // This FrontOffset value, if specified, means that the ui64ActualVideoFrameCount value is 
                                                // larger than the ui64VideoFrameCount.
   bool                 bEmbeddedAudio;         // Specifies whether or not the audio is embedded.
   EDsColorimetry       eColorimetry;           // Specifies the colorimetry standard of the surface. Audio duration is in the unit of frames.

   EDsdPulldownType     ePullDownType;          // Type of pull down can be applied (ie material is 24 over 60).

   bool                 bIsMpegFileInfoValid;   // Specifies if data in sMpegFileInfo is valid.

   SDsMpegFileInfo      sMpegFileInfo;          // File information specific to MPEG file types.

   bool                 bIsAVCFileInfoValid;    // Specifies if data in sAVCFileInfo is valid.

   SDsAVCFileInfo       sAVCFileInfo;           // File information specific to AVC file types.

   bool                 bIsProResFileInfoValid; // specifies if data in sProResFileInfo is valid
   SDsProResFileInfo    sProResFileInfo;        // 

   bool                 bIsDNxHDFileInfoValid;  // specifies if data in sDNxHDFileInfo is valid
   SDsDNxHDFileInfo     sDNxHDFileInfo;         // File information specific to VC-3 or DNxHD file types

   union FileSubType    eSubType;
   unsigned int        ulVbiStreams;           // Specifies the number of vbi streams.
   unsigned int        ulANCStreams;           // Specifies the number of ANC streams.
   unsigned int        ulTCStreams;            // Specifies the number of TC streams.
};

//
// Summary:
//    This is the standard structure returned by the GetBufferDescription() method of the IDsAudioSamples
//    interface. This structure contains all the necessary information about the buffer, but not the buffer
//    pointer itself.
//
struct SDsAudioSamplesDescription
{
   uint32_t					size;             // Structure size in bytes. 
   unsigned int            ulMaxBufferSize;   // alloc size 
   EDsPolarity				ePolarity;        // The polarity of the corresponding video frame. 
   EDsMemoryLocation    eMemoryLocation;  // Indicates in which memory location the surface has been allocated. 
   bool						bHasFadeIn;       // Flag to indicate that the buffer contains an audio fade-in. 
   bool						bHasFadeOut;      // Flag to indicate that the buffer contains an audio fade-out. 
   SDsaWaveFormatInfo	sWaveFormat;      // Structure containing wave format information for the audio buffer. 
};

struct SDsAudioMonoVolumeInfo
{
   uint32_t Size;          // Structure size in bytes.
   float    fInitialGain;  // Volume gain at the beginning of the video unit.
   float    fFinalGain;    // Volume gain at the end of the video unit.
};

};
#endif //__DSNLEDEF_H__