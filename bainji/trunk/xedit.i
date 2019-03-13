%module(directors="1") xedit

#define __ANDROID__ 1
#define API_IMPORT_EXPORT

%include "avpub.i"

%{
#include "GSize.h"
#include "IBuffer.h"
#include "IRenderer.h"
//#include "IDeserializable.h"
#include "IInputStream.h"
#include "IObserver.h"
#include "IObservable.h"
#include "IOutputStream.h"
//#include "ISerializable.h"
//#include "IXmlDeserializable.h"
//#include "IXmlSerializable.h"
%}

%feature("director") xedit::IBuffer;
%feature("nodirector") xedit::IBuffer::~IBuffer;

%feature("director") xedit::IVideoBuffer;
%feature("nodirector") xedit::IVideoBuffer::~IVideoBuffer;

%feature("director") xedit::IAudioBuffer;
%feature("nodirector") xedit::IAudioBuffer::~IAudioBuffer;

%feature("director") xedit::IRenderer;
%feature("nodirector") xedit::IRenderer::~IRenderer;

%feature("director") xedit::IAudioRenderer;
%feature("nodirector") xedit::IAudioRenderer::~IAudioRenderer;

%feature("director") xedit::IVideoRenderer;
%feature("nodirector") xedit::IVideoRenderer::~IVideoRenderer;

%feature("director") xedit::IDeserializable;
%feature("nodirector") xedit::IDeserializable::~IDeserializable;

%feature("director") xedit::IInputStream;
%feature("nodirector") xedit::IInputStream::~IInputStream;

%feature("director") xedit::IObservable;
%feature("nodirector") xedit::IObservable::~IObservable;

%feature("director") xedit::IObserver;
%feature("nodirector") xedit::IObserver::~IObserver;

%feature("director") xedit::IOutputStream;
%feature("nodirector") xedit::IOutputStream::~IOutputStream;

%feature("director") xedit::ISerializable;
%feature("nodirector") xedit::ISerializable::~ISerializable;

%feature("director") xedit::IXmlDeserializable;
%feature("nodirector") xedit::IXmlDeserializable::~IXmlDeserializable;

%feature("director") xedit::IXmlSerializable;
%feature("nodirector") xedit::IXmlSerializable::~IXmlSerializable;

%include "GSize.h"
%include "IBuffer.h"
%include "IRenderer.h"
//%include "IDeserializable.h"
%include "IInputStream.h"
%include "IObserver.h"
%include "IObservable.h"
%include "IOutputStream.h"
//%include "ISerializable.h"
//%include "IXmlDeserializable.h"
//%include "IXmlSerializable.h"

typedef unsigned char uint8_t;
typedef int uint32_t;
typedef unsigned long long uint64_t;

typedef char int8_t;
typedef unsigned int int32_t;
typedef long long int64_t;

typedef long long ID;

%include "src/xengine/xengine.i"