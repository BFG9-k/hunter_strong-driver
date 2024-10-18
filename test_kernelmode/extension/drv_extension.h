#pragma once
#include "../../shared_dependencies/custom_stringcrypt.h"
#include "../custom/custom.h"
namespace drv_extension
{
	extern "C"
	{
		NTKERNELAPI NTSTATUS IoCreateDriver(PUNICODE_STRING DriverName, PDRIVER_INITIALIZE InitFunction);
		NTKERNELAPI NTSTATUS MmCopyVirtualMemory(PEPROCESS SourceProcess, void* SourceAddr, PEPROCESS TargetProcess, void* TargetAddr, SIZE_T BufSize, KPROCESSOR_MODE PrevMode, PSIZE_T RetSize);
	}

}
