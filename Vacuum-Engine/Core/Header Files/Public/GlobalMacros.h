#pragma once
//#include <string>

#define PRINTF(VALUE, ...) Vacuum::Printf(VALUE, __VA_ARGS__)
//size_t bufSize = snprintf(nullptr, 0, TEXT(VALUE), __VA_ARGS__)\
//wchar_t* buf = nullptr;\
//swprintf_s(buf, bufSize, TEXT(VALUE), __VA_ARGS__) \
//return std::wstring(buf, bufSize) \

//constexpr wchar_t* printf(const wchar_t* _string, ...)
//{
//	va_list args
//	size_t bufSize = _snwprintf(nullptr, 0, _string);
//	wchar_t* buf = nullptr;
//	swprintf_s(buf, bufSize, _string);
//	return std::wstring(buf, bufSize);
//
//}

#define WIDE1(x) L##x
#define WIDE2(x) WIDE1(x)
#define __WFUNCTION__ WIDE2(__FUNCTION__)