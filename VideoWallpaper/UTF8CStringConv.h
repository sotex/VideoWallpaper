#pragma once
#include <string>
#include <afx.h>

namespace UTF8CStringConv
{
	std::string ConvertCStringToUTF8(CString strValue);
	CString ConvertUTF8ToCString(std::string utf8str);
}

