#ifndef ROCIMAGEPLUGS_H
#define ROCIMAGEPLUGS_H

#ifdef  WIN32
#ifndef ROCIMAGEPLUGS_DLL_FILE
#define ROCHREASUTCLASS _declspec(dllexport)
#else
#define ROCHREASUTCLASS _declspec(dllimport)
#endif
#else
#define ROCHREASUTCLASS 
#include "../cximage/ximadef.h"
#endif //  WIN32



class CRocImagePlugs
{
public:
	virtual ~CRocImagePlugs(){};
	virtual int Open(const char* filename)=0;
	virtual int GetSourceRect(RECT *out_pRect)=0;
	virtual int GetBuffer(unsigned char * pBuffer,int width,int height,int pitch,int * len,int index)=0;
};

#ifdef WIN32
class CRocImagePlugsU:public CRocImagePlugs
{
public:
	virtual int Open(LPCWSTR filename)=0;
};
#endif


class ROCHREASUTCLASS CRocPlugsFactory
{
public:
	CRocImagePlugs *CreateImagesPlugs();
#ifdef WIN32
	CRocImagePlugsU *CreateImagesPlugsU();

#endif // WIN32

	void FreeImagesPlugs(CRocImagePlugs * pDel);


};






#endif