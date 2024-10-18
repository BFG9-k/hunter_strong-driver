#include "shared_utils.h"

bool usermode::c_shared::process_attach(HANDLE drv_handle, const DWORD pid)
{
	c_drv_request requests;
	requests.proc_id = (HANDLE)(pid);
	return DeviceIoControl(drv_handle, ctl_codes::attach_code, &requests, sizeof(requests), &requests, sizeof(requests), NULL, NULL);
}

DWORD usermode::c_shared::get_pid(const wchar_t* process_name)
{
	DWORD process_id = 0;

	HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (snap_shot == INVALID_HANDLE_VALUE)
		return process_id;

	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(decltype(entry));

	if (Process32FirstW(snap_shot, &entry) == TRUE)
	{
		if (_wcsicmp(process_name, entry.szExeFile) == 0)
			process_id = entry.th32ProcessID;
		else {
			while (Process32NextW(snap_shot, &entry) == TRUE)
			{
				if (_wcsicmp(process_name, entry.szExeFile) == 0)
				{
					process_id = entry.th32ProcessID;
					break;
				}
			}
		}
	}

	CloseHandle(snap_shot);

	return process_id;
}

std::uintptr_t usermode::c_shared::get_base(const DWORD pid, const wchar_t* module_name)
{
	std::uintptr_t module_base = 0;

	HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (snap_shot == INVALID_HANDLE_VALUE)
		return module_base;

	MODULEENTRY32 entry = {};
	entry.dwSize = sizeof(decltype(entry));

	if (Module32FirstW(snap_shot, &entry) == TRUE)
	{
		if (wcsstr(module_name, entry.szModule) != nullptr)
			module_base = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
		else
		{
			while (Module32NextW(snap_shot, &entry) == TRUE)
			{
				if (wcsstr(module_name, entry.szModule) != nullptr)
				{
					module_base = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
					break;
				}
			}
		}
	}

	CloseHandle(snap_shot);

	return module_base;
}

template <class T>
T usermode::c_shared::mem_read(HANDLE driver_handle, const std::uintptr_t addr)
{
	T temp = {};

	c_drv_request requests;
	requests.target = (void*)(addr);
	requests.buffer = &temp;
	requests.size = sizeof(T);

	DeviceIoControl(driver_handle, ctl_codes::read_code, &requests, sizeof(requests), &requests, sizeof(requests), nullptr, nullptr);

	return temp;
}

template <class T>
void usermode::c_shared::mem_write(HANDLE driver_handle, const std::uintptr_t addr, const T& value)
{
	c_drv_request requests;
	requests.target = (void*)(addr);
	requests.buffer = (void*)&value;
	requests.size = sizeof(T);

	DeviceIoControl(driver_handle, ctl_codes::write_code, &requests, sizeof(requests), &requests, sizeof(requests), nullptr, nullptr);
}

void usermode::c_shared::open_handle()
{
	usermode::g_shared->drv_handle = CreateFile(L"\\\\.\\test_drv", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (usermode::g_shared->drv_handle == INVALID_HANDLE_VALUE)
	{
		std::cout << "(um) --=> unable to open driver handle\n";
		std::cin.get();
	}
}