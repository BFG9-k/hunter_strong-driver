#include "drv_related.h"

namespace drv_related
{

	NTSTATUS drv_main(PDRIVER_OBJECT drv_obj, PUNICODE_STRING reg_path)
	{
		RtlInitUnicodeString(&drv_related::drv_device_name, L"\\Device\\test_drv");

		drv_related::g_utls.dbg_print("(km) **=> an attempt to create driver device \n");
		
		drv_related::drv_status = IoCreateDevice(drv_obj, NULL, &drv_related::drv_device_name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &drv_related::drv_device_object);

		if (drv_related::drv_status != STATUS_SUCCESS)
		{
			drv_related::g_utls.dbg_print("(km) --=> driver device was not created\n"); //"__=> driver device was not created\n" / drv_extension::strings::km_s_002
			return drv_related::drv_status;
		}

		drv_related::g_utls.dbg_print("(km) ++=> created driver device \n"); // (drv_extension::strings::km_s_001

		RtlInitUnicodeString(&drv_related::drv_symlink_name, L"\\DosDevices\\test_drv");

		drv_related::g_utls.dbg_print("(km) **=> an attempt to create symlink \n");

		drv_related::symlink_status = IoCreateSymbolicLink(&drv_related::drv_symlink_name, &drv_related::drv_device_name);

		if (drv_related::symlink_status != STATUS_SUCCESS)
		{
			drv_related::g_utls.dbg_print("(km) --=> symlink was not created\n"); //"__=> symlink was not created\n" drv_extension::strings::km_s_003
			return drv_related::symlink_status;
		}

		drv_related::g_utls.dbg_print("(km) ++=> created symlink \n"); //"++=> created symlink\n" drv_extension::strings::km_s_004
		
		SetFlag(drv_related::drv_device_object->Flags, DO_BUFFERED_IO);
		
		drv_obj->MajorFunction[IRP_MJ_CREATE] = drv_related::drv_functions::create;
		drv_obj->MajorFunction[IRP_MJ_CLOSE] = drv_related::drv_functions::close;
		drv_obj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = drv_related::drv_functions::device_ctl;

		ClearFlag(drv_related::drv_device_object->Flags, DO_DEVICE_INITIALIZING);


		drv_related::g_utls.dbg_print("(km) ++=> driver initialized\n"); //"++=> driver initialized\n" drv_extension::strings::km_s_005
		return STATUS_SUCCESS;
	}
}