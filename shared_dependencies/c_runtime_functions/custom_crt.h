#pragma once
class c_custom_crt
{
public:
	const void* memchr(const void* s, int c, size_t n);
	int memcmp(const void* s1, const void* s2, size_t n);
	void* memcpy(void* dest, const void* src, size_t count);
	void* memmove(void* destination, const void* source, size_t size);
	void* memset(void* src, int val, size_t count);

	int tolower(int c);
	int stricmp(const char* cs, const char* ct);
	int strcmp(const char* cs, const char* ct);
	size_t strlen(const char* const string);
	char* strstr(const char* _Str, char const* _SubStr);
};

