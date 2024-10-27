#include "custom_crt.h"

const void* c_custom_crt::memchr(const void* s, int c, size_t n)
{
	if (n)
	{
		const char* p = (const char*)s;
		do
		{
			if (*p++ == c) return (void*)(p - 1);
		} while (--n != 0);
	}
	return 0;
}

int c_custom_crt::memcmp(const void* s1, const void* s2, size_t n)
{
	if (n != 0)
	{
		const unsigned char* p1 = (unsigned char*)s1, * p2 = (unsigned char*)s2;
		do
		{
			if (*p1++ != *p2++) return (*--p1 - *--p2);
		} while (--n != 0);
	}
	return 0;
}

void* c_custom_crt::memcpy(void* dest, const void* src, size_t count)
{
	char* char_dest = (char*)dest;
	char* char_src = (char*)src;
	if ((char_dest <= char_src) || (char_dest >= (char_src + count)))
	{
		while (count > 0)
		{
			*char_dest = *char_src;
			char_dest++;
			char_src++;
			count--;
		}
	}
	else
	{
		char_dest = (char*)dest + count - 1;
		char_src = (char*)src + count - 1;
		while (count > 0)
		{
			*char_dest = *char_src;
			char_dest--;
			char_src--;
			count--;
		}
	}
	return dest;
}

void* c_custom_crt::memmove(void* destination, const void* source, size_t size)
{
	char* char_dest = (char*)destination;
	char* char_src = (char*)source;
	if ((char_dest <= char_src) || (char_dest >= (char_src + size)))
	{
		while (size > 0)
		{
			*char_dest = *char_src;
			char_dest++;
			char_src++;
			size--;
		}
	}
	else
	{
		char_dest = (char*)destination + size - 1;
		char_src = (char*)source + size - 1;
		while (size > 0)
		{
			*char_dest = *char_src;
			char_dest--;
			char_src--;
			size--;
		}
	}
	return destination;
}

void* c_custom_crt::memset(void* src, int val, size_t count)
{
	int           i;
	unsigned char* p = (unsigned char*)src;
	i = 0;
	while (count > 0)
	{
		*p = val;
		p++;
		count--;
	}
	return(src);
}



int c_custom_crt::tolower(int c)
{
	if (c >= 'A' && c <= 'Z') return c - 'A' + 'a';
	return c;
}

int c_custom_crt::stricmp(const char* cs, const char* ct)
{
	if (cs && ct)
	{
		while (tolower(*cs) == tolower(*ct))
		{
			if (*cs == 0 && *ct == 0) return 0;
			if (*cs == 0 || *ct == 0) break;
			cs++;
			ct++;
		}
		return tolower(*cs) - tolower(*ct);
	}
	return -1;
}

int c_custom_crt::strcmp(const char* cs, const char* ct)
{
	if (cs && ct)
	{
		while (*cs == *ct)
		{
			if (*cs == 0 && *ct == 0) return 0;
			if (*cs == 0 || *ct == 0) break;
			cs++;
			ct++;
		}
		return *cs - *ct;
	}
	return -1;
}

size_t c_custom_crt::strlen(const char* const string)
{
	size_t length = 0;

	while (string[length] != '\0')
		length++;

	return length;
}

char* c_custom_crt::strstr(const char* _Str, char const* _SubStr)
{
	const char* bp = _SubStr;
	const char* back_pos;
	while (*_Str != 0 && _Str != 0 && _SubStr != 0)
	{
		back_pos = _Str;
		while (tolower(*back_pos++) == tolower(*_SubStr++))
		{
			if (*_SubStr == 0)
			{
				return (char*)(back_pos - strlen(bp));
			}
		}
		++_Str;
		_SubStr = bp;
	}
	return 0;
}