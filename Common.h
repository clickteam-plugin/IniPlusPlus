/* Common.h
 * This is the common header file included
 * by almost all other *.cpp files in
 * the project. Anything that gets put or
 * included in here will be available to you
 * everywhere else. You shouldn't need to
 * change anything in this file except
 * to include headers that you want and
 * to change the edition of MMF2 you want
 * to support.
 */

#pragma once

//Only define one of these at once:
//	#define TGFEXT	//TGF2.0, MMF2.0, CTF2.5, MMF2.0 Dev, CTF2.5 Dev
	#define MMFEXT	//        MMF2.0, CTF2.5, MMF2.0 Dev, CTF2.5 Dev
//	#define PROEXT	//                        MMF2.0 Dev, CTF2.5 Dev

#include "Edif.h"
#include "Resource.h"

#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <tuple>
//#include <experimental/filesystem>
#include <memory>
#include <deque>
#include <stdexcept>
#include <type_traits>
#include <cstdint>
#include <codecvt>
#include <iterator>
#include <fstream>

#include <Shlwapi.h>
#include <CommCtrl.h>

/* stdtstring
 * A std::string that knows if you're using
 * unicode or not. (Protip: strings passed
 * to your extension A/C/Es still need to be
 * const TCHAR *, and the same goes for strings
 * returned from expressions and conditions).
 */
typedef std::basic_string<TCHAR> stdtstring;

template<typename T>
T cast(stdtstring const &value)
{
	std::basic_istringstream<TCHAR> iss (value);
	T t = T();
	iss >> t;
	return t;
}
template<typename T>
stdtstring to_tstring(T t)
{
	std::basic_ostringstream<TCHAR> oss;
	oss << t;
	return oss.str();
}

inline stdtstring lowercase(stdtstring const &s)
{
	if(s.empty())
	{
		return s;
	}
	int result = LCMapStringEx(LOCALE_NAME_USER_DEFAULT, LCMAP_LOWERCASE, s.c_str(), s.length(), NULL, 0, NULL, NULL, 0);
	if(result > 0)
	{
		std::unique_ptr<TCHAR[]> buf {new TCHAR[result + 1]()};
		if(LCMapStringEx(LOCALE_NAME_USER_DEFAULT, LCMAP_LOWERCASE, s.c_str(), s.length(), buf.get(), result, NULL, NULL, 0) > 0)
		{
			return stdtstring{buf.get(), static_cast<std::size_t>(result)};
		}
	}
	throw std::runtime_error{"Failed to lowercase a string"};
}

inline std::string utf8from16(std::wstring const &s)
{
	if(s.empty())
	{
		return {};
	}
	int r1 = WideCharToMultiByte
	(
		CP_UTF8,
		0,
		s.c_str(),
		s.size(),
		NULL,
		0,
		NULL,
		NULL
	);
	if(r1 == 0)
	{
		return {};
	}
	std::unique_ptr<std::string::value_type[]> buf {new std::string::value_type[r1]};
	int r2 = WideCharToMultiByte
	(
		CP_UTF8,
		0,
		s.c_str(),
		s.size(),
		buf.get(),
		r1,
		NULL,
		NULL
	);
	if(r2 == 0)
	{
		return {};
	}
	return std::string(buf.get(), r2);
}
inline std::wstring utf16from8(std::string const &s)
{
	if(s.empty())
	{
		return {};
	}
	int r1 = MultiByteToWideChar
	(
		CP_UTF8,
		0,
		s.c_str(),
		s.size(),
		NULL,
		0
	);
	if(r1 == 0)
	{
		return {};
	}
	std::unique_ptr<std::wstring::value_type[]> buf {new std::wstring::value_type[r1]};
	int r2 = MultiByteToWideChar
	(
		CP_UTF8,
		0,
		s.c_str(),
		s.size(),
		buf.get(),
		r1
	);
	if(r2 == 0)
	{
		return {};
	}
	return std::wstring(buf.get(), r2);
}

#include "EditData.hpp"
#include "Extension.hpp"
