#pragma once
#include "../utils/drv_utils.h"

namespace drv_related
{

	inline c_drv_utl g_utls;
	inline c_drv_request g_requests;

	inline UNICODE_STRING drv_name = {};
	inline UNICODE_STRING drv_device_name = {};
	inline UNICODE_STRING drv_symlink_name = {};

	inline PDEVICE_OBJECT drv_device_object = nullptr;

	NTSTATUS gasfapster(PDRIVER_OBJECT drv_obj, PUNICODE_STRING reg_path);
	inline NTSTATUS drv_status = {};
	inline NTSTATUS symlink_status = {};

	namespace drv_functions
	{
		inline NTSTATUS ctl_status = STATUS_UNSUCCESSFUL;

		NTSTATUS create(PDEVICE_OBJECT device_obj, PIRP irp);
		NTSTATUS close(PDEVICE_OBJECT device_obj, PIRP irp);
		NTSTATUS device_ctl(PDEVICE_OBJECT device_obj, PIRP irp);
	};

	namespace ctl_codes
	{
		constexpr unsigned long attach_code = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
		constexpr unsigned long read_code = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x697, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
		constexpr unsigned long write_code = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x698, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
	}


};