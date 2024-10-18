#include "drv_related.h"


NTSTATUS drv_related::drv_functions::create(PDEVICE_OBJECT device_obj, PIRP irp)
{
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

NTSTATUS drv_related::drv_functions::close(PDEVICE_OBJECT device_obj, PIRP irp)
{
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

NTSTATUS drv_related::drv_functions::device_ctl(PDEVICE_OBJECT device_obj, PIRP irp)
{
	drv_related::g_utls.dbg_print("(km) ++=> waiting for device ctl codes...\n"); // "++=> created driver device\n" / drv_extension::strings::km_s_001

	PIO_STACK_LOCATION stack_location = IoGetCurrentIrpStackLocation(irp);
	c_drv_request* req = (c_drv_request*)(irp->AssociatedIrp.SystemBuffer);

	if (stack_location == NULL || req == NULL)
	{
		IoCompleteRequest(irp, IO_NO_INCREMENT);
		return drv_related::drv_functions::ctl_status;
	}
	
	static PEPROCESS target_proc = NULL;
	const unsigned long ctl_code = stack_location->Parameters.DeviceIoControl.IoControlCode;

	switch (ctl_code)
	{
		case drv_related::ctl_codes::attach_code:
			drv_related::g_utls.dbg_print("(km) ++=> received attach ctl code\n");
			drv_related::drv_functions::ctl_status = PsLookupProcessByProcessId(req->proc_id, &target_proc);
			break;

		case drv_related::ctl_codes::read_code:
			if (target_proc != NULL)
			drv_related::g_utls.dbg_print("(km) ++=> received read ctl code\n");
			drv_related::drv_functions::ctl_status = drv_extension::MmCopyVirtualMemory(target_proc, req->target, PsGetCurrentProcess(), req->buffer, req->size, KernelMode, &req->return_size);
			break;

		case drv_related::ctl_codes::write_code:
			if (target_proc != NULL)
			drv_related::g_utls.dbg_print("(km) ++=> received write ctl code\n");
			drv_related::drv_functions::ctl_status = drv_extension::MmCopyVirtualMemory(PsGetCurrentProcess(), req->buffer, target_proc, req->target, req->size, KernelMode, &req->return_size);
			break;

		default:
			break;
	}

	irp->IoStatus.Status = drv_related::drv_functions::ctl_status;
	irp->IoStatus.Information = sizeof(c_drv_request);

	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return drv_related::drv_functions::ctl_status;
}

