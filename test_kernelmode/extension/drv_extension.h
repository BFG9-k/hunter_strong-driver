#pragma once
#include "../../shared_dependencies/custom_stringcrypt.h"
namespace drv_extension
{
	extern "C"
	{
		NTKERNELAPI NTSTATUS IoCreateDriver(PUNICODE_STRING DriverName, PDRIVER_INITIALIZE InitFunction);
		NTKERNELAPI NTSTATUS MmCopyVirtualMemory(PEPROCESS SourceProcess, void* SourceAddr, PEPROCESS TargetProcess, void* TargetAddr, SIZE_T BufSize, KPROCESSOR_MODE PrevMode, PSIZE_T RetSize);
	}

	namespace strings
	{
		/*
		inline auto km_s_001 = skCrypt("++=> created driver device\n");
		inline auto km_s_002 = skCrypt("__=> driver device was not created\n");
		inline auto km_s_003 = skCrypt("__=> symlink was not created\n");
		inline auto km_s_004 = skCrypt("++=> created symlink\n");
		inline auto km_s_005 = skCrypt("++=> driver initialized\n");
		inline auto km_s_006 = skCrypt("++=> entered kernel EP\n");
		*/
	}
}
