#pragma once

#include "DsNleDef.h"
namespace DSNleLib {
// DOM-IGNORE-BEGIN
// Extracted from the HQ System clock specification
// LUT Info:                    SD/HD    Scan modes         Frame rates    Nb Infos
const unsigned int kauiClockLUT  [2] [keDsScanModeLast] [keDsFrameRateLast] [3] =
{  
   // SD Tables
   {
      // keDsScanModeInvalid
      {
      //  D    E   TRIPLE_DUR
         {0,   0,    0},  // keDsFrameRateInvalid
         {0,   0,    0},  // keDsFrameRate24
         {0,   0,    0},  // keDsFrameRate24M
         {0,   0,    0},  // keDsFrameRate25
         {0,   0,    0},  // keDsFrameRate30
         {0,   0,    0},  // keDsFrameRate30M
         {0,   0,    0},  // keDsFrameRate50
         {0,   0,    0},  // keDsFrameRate60
         {0,   0,    0},  // keDsFrameRate60M
         {0,   0,    0}   // keDsFrameRateVariable
      },
      // keDsScanModeFirstFieldTop
      {
      //  D    E           TRIPLE_DUR
         {0,   0,             0},      // keDsFrameRateInvalid
         {0,   0,             0},      // keDsFrameRate24
         {0,   0,             0},      // keDsFrameRate24M
         {4,   270000 + 432,  150000}, // keDsFrameRate25      // Since the number of lines are not odd for every field,
         {4,   0,             125000}, // keDsFrameRate30      // we must add the number of pixel of half of a line
         {4,   225225 + 429,  125125}, // keDsFrameRate30M     
         {0,   0,             0},      // keDsFrameRate50      
         {0,   0,             0},      // keDsFrameRate60
         {0,   0,             0},      // keDsFrameRate60M
         {0,   0,             0}       // keDsFrameRateVariable
      },
      // keDsScanModeSecondFieldTop
      {
      //  D    E           TRIPLE_DUR
         {0,   0,             0},      // keDsFrameRateInvalid
         {0,   0,             0},      // keDsFrameRate24
         {0,   0,             0},      // keDsFrameRate24M
         {4,   270000 + 432,  150000}, // keDsFrameRate25      // Since the number of lines are not odd for every field,
         {4,   0,             125000}, // keDsFrameRate30      // we must add the number of pixel of half of a line
         {4,   225225 + 429,  125125}, // keDsFrameRate30M     
         {0,   0,             0},      // keDsFrameRate50      
         {0,   0,             0},      // keDsFrameRate60
         {0,   0,             0},      // keDsFrameRate60M
         {0,   0,             0}       // keDsFrameRateVariable
      },
      // keDsScanModeProgressive
      {
      //  D    E     TRIPLE_DUR
         {0,   0,       0},      // keDsFrameRateInvalid
         {10,  309375,  125000}, // keDsFrameRate24
         {10,  309375,  125125}, // keDsFrameRate24M
         {8,   270000,  150000}, // keDsFrameRate25
         {8,   0,       125000}, // keDsFrameRate30
         {8,   225225,  125125}, // keDsFrameRate30M     
         {4,   270000,  150000}, // keDsFrameRate50
         {4,   0,       125000}, // keDsFrameRate60
         {4,   225225,  125125}, // keDsFrameRate60M     
         {0,   0,       0}       // keDsFrameRateVariable
      },
      // keDsScanModeProgressiveSegmented
      {
      //  D    E          TRIPLE_DUR
         {0,   0,             0},  // keDsFrameRateInvalid
         {0,   0,             0},  // keDsFrameRate24
         {0,   0,             0},  // keDsFrameRate24M
         {8,   270000,        150000}, // keDsFrameRate25
         {8,   0,             125000},  // keDsFrameRate30
         {8,   225225 + 429,  125125}, // keDsFrameRate30M     
         {0,   0,             0},  // keDsFrameRate50
         {0,   0,             0},  // keDsFrameRate60
         {0,   0,             0},  // keDsFrameRate60M
         {0,   0,             0}   // keDsFrameRateVariable
      },
      // keDsScanModeInterlacedFieldsInAFrame
      {
         //  D    E          TRIPLE_DUR
         {0,   0,             0},  // keDsFrameRateInvalid
         {0,   0,             0},  // keDsFrameRate24
         {0,   0,             0},  // keDsFrameRate24M
         {8,   270000,        150000}, // keDsFrameRate25
         {8,   0,             125000},  // keDsFrameRate30
         {8,   225225 + 429,  125125}, // keDsFrameRate30M     
         {0,   0,             0},  // keDsFrameRate50
         {0,   0,             0},  // keDsFrameRate60
         {0,   0,             0},  // keDsFrameRate60M
         {0,   0,             0}   // keDsFrameRateVariable
      }
   },
   // HD Tables
   {
      // keDsScanModeInvalid
      {
      //  D    E     TRIPLE_DUR
         {0,   0,       0},  // keDsFrameRateInvalid
         {0,   0,       0},  // keDsFrameRate24
         {0,   0,       0},  // keDsFrameRate24M
         {0,   0,       0},  // keDsFrameRate25
         {0,   0,       0},  // keDsFrameRate30
         {0,   0,       0},  // keDsFrameRate30M
         {0,   0,       0},  // keDsFrameRate50
         {0,   0,       0},  // keDsFrameRate60
         {0,   0,       0},  // keDsFrameRate60M
         {0,   0,       0}   // keDsFrameRateVariable
      },
      // keDsScanModeFirstFieldTop
      {
      //  D    E           TRIPLE_DUR
         {0,   0,             0},         // keDsFrameRateInvalid
         {5,   309375 + 1375, 125000},    // keDsFrameRate24            // This is not a real supported resolution, it is used by the I/O drivers in Psf mode
         {5,   309375 + 1375, 125125},    // keDsFrameRate24M           // This is not a real supported resolution, it is used by the I/O drivers in Psf mode
         {4,   371250 + 1320, 150000},    // keDsFrameRate25            // Since the number of lines are not odd for every field,
         {4,   309375 + 1100, 125000},    // keDsFrameRate30            // we must add the number of pixel of half of a line
         {4,   309375 + 1100, 125125},    // keDsFrameRate30M        
         {0,   0,             0},         // keDsFrameRate50
         {0,   0,             0},         // keDsFrameRate60
         {0,   0,             0},         // keDsFrameRate60M
         {0,   0,             0}          // keDsFrameRateVariable
      },
      // keDsScanModeSecondFieldTop
      {
      //  D    E           TRIPLE_DUR
         {0,   0,             0},         // keDsFrameRateInvalid
         {5,   309375 + 1375, 125000},    // keDsFrameRate24            // This is not a real supported resolution, it is used by the I/O drivers in Psf mode
         {5,   309375 + 1375, 125125},    // keDsFrameRate24M           // This is not a real supported resolution, it is used by the I/O drivers in Psf mode
         {4,   371250 + 1320, 150000},    // keDsFrameRate25            // Since the number of lines are not odd for every field,
         {4,   309375 + 1100, 125000},    // keDsFrameRate30            // we must add the number of pixel of half of a line
         {4,   309375 + 1100, 125125},    // keDsFrameRate30M
         {0,   0,             0},         // keDsFrameRate50
         {0,   0,             0},         // keDsFrameRate60
         {0,   0,             0},         // keDsFrameRate60M
         {0,   0,             0}          // keDsFrameRateVariable
      },
      // keDsScanModeProgressive
      {
      //  D    E     TRIPLE_DUR
         {0,   0,        0},        // keDsFrameRateInvalid
         {10,  309375,   125000},   // keDsFrameRate24
         {10,  309375,   125125},   // keDsFrameRate24M
         {8,   371250,   150000},   // keDsFrameRate25
         {8,   309375,   125000},   // keDsFrameRate30
         {8,   309375,   125125},   // keDsFrameRate30M
         {4,   371250,   150000},   // keDsFrameRate50
         {4,   309375,   125000},   // keDsFrameRate60
         {4,   309375,   125125},   // keDsFrameRate60M
         {0,   0,        0}         // keDsFrameRateVariable
      },
      // keDsScanModeProgressiveSegmented
      {
      //  D    E          TRIPLE_DUR
         {0,   0,             0},      // keDsFrameRateInvalid
         {10,  309375 + 1375, 125000}, // keDsFrameRate24
         {10,  309375 + 1375, 125125}, // keDsFrameRate24M
         {8,   371250 + 1320, 150000}, // keDsFrameRate25            // Since the number of lines are not odd for every field,
         {8,   309375 + 1100, 125000}, // keDsFrameRate30            // we must add the number of pixel of half of a line
         {8,   309375 + 1100, 125125}, // keDsFrameRate30M
         {0,   0,             0},      // keDsFrameRate50
         {0,   0,             0},      // keDsFrameRate60
         {0,   0,             0},      // keDsFrameRate60M
         {0,   0,             0}       // keDsFrameRateVariable
      },
      // keDsScanModeInterlacedFieldsInAFrame
      {
         //  D    E          TRIPLE_DUR
         {0,   0,             0},      // keDsFrameRateInvalid
         {10,  309375 + 1375, 125000}, // keDsFrameRate24
         {10,  309375 + 1375, 125125}, // keDsFrameRate24M
         {8,   371250 + 1320, 150000}, // keDsFrameRate25            // Since the number of lines are not odd for every field,
         {8,   309375 + 1100, 125000}, // keDsFrameRate30            // we must add the number of pixel of half of a line
         {8,   309375 + 1100, 125125}, // keDsFrameRate30M
         {0,   0,             0},      // keDsFrameRate50
         {0,   0,             0},      // keDsFrameRate60
         {0,   0,             0},      // keDsFrameRate60M
         {0,   0,             0}       // keDsFrameRateVariable
      }
   }
};

// DOM-IGNORE-END

//!ignore me! This is to document properly the methods
inline int64_t DsRound(int64_t in_ui64Numerator, int64_t in_ui64Denominator);
inline int64_t DsGetNanoTimeForSample(int64_t in_ui64Sample, SDsResolutionInfo& in_rsResolutionInfo);
inline int64_t DsGetSampleForNanoTime(int64_t in_ui64NanoTime, SDsResolutionInfo& in_rsResolutionInfo);
inline int64_t DsGetNextSampleNanoTime(int64_t in_ui64CurrentTime, SDsResolutionInfo& in_rsResolutionInfo);
inline int64_t DsGetNanoTimeForSystemClockTicks(int64_t in_uiNbTicks, SDsResolutionInfo& in_rsResolutionInfo);
inline int64_t DsGetSystemClockTicksForNanoTime(int64_t in_ui64NanoTime, SDsResolutionInfo& in_rsResolutionInfo);

////////////////////////////////////////////////////////////////////
//
// Summary:
//    Like ANSI ceil function but for integer
//
////////////////////////////////////////////////////////////////////
inline int64_t DsRound(int64_t in_ui64Numerator, int64_t in_ui64Denominator)
{
   return (in_ui64Numerator % in_ui64Denominator) ? ((in_ui64Numerator + in_ui64Denominator) / in_ui64Denominator) : (in_ui64Numerator / in_ui64Denominator);
};

////////////////////////////////////////////////////////////////////
//
// Summary:
//    Returns the nanotime for the passed sample
//
////////////////////////////////////////////////////////////////////
inline int64_t DsGetNanoTimeForSample(int64_t in_ui64Sample, SDsResolutionInfo& in_rsResolutionInfo)
{
   unsigned int uiSDHD = ((in_rsResolutionInfo.ulHeight == 486) || (in_rsResolutionInfo.ulHeight == 576)) ? 0 : 1;
   unsigned int uiD = kauiClockLUT[uiSDHD][in_rsResolutionInfo.eScanMode][in_rsResolutionInfo.eFrameRate][0];
   unsigned int uiTD = kauiClockLUT[uiSDHD][in_rsResolutionInfo.eScanMode][in_rsResolutionInfo.eFrameRate][2];

   if((uiD == 0) || (uiTD == 0))
      return 0;

   return (DsRound(in_ui64Sample * uiD * uiTD, 3));
};

////////////////////////////////////////////////////////////////////
//
// Summary:
//    Returns the sample for the passed nanotime
//
////////////////////////////////////////////////////////////////////
inline int64_t DsGetSampleForNanoTime(int64_t in_ui64NanoTime, SDsResolutionInfo& in_rsResolutionInfo)
{
   unsigned int uiSDHD = ((in_rsResolutionInfo.ulHeight == 486) || (in_rsResolutionInfo.ulHeight == 576)) ? 0 : 1;
   unsigned int uiD = kauiClockLUT[uiSDHD][in_rsResolutionInfo.eScanMode][in_rsResolutionInfo.eFrameRate][0];
   unsigned int uiTD = kauiClockLUT[uiSDHD][in_rsResolutionInfo.eScanMode][in_rsResolutionInfo.eFrameRate][2];

   if((uiD == 0) || (uiTD == 0))
      return 0;

   return (((3 * in_ui64NanoTime) / uiTD) / uiD);
};

////////////////////////////////////////////////////////////////////
//
// Summary:
//    Returns the nanotime for the next sample.
//
////////////////////////////////////////////////////////////////////
inline int64_t DsGetNextSampleNanoTime(int64_t in_ui64CurrentTime, SDsResolutionInfo& in_rsResolutionInfo)
{
   return DsGetNanoTimeForSample((DsGetSampleForNanoTime(in_ui64CurrentTime, in_rsResolutionInfo) + 1), in_rsResolutionInfo);
};


//////////////////////////////////////////////////////////////////////////////////////////////////
//
// Summary:
//    Returns the nanotime for the number of system clock ticks.
// Remarks:
//    - There are many clock ticks per sample.
//    - DO NOT get the duration for one tick and multiply by the "nb" you want, use the macro with "nb"
//////////////////////////////////////////////////////////////////////////////////////////////////
inline int64_t DsGetNanoTimeForSystemClockTicks(int64_t in_uiNbTicks, SDsResolutionInfo& in_rsResolutionInfo)
{
   unsigned int uiSDHD = ((in_rsResolutionInfo.ulHeight == 486) || (in_rsResolutionInfo.ulHeight == 576)) ? 0 : 1;
   unsigned int uiTD = kauiClockLUT[uiSDHD][in_rsResolutionInfo.eScanMode][in_rsResolutionInfo.eFrameRate][2];

   if(uiTD == 0)
      return 0;

   return (DsRound(in_uiNbTicks * uiTD, 3));
};

////////////////////////////////////////////////////////////////////
//
// Summary:
//    Returns the clock ticks for the passed nanotime
// Remarks:
//    - There are many clock ticks per sample and it is different for every resolution
//    - DO NOT get the duration by playing with ticks 
//      use the proper macro
////////////////////////////////////////////////////////////////////
inline int64_t DsGetSystemClockTicksForNanoTime(int64_t in_ui64NanoTime, SDsResolutionInfo& in_rsResolutionInfo)
{
    unsigned int uiSDHD = ((in_rsResolutionInfo.ulHeight == 486) || (in_rsResolutionInfo.ulHeight == 576)) ? 0 : 1;
    unsigned int uiD = kauiClockLUT[uiSDHD][in_rsResolutionInfo.eScanMode][in_rsResolutionInfo.eFrameRate][0];
    unsigned int uiTD = kauiClockLUT[uiSDHD][in_rsResolutionInfo.eScanMode][in_rsResolutionInfo.eFrameRate][2];

    if((uiD == 0) || (uiTD == 0))
        return 0;

    return (((3 * in_ui64NanoTime) / uiTD) );
};

};

