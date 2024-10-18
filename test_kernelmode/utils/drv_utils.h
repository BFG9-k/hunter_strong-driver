#pragma once
#include <ntifs.h>
#include "../extension/drv_extension.h"

class c_drv_utl
{
public:
	void dbg_print(PCSTR txt);
};

class c_drv_request
{
public:
	HANDLE proc_id;
	void* target;
	void* buffer;
	SIZE_T size;
	SIZE_T return_size;
};

