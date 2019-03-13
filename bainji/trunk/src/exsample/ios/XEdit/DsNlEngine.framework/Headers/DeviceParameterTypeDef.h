#pragma once

namespace DSNleLib {
	enum EDsDeviceType
	{
		eDsDeviceTypeInput,
		eDsDeviceTypeOutput,
	};

	enum EDSDeviceParametersType
	{
		eDsVideoOutputGetDefault		    = 0x100000,   // 取默认参数 
		eDsVideoOutputSupperBlack			= 0x100,    // 参数类型 bool
		eDsVideoOutputSupperWhite			= 0x200,	// 参数类型 bool
		eDsVideoOutputChromaClipping		= 0x400,	// 参数类型 bool
		eDsVideoOutputSetup					= 0x800,	// 参数类型 bool
		eDsVideoOutputSchphase				= 0x1000,	// 参数类型 double
		eDsVideoOutputSDiHorizontalDelay	= 0x2000,	// 参数类型 long
		eDsVideoOutputAnalogHorizontalDelay = 0x4000,	// 参数类型 long
		eDsVideoOutputComponent				= 0x8000,	// 参数类型 bool
		eDsVideoOutputStillMode			    = 0x10000,	// 参数类型 int ==> 0(场模式)  1(帧模式)
		eDsVideoOutputLast				    = 0x20000,
	};

}