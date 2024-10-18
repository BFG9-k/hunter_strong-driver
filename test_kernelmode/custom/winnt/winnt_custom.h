#pragma once
#include <ntifs.h>
#include <ntstrsafe.h>
#include <windef.h>
#include "winnt_custom_defs.h"

struct winnt_data_t
{
	HANDLE drv_handle;
	USHORT name_length;
	WCHAR name_buffer[100];
	PDRIVER_OBJECT drv_obj;
	OBJECT_ATTRIBUTES obj_attribs;
	UNICODE_STRING local_drv_name;
	UNICODE_STRING service_key_name;
	NTSTATUS status = STATUS_SUCCESS;
	ULONG obj_size = sizeof(DRIVER_OBJECT) + sizeof(custom_winnt::EXTENDED_DRIVER_EXTENSION);
};

class c_custom_winnt
{
public:
	NTSTATUS __fastcall IoCreateDriver(_In_ NTSTATUS(__fastcall* EntryPoint)(_In_ PDRIVER_OBJECT, _In_ PUNICODE_STRING));
};

namespace custom_winnt
{
	inline winnt_data_t g_winnt_data;
	inline c_custom_winnt g_winnt;
	NTSTATUS NTAPI IopInvalidDeviceRequest(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);
}