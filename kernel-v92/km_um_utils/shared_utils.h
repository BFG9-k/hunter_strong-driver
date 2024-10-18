#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include "memory"
#include <string>
#include "../../shared_dependencies/custom_stringcrypt.h"
namespace ctl_codes
{
	constexpr unsigned long attach_code = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
	constexpr unsigned long read_code = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x697, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
	constexpr unsigned long write_code = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x698, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
}

namespace usermode
{
	
	inline const char* target_process = "hl2.exe";

	inline DWORD target_pid = 0;

	class c_drv_request
	{
	public:
		HANDLE proc_id;
		void* target;
		void* buffer;
		SIZE_T size;
		SIZE_T return_size;
	};
	inline c_drv_request g_drv_req;
	class c_shared
	{
	public:
		bool process_attach(HANDLE drv_handle, const DWORD pid);

		DWORD get_pid(const wchar_t* process_name);

		std::uintptr_t get_base(const DWORD pid, const wchar_t* module_name);

		template <class T>
		T mem_read(HANDLE driver_handle, const std::uintptr_t addr);

		template <class T>
		void mem_write(HANDLE driver_handle, const std::uintptr_t addr, const T& value);

		void open_handle();

		HANDLE drv_handle{};
	};

	inline std::shared_ptr<c_shared> g_shared = std::make_shared<c_shared>();
}

namespace strings
{
	inline auto s_001 = skCrypt("(um) was not found\n");
	inline auto s_002 = skCrypt("(um) ++=> attached to the target process\n");
}
