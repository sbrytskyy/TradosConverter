#pragma once

#include <iostream>
#include <Windows.h>


class CUtil
{
public:
	CUtil();
	~CUtil();

	static std::string CUtil::utf8_encode(const std::wstring &wstr);
	static std::wstring CUtil::utf8_decode(const std::string &str);
};

