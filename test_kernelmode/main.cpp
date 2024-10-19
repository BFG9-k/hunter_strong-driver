#include "related\drv_related.h"


NTSTATUS DriverEntry()
{
	return drv_extension::IoCreateDriver(&drv_related::drv_name, drv_related::gasfapster);
}