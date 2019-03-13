#pragma once

namespace DSNleLib {
	enum EDsDeviceType
	{
		eDsDeviceTypeInput,
		eDsDeviceTypeOutput,
	};

	enum EDSDeviceParametersType
	{
		eDsVideoOutputGetDefault		    = 0x100000,   // ȡĬ�ϲ��� 
		eDsVideoOutputSupperBlack			= 0x100,    // �������� bool
		eDsVideoOutputSupperWhite			= 0x200,	// �������� bool
		eDsVideoOutputChromaClipping		= 0x400,	// �������� bool
		eDsVideoOutputSetup					= 0x800,	// �������� bool
		eDsVideoOutputSchphase				= 0x1000,	// �������� double
		eDsVideoOutputSDiHorizontalDelay	= 0x2000,	// �������� long
		eDsVideoOutputAnalogHorizontalDelay = 0x4000,	// �������� long
		eDsVideoOutputComponent				= 0x8000,	// �������� bool
		eDsVideoOutputStillMode			    = 0x10000,	// �������� int ==> 0(��ģʽ)  1(֡ģʽ)
		eDsVideoOutputLast				    = 0x20000,
	};

}