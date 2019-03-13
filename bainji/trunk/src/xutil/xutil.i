%module(directors="1") xutil

%include <std_string.i>
%include <swiginterface.i>
        
%{
#include "xutil/id.h"
#include "xutil/InputFileStream.h"
#include "xutil/OutputFileStream.h"
#include "xutil/IDictionary.h"
#include "xutil/Image.h"
%}

%include "xutil/id.h"
%include "xutil/InputFileStream.h"
%include "xutil/OutputFileStream.h"
%include "xutil/IDictionary.h"
%include "xutil/Image.h"