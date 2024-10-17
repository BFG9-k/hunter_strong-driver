#include "related\drv_related.h"


NTSTATUS kernel_entry()
{
	
	drv_related::g_utls.dbg_print("++=> entered kernel EP\n"); // "++=> entered kernel EP\n" (drv_extension::strings::km_s_006)
	RtlInitUnicodeString(&drv_related::drv_name, L"\\Driver\\test_drv");



	return drv_extension::IoCreateDriver(&drv_related::drv_name, drv_related::drv_main);
}