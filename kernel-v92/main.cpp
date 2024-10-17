#include "km_um_utils/shared_utils.h"
int main()
{
	usermode::target_pid = usermode::g_shared->get_pid(L"notepad.exe");

	if (usermode::target_pid == 0)
	{
		std::cerr << usermode::target_process << strings::s_001;
		std::cin.get();
		return 1;
	}

	usermode::g_shared->open_handle();

	if (usermode::g_shared->process_attach(usermode::g_shared->drv_handle, usermode::target_pid) == true)
	{
		std::cout << strings::s_002;
		std::cin.get();
		return 0;
	}
	
}