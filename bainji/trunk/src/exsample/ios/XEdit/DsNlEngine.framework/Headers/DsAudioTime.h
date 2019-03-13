//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DsNleDef.h"

namespace DSNleLib
{
   

const uint64_t kaui64AudioSeq30MFrame[5][5] =
{
   //[remainder][reference]
   {    0,    0,    0,    0,    0},    // 0 frame remainder
   { 1602, 1601, 1602, 1601, 1602},    // 1-frame remainder
   { 3203, 3203, 3203, 3203, 3204},    // 2-frame remainder  
   { 4805, 4804, 4805, 4805, 4805},    // 3-frame remainder
   { 6406, 6406, 6407, 6406, 6407}     // 4-frame remainder
};

const uint64_t kaui64AudioSeq30MField[5][5] = 
{
   //[remainder][reference ]
   {    0,    0,    0,    0,    0},    // 0 field remainder
   {  801,  801,  800,  801,  801},    // 1-field remainder
   { 1602, 1601, 1601, 1602, 1602},    // 2-field remainder  
   { 2402, 2402, 2402, 2403, 2403},    // 3-field remainder
   { 3203, 3203, 3203, 3204, 3203}     // 4-field remainder
};

const uint64_t kaui64AudioSeq60MField[5][5] = 
{
   //[remainder][reference ]
   {    0,    0,    0,    0,    0},    // 0 field remainder
   {  400,  401,  400,  401,  400},    // 1-field remainder
   {  801,  801,  801,  801,  800},    // 2-field remainder  
   { 1201, 1202, 1201, 1201, 1201},    // 3-field remainder
   { 1602, 1602, 1601, 1602, 1601}     // 4-field remainder
};


inline uint64_t DsGetNanoTimeFromAudioSamples(uint64_t in_ui64AudioSamples, SDsAudioSamplesDescription & in_rsAudioSamplesDescription);
inline uint64_t DsGetAudioSamplesFromNanoTime(uint64_t in_ui64NanoTime, SDsAudioSamplesDescription & in_rsAudioSamplesDescription);
inline uint64_t DsGetReferencedVideoUnitsFromAudioSamples(uint64_t in_ui64AudioSamples, int64_t in_i64RefVideoUnit, const SDsResolutionInfo& in_rsResolutionInfo, uint64_t in_ui64FrameRateNum=1,uint64_t in_ui64FrameRateDenum=1);
inline uint64_t DsGetAudioSamplesFromReferencedVideoUnits(uint64_t in_ui64VideoUnit, int64_t in_i64RefVideoUnit, const SDsResolutionInfo& in_rsResolutionInfo,  uint64_t in_ui64FrameRateNum=1,uint64_t in_ui64FrameRateDenum=1);
inline uint64_t DsGetReferencedVideoFramesFromAudioSamples(uint64_t in_ui64AudioSamples, int64_t in_i64RefVideoFrame, EDsFrameRate in_eVideoFrameRate,  uint64_t in_ui64FrameRateNum=1,uint64_t in_ui64FrameRateDenum=1);
inline uint64_t DsGetAudioSamplesFromReferencedVideoFrames(uint64_t in_ui64VideoFrame, int64_t in_i64RefVideoFrame, EDsFrameRate in_eVideoFrameRate, uint64_t in_ui64FrameRateNum=1,uint64_t in_ui64FrameRateDenum=1);
inline uint64_t DsGetVideoUnitFromAudioSamples(uint64_t in_ui64AudioSamples, const SDsResolutionInfo& in_rsResolutionInfo,  uint64_t in_ui64FrameRateNum=1,uint64_t in_ui64FrameRateDenum=1);
inline uint64_t DsGetAudioSamplesFromVideoUnit(uint64_t in_ui64VideoUnit, const SDsResolutionInfo& in_rsResolutionInfo,  uint64_t in_ui64FrameRateNum=1,uint64_t in_ui64FrameRateDenum=1);

/////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Obtains the corresponding nano-time for x amount of audio samples.
//
/////////////////////////////////////////////////////////////////////////////////
inline uint64_t DsGetNanoTimeFromAudioSamples
   (
   uint64_t                     in_ui64AudioSamples,           // Number of samples.
   SDsAudioSamplesDescription & in_rsAudioSamplesDescription   // Sample's format for nanotime computation.
   )
{
   return ((in_ui64AudioSamples * ( uint64_t )10000000  ) / in_rsAudioSamplesDescription.sWaveFormat.ulSamplesPerSec);
}

/////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Obtains the corresponding sample for x amount of nano-time.
//
/////////////////////////////////////////////////////////////////////////////////
inline uint64_t DsGetAudioSamplesFromNanoTime
   (
   uint64_t                     in_ui64NanoTime,               // Nanotime to convert.
   SDsAudioSamplesDescription & in_rsAudioSamplesDescription   // Sample's format for nanotime computation.
   )
{
   return (((in_ui64NanoTime + (uint64_t)1) * in_rsAudioSamplesDescription.sWaveFormat.ulSamplesPerSec)/(uint64_t)10000000 );
}

/////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Evaluate the corresponding sample count for a duration/position and reference in video units 
//    (eRes).
//
// Example:
//    You can use this function to evaluate how many audio samples you need for "d" video units of
//    duration at a specific clock time expressed in the same video units. To do so you set 
//    in_ui64VideoUnit = d and pass your clock time to in_i64RefVideoUnit.
//
//       SampleLength = DsGetAudioSamplesFromReferencedVideoUnits(d, YourClockTime, sResolutionInfo)
//
//    In this example the duration is static and the reference in the audio sequence is dynamic 
//    (YourClockTime). Since this function applies a modulo "sequence length" to the reference,
//    successive calls with your next clock time will return the sample values contained in the
//    audio sequence.
//
//    In NTSC 30M /frame, with duration = 1 frame
//    <CODE>
//    DsGetAudioSamplesFromReferencedVideoUnits(1, 0, sResolutionInfo) = 1602
//    DsGetAudioSamplesFromReferencedVideoUnits(1, 1, sResolutionInfo) = 1601
//    DsGetAudioSamplesFromReferencedVideoUnits(1, 2, sResolutionInfo) = 1602
//    DsGetAudioSamplesFromReferencedVideoUnits(1, 3, sResolutionInfo) = 1601
//    DsGetAudioSamplesFromReferencedVideoUnits(1, 4, sResolutionInfo) = 1602
//    DsGetAudioSamplesFromReferencedVideoUnits(1, 5, sResolutionInfo) = 1602, and so forth...
//    </CODE>
//
// Example:
//    You can use this function to evaluate a position in audio samples for a "p" position in video
//    units with a reference in the audio sequence. Supposing that we want to start at the first
//    position of the audio sequence, set in_ui64VideoUnit = p and in_i64RefVideoUnit = 0;
//
//       SamplePosition = DsGetAudioSamplesFromReferencedVideoUnits(p, 0, sResolutionInfo)
//
//    In this example the position is dynamic and the reference in the audio sequence is static.
//
//    In NTSC 30M /frame, with reference = 0
//    <CODE>
//    DsGetAudioSamplesFromReferencedVideoUnits(1, 0, sResolutionInfo) = 1602
//    DsGetAudioSamplesFromReferencedVideoUnits(2, 0, sResolutionInfo) = 3203
//    DsGetAudioSamplesFromReferencedVideoUnits(3, 0, sResolutionInfo) = 4805
//    DsGetAudioSamplesFromReferencedVideoUnits(4, 0, sResolutionInfo) = 6406
//    </CODE>
//
//    In NTSC 30M /frame, with reference = 1 (start forward in sequence)
//    <CODE>
//    DsGetAudioSamplesFromReferencedVideoUnits(1, 1, sResolutionInfo) = 1601
//    DsGetAudioSamplesFromReferencedVideoUnits(2, 1, sResolutionInfo) = 3203
//    DsGetAudioSamplesFromReferencedVideoUnits(3, 1, sResolutionInfo) = 4804
//    DsGetAudioSamplesFromReferencedVideoUnits(4, 1, sResolutionInfo) = 6406
//    </CODE>
//
// Remarks:
//    - Changing the reference in an audio sequence will return different values for 30M and 60M 
//      resolutions only.
/////////////////////////////////////////////////////////////////////////////////
inline uint64_t DsGetAudioSamplesFromReferencedVideoUnits
   (
   uint64_t                 in_ui64VideoUnit,   // Position, or duration in video units.
   int64_t                  in_i64RefVideoUnit, // Reference in video units in the audio sequence. 
                                                   // This reference tells only where to start in the 
                                                   // audio sequence. Both negative and positive
                                                   // reference are supported. Values greater than 
                                                   // the sequence length are "moduloed". 
                                                   // (in_i64RefVideoUnit % (sequence length))
                                                   // This parameter is used for 30M and 60M 
                                                   // resolutions only.
   const SDsResolutionInfo& in_rsResolutionInfo, // SDsResolutionInfo structure containing resolution 
                                                   // information.
    uint64_t in_ui64FrameRateNum,
	uint64_t in_ui64FrameRateDenum				  // variable frame rate value	
   )
{
   uint64_t ui64Samples = 0;
   switch(in_rsResolutionInfo.eFrameRate)
   {
   case keDsFrameRateVariable:
	  if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop) // we are in field mode divided by 2
         ui64Samples = in_ui64VideoUnit * ((48000*in_ui64FrameRateDenum)/in_ui64FrameRateNum) / 2;  
      else
         ui64Samples = in_ui64VideoUnit * ((48000*in_ui64FrameRateDenum)/in_ui64FrameRateNum);       // where 2000 = 48000/24
	   break;
   case keDsFrameRate24:
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop) // we are in field mode divided by 2
         ui64Samples = in_ui64VideoUnit * 2000 / 2;   // where 2000 = 48000/24
      else
         ui64Samples = in_ui64VideoUnit * 2000;       // where 2000 = 48000/24
      break;
   case keDsFrameRate24M:
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop) // we are in field mode divided by 2
      {
         ui64Samples = in_ui64VideoUnit * 1001;       // where 1001 = 48000/(24 * 1000/1001 ) / 2
      }
      else
      {
         ui64Samples = in_ui64VideoUnit * 2002;       // where 2002 = 48000/(24 * 1000/1001 )
      }
      break;
   case keDsFrameRate25:
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop) // we are in field mode divided by 2
         ui64Samples = in_ui64VideoUnit * 1920 / 2;   // where 1920 = 48000/25
      else
         ui64Samples = in_ui64VideoUnit * 1920;       // where 1920 = 48000/25
      break;
   case keDsFrameRate30:
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop) // we are in field mode divided by 2
         ui64Samples = in_ui64VideoUnit * 1600 / 2;   // where 1600 = 48000/30
      else
         ui64Samples = in_ui64VideoUnit * 1600;       // where 1600 = 48000/30
      break;
   case keDsFrameRate30M:  // this is 29.97 rate in frame or fields, we need to do special calculations for sequence
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop) 
      {
         int64_t iRefIndex = in_i64RefVideoUnit%5;
         if(iRefIndex < 0)
         {
            iRefIndex = 5 + iRefIndex;
         }
         ui64Samples = (( in_ui64VideoUnit / 5 ) * 4004 ) + kaui64AudioSeq30MField[in_ui64VideoUnit%5][iRefIndex];
      }
      else
      {
         int64_t iRefIndex = in_i64RefVideoUnit%5;
         if(iRefIndex < 0)
         {
            iRefIndex = 5 + iRefIndex;
         }
         ui64Samples = (( in_ui64VideoUnit / 5 ) * 8008 ) + kaui64AudioSeq30MFrame[in_ui64VideoUnit%5][iRefIndex];
      }
      break;
   case keDsFrameRate50:
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop) // we are in field mode divided by 2
         ui64Samples = in_ui64VideoUnit * 960 / 2;    // where 960 = 48000/50
      else
         ui64Samples = in_ui64VideoUnit * 960;        // where 960 = 48000/50
      break;
   case keDsFrameRate60:
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop) // we are in field mode divided by 2
         ui64Samples = in_ui64VideoUnit * 800 / 2;    // where 800 = 48000/60
      else
         ui64Samples = in_ui64VideoUnit * 800;        // where 800 = 48000/60
      break;
   case keDsFrameRate60M: // this is 59.98 rate in frame or fields, we need to do special calculations for sequence
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop) 
      {
         int64_t iRefIndex = in_i64RefVideoUnit%5;
         if(iRefIndex < 0)
         {
            iRefIndex = 5 + iRefIndex;
         }
         ui64Samples = (( in_ui64VideoUnit / 5 ) * 2002 ) + kaui64AudioSeq60MField[in_ui64VideoUnit%5][iRefIndex];
      }
      else
      {
         int64_t iRefIndex = in_i64RefVideoUnit%5;
         if(iRefIndex < 0)
         {
            iRefIndex = 5 + iRefIndex;
         }
         ui64Samples = (( in_ui64VideoUnit / 5 ) * 4004 ) + kaui64AudioSeq30MField[in_ui64VideoUnit%5][iRefIndex]; //60M FRAME shares the same LUT as 30M Field
      }
      break;
   default:
      break;
   }
   return ui64Samples;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Evaluates the corresponding lowest entire video unit count (eRes) from an amount of audio samples
//    and a video unit reference.
//
// Remarks:
//    One sample duration can make a difference in 30M or 60M resolution depending on the position 
//    in the audio sequence.
//
//    For example, in NTSC 30M /frame
//
//    DsGetReferencedVideoUnitsFromAudioSamples(1601, 0, sResolutionInfo) will return 0, because
//    the first frame in NTSC 30M must contain 1602 samples. That is, 1 sample is missing to
//    complete the frame.
//
//    On the other hand, DsGetReferencedVideoUnitsFromAudioSamples(1601, 1, sResolutionInfo) will
//    return 1 frame because a frame duration at a reference of 1 in the audio sequence is expected
//    to be 1601.
// 
/////////////////////////////////////////////////////////////////////////////////
inline uint64_t DsGetReferencedVideoUnitsFromAudioSamples
   (
   uint64_t                 in_ui64AudioSamples,   // Position, or duration in audio samples.
   int64_t                  in_i64RefVideoUnit,    // Reference in video units in the audio 
                                                      // sequence. This reference tells only where
                                                      // to start in the audio sequence. Both 
                                                      // negative and positive reference are 
                                                      // supported. Values greater than the sequence
                                                      // length are "moduloed". 
                                                      // (in_i64RefVideoUnit % (sequence length))
                                                      // This parameter is used for 30M and 60M 
                                                      // resolutions only.
   const SDsResolutionInfo& in_rsResolutionInfo,    // SDsResolutionInfo structure containing 
                                                      // resolution information.
    uint64_t in_ui64FrameRateNum,
	uint64_t in_ui64FrameRateDenum// variable frame rate value	
   )
{
   uint64_t ui64VideoUnit = 0;
   unsigned long ulSampleRate = 48000;
   switch(in_rsResolutionInfo.eFrameRate)
   {

   case keDsFrameRateVariable:
	  if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop)
      {
         ui64VideoUnit = (( (in_ui64AudioSamples *in_ui64FrameRateNum)/in_ui64FrameRateDenum) * 2) / ulSampleRate;  // 24 frame rate.
      }
      else
      {
         ui64VideoUnit = (( (in_ui64AudioSamples *in_ui64FrameRateNum)/in_ui64FrameRateDenum))/ ulSampleRate;       // 24 frame rate.
      }
	   break;
   case keDsFrameRate24:
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop)
      {
         ui64VideoUnit = (in_ui64AudioSamples * (24 * 2)) / ulSampleRate;  // 24 frame rate.
      }
      else
      {
         ui64VideoUnit = (in_ui64AudioSamples * (24))/ ulSampleRate;       // 24 frame rate.
      }
      break;
   case keDsFrameRate24M:
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop)      // we are in field mode multiplied by 2
      {
         ui64VideoUnit = (in_ui64AudioSamples / 1001);                     // sequence is 1001 samples/field @ 48KHz
      }
      else
      {
         ui64VideoUnit = (in_ui64AudioSamples / 2002);                     // sequence is 2002 samples/frame @ 48KHz
      }
      break;
   case keDsFrameRate25:
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop)      // we are in field mode multiplied by 2
         ui64VideoUnit = (in_ui64AudioSamples * (25 * 2)) / ulSampleRate;  // 25 frame rate.
      else
         ui64VideoUnit = (in_ui64AudioSamples * (25)) / ulSampleRate;      // 25 frame rate.
      break;
   case keDsFrameRate30:
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop)      // we are in field mode multiplied by 2
         ui64VideoUnit = (in_ui64AudioSamples * (30 * 2)) / ulSampleRate;  // 30 frame rate.
      else
         ui64VideoUnit = (in_ui64AudioSamples * (30)) / ulSampleRate;      // 30 frame rate.
      break;
   case keDsFrameRate30M: 
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop)      
      {
         // set reference position in sequence
         int64_t iRefIndex = in_i64RefVideoUnit%5; 
         if(iRefIndex < 0)
         {
            iRefIndex = 5 + iRefIndex;    // adjust index for negative references
         }

         // Sequence is made of 5 fields. The durations of those fields @ ref zero are:
         // 801 - 801 - 800 - 801 - 801 (in samples)
         uint64_t ui645FieldSequence = ( in_ui64AudioSamples / 4004 );           // The amount of complete 5 fields sequences...
         uint64_t ui645FieldSequenceInSamples = ( ui645FieldSequence * 4004 );   // ...converted back into samples

         // Find the number of fields of the unfinished sequence (remainder)
         uint64_t ui64RemainderInSamples = in_ui64AudioSamples - ui645FieldSequenceInSamples;

         uint64_t ui64RemainderInFields = 0;

         if (ui64RemainderInSamples >= kaui64AudioSeq30MField[4][iRefIndex])
         {
            ui64RemainderInFields = 4;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MField[3][iRefIndex])
         {
            ui64RemainderInFields = 3;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MField[2][iRefIndex])
         {
            ui64RemainderInFields = 2;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MField[1][iRefIndex])
         {
            ui64RemainderInFields = 1;
         }

         ui64VideoUnit = ui645FieldSequence * 5 + ui64RemainderInFields;
      }
      else
      {
         // set reference position in sequence
         int64_t iRefIndex = in_i64RefVideoUnit%5; 
         if(iRefIndex < 0)
         {
            iRefIndex = 5 + iRefIndex;             // adjust index for negative references
         }

         // Sequence is made of 5 frames. The durations of those frames @ ref zero are:
         // 1602 - 1601 - 1602 - 1601 - 1602 (in samples)
         uint64_t ui645FrameSequence = ( in_ui64AudioSamples / 8008 );           // The amount of complete 5 frames sequences...
         uint64_t ui645FrameSequenceInSamples = ( ui645FrameSequence * 8008 );   // ...converted back into samples

         // Find the number of frames of the unfinished sequence (remainder)
         uint64_t ui64RemainderInSamples = in_ui64AudioSamples - ui645FrameSequenceInSamples;
         uint64_t ui64RemainderInFrames = 0;

         if (ui64RemainderInSamples >= kaui64AudioSeq30MFrame[4][iRefIndex])
         {
            ui64RemainderInFrames = 4;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MFrame[3][iRefIndex])
         {
            ui64RemainderInFrames = 3;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MFrame[2][iRefIndex])
         {
            ui64RemainderInFrames = 2;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MFrame[1][iRefIndex])
         {
            ui64RemainderInFrames = 1;
         }

         ui64VideoUnit = ui645FrameSequence * 5 + ui64RemainderInFrames;
      }
      break;
   case keDsFrameRate50:
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop) // we are in field mode multiplied by 2
         ui64VideoUnit = (in_ui64AudioSamples * (50 * 2 )) / ulSampleRate; // 50 frame rate.
      else
         ui64VideoUnit = (in_ui64AudioSamples * (50)) / ulSampleRate; // 50 frame rate.
      break;
   case keDsFrameRate60:
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop) // we are in field mode multiplied by 2
         ui64VideoUnit = (in_ui64AudioSamples * (60 * 2)) / ulSampleRate; // 60 frame rate.
      else
         ui64VideoUnit = (in_ui64AudioSamples  * (60)) / ulSampleRate; // 60 frame rate.
      break;
   case keDsFrameRate60M: // this is 59.94 rate in frame or fields
      if(in_rsResolutionInfo.eScanMode == keDsScanModeFirstFieldTop || 
         in_rsResolutionInfo.eScanMode == keDsScanModeSecondFieldTop) 
      {
         // set reference position in sequence
         int64_t iRefIndex = in_i64RefVideoUnit%5; 
         if(iRefIndex < 0)
         {
            iRefIndex = 5 + iRefIndex;             // adjust index for negative references
         }

         // Sequence is made of 5 fields. The durations of those fields @ ref zero are:
         // 400 - 401 - 400 - 400 - 401 (in samples)
         uint64_t ui645FieldSequence = ( in_ui64AudioSamples / 2002 );           // The amount of complete 5 fields sequences...
         uint64_t ui645FieldSequenceInSamples = ( ui645FieldSequence * 2002 );   // ...converted back into samples

         // Find the number of fields of the unfinished sequence (remainder)
         uint64_t ui64RemainderInSamples = in_ui64AudioSamples - ui645FieldSequenceInSamples;

         uint64_t ui64RemainderInFields = 0;

         if (ui64RemainderInSamples >= kaui64AudioSeq60MField[4][iRefIndex])
         {
            ui64RemainderInFields = 4;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq60MField[3][iRefIndex])
         {
            ui64RemainderInFields = 3;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq60MField[2][iRefIndex])
         {
            ui64RemainderInFields = 2;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq60MField[1][iRefIndex])
         {
            ui64RemainderInFields = 1;
         }

         ui64VideoUnit = ui645FieldSequence * 5 + ui64RemainderInFields;
      }
      else
      {
         // set reference position in sequence
         int64_t iRefIndex = in_i64RefVideoUnit%5; 
         if(iRefIndex < 0)
         {
            iRefIndex = 5 + iRefIndex;             // adjust index for negative references
         }

         // Sequence is made of 5 frames. The durations of those frames @ ref zero are:
         // 801 - 801 - 800 - 801 - 801 (in samples)
         uint64_t ui645FrameSequence = ( in_ui64AudioSamples / 4004 );           // The amount of complete 5 frames sequences...
         uint64_t ui645FrameSequenceInSamples = ( ui645FrameSequence * 4004 );   // ...converted back into samples

         // Find the number of frames of the unfinished sequence (remainder)
         uint64_t ui64RemainderInSamples = in_ui64AudioSamples - ui645FrameSequenceInSamples;
         uint64_t ui64RemainderInFrames = 0;

         // 60M frames is identical to 30M fields table
         if (ui64RemainderInSamples >= kaui64AudioSeq30MField[4][iRefIndex])
         {
            ui64RemainderInFrames = 4;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MField[3][iRefIndex])
         {
            ui64RemainderInFrames = 3;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MField[2][iRefIndex])
         {
            ui64RemainderInFrames = 2;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MField[1][iRefIndex])
         {
            ui64RemainderInFrames = 1;
         }

         ui64VideoUnit = ui645FrameSequence * 5 + ui64RemainderInFrames;
      }
      break;
   default:
      break;
   }
   return ui64VideoUnit;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Evaluate the corresponding sample count for a duration/position and reference in video frames.
//
// Example:
//    You can use this function to evaluate how many audio samples you need for "d" video frames of
//    duration at a specific video frame reference. To do so you set 
//    in_ui64VideoFrame = d and pass your video frame reference in in_i64RefVideoFrame.
//
//       SampleLength = DsGetAudioSamplesFromReferencedVideoUnits(d, YourFrameReference, eVideoFrameRate)
//
//    In this example the duration is static and the reference in the audio sequence is dynamic 
//    (YourFrameReference). Since this function applies a modulo "sequence length" to the reference,
//    successive calls with your next video frame reference will return the sample values contained in the
//    audio sequence.
//
//    In NTSC 30M, with duration = 1 frame
//    <CODE>
//    DsGetAudioSamplesFromReferencedVideoFrames(1, 0, eVideoFrameRate) = 1602
//    DsGetAudioSamplesFromReferencedVideoFrames(1, 1, eVideoFrameRate) = 1601
//    DsGetAudioSamplesFromReferencedVideoFrames(1, 2, eVideoFrameRate) = 1602
//    DsGetAudioSamplesFromReferencedVideoFrames(1, 3, eVideoFrameRate) = 1601
//    DsGetAudioSamplesFromReferencedVideoFrames(1, 4, eVideoFrameRate) = 1602
//    DsGetAudioSamplesFromReferencedVideoFrames(1, 5, eVideoFrameRate) = 1602, and so forth...
//    </CODE>
//
// Example:
//    You can use this function to evaluate a position in audio samples for a "p" position in video
//    frames with a reference in the audio sequence. Supposing that we want to start at the first
//    position of the audio sequence, set in_ui64VideoFrame = p and in_i64RefVideoFrame = 0;
//
//       SamplePosition = DsGetAudioSamplesFromReferencedVideoUnits(p, 0, eVideoFrameRate)
//
//    In this example the position is dynamic and the reference in the audio sequence is static.
//
//    In NTSC 30M, with reference = 0
//    <CODE>
//    DsGetAudioSamplesFromReferencedVideoFrames(1, 0, eVideoFrameRate) = 1602
//    DsGetAudioSamplesFromReferencedVideoFrames(2, 0, eVideoFrameRate) = 3203
//    DsGetAudioSamplesFromReferencedVideoFrames(3, 0, eVideoFrameRate) = 4805
//    DsGetAudioSamplesFromReferencedVideoFrames(4, 0, eVideoFrameRate) = 6406
//    </CODE>
//
//    In NTSC 30M, with reference = 1 (start forward in sequence)
//    <CODE>
//    DsGetAudioSamplesFromReferencedVideoFrames(1, 1, eVideoFrameRate) = 1601
//    DsGetAudioSamplesFromReferencedVideoFrames(2, 1, eVideoFrameRate) = 3203
//    DsGetAudioSamplesFromReferencedVideoFrames(3, 1, eVideoFrameRate) = 4804
//    DsGetAudioSamplesFromReferencedVideoFrames(4, 1, eVideoFrameRate) = 6406
//    </CODE>
//
// Remarks:
//    - Changing the reference in an audio sequence will return different values for 30M and 60M 
//      resolutions only.
/////////////////////////////////////////////////////////////////////////////////
inline uint64_t DsGetAudioSamplesFromReferencedVideoFrames
   (
   uint64_t     in_ui64VideoFrame,     // Position, or duration in video frames.
   int64_t      in_i64RefVideoFrame,   // Reference in video frames into the audio sequence. 
                                       // This reference tells only where to start in the 
                                       // audio sequence. Both negative and positive
                                       // references are supported. Values greater than 
                                       // the sequence length are "moduloed". 
                                       // (in_i64RefVideoFrame % (sequence length))
                                       // This parameter is used for 30M and 60M 
                                       // resolutions only.
   EDsFrameRate in_eVideoFrameRate,    // Video frame rate 
   uint64_t in_ui64FrameRateNum,
   uint64_t in_ui64FrameRateDenum	   // variable frame rate value	
   )
{
   uint64_t ui64Samples = 0;
   switch (in_eVideoFrameRate)
   {
   case keDsFrameRateVariable:
      ui64Samples = in_ui64VideoFrame * ((48000*in_ui64FrameRateDenum)/in_ui64FrameRateNum);       // where 2000 = 48000/24
      break;
   case keDsFrameRate24:
      ui64Samples = in_ui64VideoFrame * 2000;       // where 2000 = 48000/24
      break;
   case keDsFrameRate24M:
      ui64Samples = in_ui64VideoFrame * 2002;       // where 2002 = 48000/(24 * 1000/1001 )
      break;
   case keDsFrameRate25:
      ui64Samples = in_ui64VideoFrame * 1920;       // where 1920 = 48000/25
      break;
   case keDsFrameRate30:
      ui64Samples = in_ui64VideoFrame * 1600;       // where 1600 = 48000/30
      break;
   case keDsFrameRate30M:  // this is 29.97 rate in frames, we need to do special calculations for sequence
      {
         int64_t iRefIndex = in_i64RefVideoFrame%5;
         if(iRefIndex < 0)
         {
            iRefIndex = 5 + iRefIndex;
         }
         ui64Samples = (( in_ui64VideoFrame / 5 ) * 8008 ) + kaui64AudioSeq30MFrame[in_ui64VideoFrame%5][iRefIndex];
      }
      break;
   case keDsFrameRate50:
      ui64Samples = in_ui64VideoFrame * 960;        // where 960 = 48000/50
      break;
   case keDsFrameRate60:
      ui64Samples = in_ui64VideoFrame * 800;        // where 800 = 48000/60
      break;
   case keDsFrameRate60M: // this is 59.98 rate in frames, we need to do special calculations for sequence
      {
         int64_t iRefIndex = in_i64RefVideoFrame%5;
         if(iRefIndex < 0)
         {
            iRefIndex = 5 + iRefIndex;
         }
         ui64Samples = (( in_ui64VideoFrame / 5 ) * 4004 ) + kaui64AudioSeq30MField[in_ui64VideoFrame%5][iRefIndex]; //60M FRAME shares the same LUT as 30M Field
      }
      break;
   default:
      break;
   }
   return ui64Samples;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Evaluates the corresponding complete video frames from an amount of audio samples
//    and a video frame reference.
//
// Remarks:
//    One audio sample can make a difference in 30M or 60M video frame rate depending on the position 
//    in the audio sequence.
//
//    For example, in NTSC 30M
//
//    DsGetReferencedVideoFramesFromAudioSamples(1601, 0, eVideoFrameRate) will return 0, because
//    the first frame in NTSC 30M must contain 1602 samples. That is, 1 sample is missing to
//    complete the frame.
//
//    On the other hand, DsGetReferencedVideoFramesFromAudioSamples(1601, 1, eVideoFrameRate) will
//    return 1 frame because a frame duration at a reference of 1 in the audio sequence is expected
//    to be 1601.
// 
/////////////////////////////////////////////////////////////////////////////////
inline uint64_t DsGetReferencedVideoFramesFromAudioSamples
   (
   uint64_t     in_ui64AudioSamples,   // Position, or duration in audio samples.
   int64_t      in_i64RefVideoFrame,   // Reference in video frames into the audio 
                                       // sequence. This reference tells only where
                                       // to start in the audio sequence. Both 
                                       // negative and positive references are 
                                       // supported. Values greater than the sequence
                                       // length are "moduloed". 
                                       // (in_i64RefVideoFrame % (sequence length))
                                       // This parameter is used for 30M and 60M 
                                       // resolutions only.
   EDsFrameRate in_eVideoFrameRate,     // Video frame rate 
   uint64_t in_ui64FrameRateNum,
   uint64_t in_ui64FrameRateDenum	   // variable frame rate value	
   )
{
   uint64_t ui64VideoUnit = 0;
   unsigned long ulSampleRate = 48000;
   switch (in_eVideoFrameRate)
   {
   case keDsFrameRateVariable:
      ui64VideoUnit = ( ((in_ui64AudioSamples *in_ui64FrameRateNum)/in_ui64FrameRateDenum))/ ulSampleRate;       // 24 frame rate.
      break;
   case keDsFrameRate24:
      ui64VideoUnit = (in_ui64AudioSamples * (24))/ ulSampleRate;       // 24 frame rate.
      break;
   case keDsFrameRate24M:
      ui64VideoUnit = (in_ui64AudioSamples / 2002);                     // sequence is 2002 samples/frame @ 48KHz
      break;
   case keDsFrameRate25:
      ui64VideoUnit = (in_ui64AudioSamples * (25)) / ulSampleRate;      // 25 frame rate.
      break;
   case keDsFrameRate30:
      ui64VideoUnit = (in_ui64AudioSamples * (30)) / ulSampleRate;      // 30 frame rate.
      break;
   case keDsFrameRate30M: 
      {
         // set reference position in sequence
         int64_t iRefIndex = in_i64RefVideoFrame%5; 
         if(iRefIndex < 0)
         {
            iRefIndex = 5 + iRefIndex;             // adjust index for negative references
         }

         // Sequence is made of 5 frames. The durations of those frames @ ref zero are:
         // 1602 - 1601 - 1602 - 1601 - 1602 (in samples)
         uint64_t ui645FrameSequence = ( in_ui64AudioSamples / 8008 );           // The amount of complete 5 frames sequences...
         uint64_t ui645FrameSequenceInSamples = ( ui645FrameSequence * 8008 );   // ...converted back into samples

         // Find the number of frames of the unfinished sequence (remainder)
         uint64_t ui64RemainderInSamples = in_ui64AudioSamples - ui645FrameSequenceInSamples;
         uint64_t ui64RemainderInFrames = 0;

         if (ui64RemainderInSamples >= kaui64AudioSeq30MFrame[4][iRefIndex])
         {
            ui64RemainderInFrames = 4;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MFrame[3][iRefIndex])
         {
            ui64RemainderInFrames = 3;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MFrame[2][iRefIndex])
         {
            ui64RemainderInFrames = 2;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MFrame[1][iRefIndex])
         {
            ui64RemainderInFrames = 1;
         }

         ui64VideoUnit = ui645FrameSequence * 5 + ui64RemainderInFrames;
      }
      break;
   case keDsFrameRate50:
      ui64VideoUnit = (in_ui64AudioSamples * (50)) / ulSampleRate; // 50 frame rate.
      break;
   case keDsFrameRate60:
      ui64VideoUnit = (in_ui64AudioSamples  * (60)) / ulSampleRate; // 60 frame rate.
      break;
   case keDsFrameRate60M: // this is 59.94 rate in frames
      {
         // set reference position in sequence
         int64_t iRefIndex = in_i64RefVideoFrame%5; 
         if(iRefIndex < 0)
         {
            iRefIndex = 5 + iRefIndex;             // adjust index for negative references
         }

         // Sequence is made of 5 frames. The durations of those frames @ ref zero are:
         // 801 - 801 - 800 - 801 - 801 (in samples)
         uint64_t ui645FrameSequence = ( in_ui64AudioSamples / 4004 );           // The amount of complete 5 frames sequences...
         uint64_t ui645FrameSequenceInSamples = ( ui645FrameSequence * 4004 );   // ...converted back into samples

         // Find the number of frames of the unfinished sequence (remainder)
         uint64_t ui64RemainderInSamples = in_ui64AudioSamples - ui645FrameSequenceInSamples;
         uint64_t ui64RemainderInFrames = 0;

         // 60M frames is identical to 30M fields table
         if (ui64RemainderInSamples >= kaui64AudioSeq30MField[4][iRefIndex])
         {
            ui64RemainderInFrames = 4;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MField[3][iRefIndex])
         {
            ui64RemainderInFrames = 3;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MField[2][iRefIndex])
         {
            ui64RemainderInFrames = 2;
         }
         else if (ui64RemainderInSamples >= kaui64AudioSeq30MField[1][iRefIndex])
         {
            ui64RemainderInFrames = 1;
         }

         ui64VideoUnit = ui645FrameSequence * 5 + ui64RemainderInFrames;
      }
      break;
   default:
      break;
   }
   return ui64VideoUnit;
}



};  


