#include "drv_utils.h"


void c_drv_utl::dbg_print(PCSTR txt)
{
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, txt));
}
