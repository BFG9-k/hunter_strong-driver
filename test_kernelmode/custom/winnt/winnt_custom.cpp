#include "winnt_custom.h"

NTSTATUS NTAPI custom_winnt::IopInvalidDeviceRequest(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp)
{
	Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_INVALID_DEVICE_REQUEST;
}

NTSTATUS __fastcall c_custom_winnt::IoCreateDriver(_In_ NTSTATUS(__fastcall* EntryPoint)(_In_ PDRIVER_OBJECT, _In_ PUNICODE_STRING))
{
	custom_winnt::g_winnt_data.name_length = (USHORT)swprintf(custom_winnt::g_winnt_data.name_buffer, L"\\Driver\\test_drv");
	custom_winnt::g_winnt_data.local_drv_name.Length = custom_winnt::g_winnt_data.name_length * sizeof(WCHAR);
	custom_winnt::g_winnt_data.local_drv_name.MaximumLength = custom_winnt::g_winnt_data.local_drv_name.Length + sizeof(UNICODE_NULL);
	custom_winnt::g_winnt_data.local_drv_name.Buffer = custom_winnt::g_winnt_data.name_buffer;

	InitializeObjectAttributes(&custom_winnt::g_winnt_data.obj_attribs, &custom_winnt::g_winnt_data.local_drv_name, OBJ_PERMANENT | OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	custom_winnt::g_winnt_data.status = custom_winnt::ObCreateObject(KernelMode, custom_winnt::IoDriverObjectType, &custom_winnt::g_winnt_data.obj_attribs, KernelMode, NULL, custom_winnt::g_winnt_data.obj_size, 0, 0, (PVOID*)&custom_winnt::g_winnt_data.drv_obj);
	if (!NT_SUCCESS(custom_winnt::g_winnt_data.status))
		return custom_winnt::g_winnt_data.status;

	RtlZeroMemory(custom_winnt::g_winnt_data.drv_obj, custom_winnt::g_winnt_data.obj_size);
	custom_winnt::g_winnt_data.drv_obj->Type = IO_TYPE_DRIVER;
	custom_winnt::g_winnt_data.drv_obj->Size = sizeof(DRIVER_OBJECT);
	custom_winnt::g_winnt_data.drv_obj->Flags = DRVO_BUILTIN_DRIVER;		
	custom_winnt::g_winnt_data.drv_obj->DriverExtension = (PDRIVER_EXTENSION)(custom_winnt::g_winnt_data.drv_obj + 1);
	custom_winnt::g_winnt_data.drv_obj->DriverExtension->DriverObject = custom_winnt::g_winnt_data.drv_obj;
	custom_winnt::g_winnt_data.drv_obj->DriverInit = EntryPoint;

	for (int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		custom_winnt::g_winnt_data.drv_obj->MajorFunction[i] = custom_winnt::IopInvalidDeviceRequest;
	}

	custom_winnt::g_winnt_data.service_key_name.MaximumLength = custom_winnt::g_winnt_data.local_drv_name.Length + sizeof(UNICODE_NULL);
	custom_winnt::g_winnt_data.service_key_name.Buffer = (PWCH)ExAllocatePool2(POOL_FLAG_PAGED, custom_winnt::g_winnt_data.local_drv_name.MaximumLength, (ULONG)KeQueryUnbiasedInterruptTime());

	if (!custom_winnt::g_winnt_data.service_key_name.Buffer)
	{
		ObMakeTemporaryObject(custom_winnt::g_winnt_data.drv_obj);
		ObfDereferenceObject(custom_winnt::g_winnt_data.drv_obj);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	RtlCopyUnicodeString(&custom_winnt::g_winnt_data.service_key_name, &custom_winnt::g_winnt_data.local_drv_name);

	custom_winnt::g_winnt_data.service_key_name.Buffer[custom_winnt::g_winnt_data.service_key_name.Length / sizeof(WCHAR)] = UNICODE_NULL;
	custom_winnt::g_winnt_data.drv_obj->DriverExtension->ServiceKeyName = custom_winnt::g_winnt_data.service_key_name;
	custom_winnt::g_winnt_data.drv_obj->DriverName.MaximumLength = custom_winnt::g_winnt_data.local_drv_name.Length;
	custom_winnt::g_winnt_data.drv_obj->DriverName.Buffer = (PWCH)ExAllocatePool2(POOL_FLAG_PAGED, custom_winnt::g_winnt_data.drv_obj->DriverName.MaximumLength, (ULONG)KeQueryUnbiasedInterruptTime());
	if (!custom_winnt::g_winnt_data.drv_obj->DriverName.Buffer)
	{
		ObMakeTemporaryObject(custom_winnt::g_winnt_data.drv_obj);
		ObfDereferenceObject(custom_winnt::g_winnt_data.drv_obj);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	RtlCopyUnicodeString(&custom_winnt::g_winnt_data.drv_obj->DriverName, &custom_winnt::g_winnt_data.local_drv_name);

	custom_winnt::g_winnt_data.status = ObInsertObject(custom_winnt::g_winnt_data.drv_obj, NULL, FILE_READ_DATA, 0, NULL, &custom_winnt::g_winnt_data.drv_handle);
	ZwClose(custom_winnt::g_winnt_data.drv_handle);
	if (!NT_SUCCESS(custom_winnt::g_winnt_data.status))
	{
		ObMakeTemporaryObject(custom_winnt::g_winnt_data.drv_obj);
		ObfDereferenceObject(custom_winnt::g_winnt_data.drv_obj);
		return custom_winnt::g_winnt_data.status;
	}

	custom_winnt::g_winnt_data.status = EntryPoint(custom_winnt::g_winnt_data.drv_obj, NULL);
	if (!NT_SUCCESS(custom_winnt::g_winnt_data.status))
	{
		ObMakeTemporaryObject(custom_winnt::g_winnt_data.drv_obj);
		ObDereferenceObject(custom_winnt::g_winnt_data.drv_obj);
		return custom_winnt::g_winnt_data.status;
	}

	for (int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		if (!custom_winnt::g_winnt_data.drv_obj->MajorFunction[i])
		{
			custom_winnt::g_winnt_data.drv_obj->MajorFunction[i] = custom_winnt::IopInvalidDeviceRequest;
		}
	}

	return custom_winnt::g_winnt_data.status; // If everything went correctly this will return the driver's result
}