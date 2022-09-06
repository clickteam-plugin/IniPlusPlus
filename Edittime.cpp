#include "EditData.hpp"
#include "Resources.h"

#include "FusionAPI.hpp"
#include "lSDK.hpp"
#include "lSDK/WindowsUtilities.hpp"

#include <array>
#include <bit>
#include <cassert>
#include <span>
#include <type_traits>
#include <vector>

#ifdef FUSION_GET_EXTENSION_METADATA
void FUSION_API GetObjInfos(mv *const mV, SerializedEditData *serialized_edit_data,
	fusion::string_buffer const extension_name_buffer/*255*/,
	fusion::string_buffer const extension_author_buffer/*255*/,
	fusion::string_buffer const extension_copyright_buffer/*255*/,
	fusion::string_buffer const extension_description_buffer/*1024*/,
	fusion::string_buffer const extension_website_buffer/*255*/
) noexcept
{
	lSDK::fill_buffer(extension_name_buffer       , 255 , lSDK::get_resource_string(RIPP_NAME));
	lSDK::fill_buffer(extension_author_buffer     , 255 , lSDK::get_resource_string(RIPP_AUTHOR));
	lSDK::fill_buffer(extension_copyright_buffer  , 255 , lSDK::get_resource_string(RIPP_COPYRIGHT));
	lSDK::fill_buffer(extension_description_buffer, 1024, lSDK::get_resource_string(RIPP_DESCRIPTION));
	lSDK::fill_buffer(extension_website_buffer    , 255 , lSDK::get_resource_string(RIPP_WEBSITE));
}
#endif

#ifdef FUSION_GET_HELP_FILE
auto FUSION_API GetHelpFileName() noexcept
-> fusion::string_view
{
	static lSDK::string_t help_path;
	help_path.assign(lSDK::get_resource_string(RIPP_HELP));
	return help_path.c_str();
}
#endif

#ifndef FUSION_RUNTIME_ONLY
template<typename T>
void write_int(std::vector<std::byte>& buf, std::common_type_t<T> const t)
{
	auto const bytes{std::bit_cast<std::array<std::byte, sizeof(T)>>(t)};
	for(std::byte const b : bytes)
	{
		buf.emplace_back(b);
	}
}
void write_str(std::vector<std::byte>& buf, lSDK::string_view_t const s)
{
	buf.reserve(std::size(buf) + (std::size(s) + 1)*sizeof(lSDK::char_t));
	for(lSDK::char_t const c : s)
	{
		write_int<lSDK::char_t>(buf, c);
	}
	write_int<lSDK::char_t>(buf, {});
}
void SerializedEditData::serialize(mv *const mV, SerializedEditData *&serialized_edit_data, EdittimeSettings const &settings)
{
	std::vector<std::byte> raw_buf;
	write_int<std::uint8_t>(raw_buf, settings.b_defaultFile);
	write_int<std::uint8_t>(raw_buf, settings.b_ReadOnly);
	write_str              (raw_buf, settings.defaultFile);
	write_int<std::int32_t>(raw_buf, settings.defaultFolder);
	write_str              (raw_buf, settings.defaultText);
	write_int<std::uint8_t>(raw_buf, settings.bool_CanCreateFolders);
	write_int<std::uint8_t>(raw_buf, settings.bool_AutoSave);
	write_int<std::uint8_t>(raw_buf, settings.bool_stdINI);
	write_int<std::uint8_t>(raw_buf, settings.bool_compress);
	write_int<std::uint8_t>(raw_buf, settings.bool_encrypt);
	write_str              (raw_buf, settings.encrypt_key);
	write_int<std::uint8_t>(raw_buf, settings.bool_newline);
	write_str              (raw_buf, settings.newline);
	write_int<std::uint8_t>(raw_buf, settings.bool_QuoteStrings);
	write_int<std::int32_t>(raw_buf, settings.repeatGroups);
	write_int<std:: int8_t>(raw_buf, settings.repeatItems);
	write_int<std:: int8_t>(raw_buf, settings.undoCount);
	write_int<std:: int8_t>(raw_buf, settings.redoCount);
	write_int<std::uint8_t>(raw_buf, settings.saveRepeated);
	write_int<std::uint8_t>(raw_buf, settings.bool_EscapeGroup);
	write_int<std::uint8_t>(raw_buf, settings.bool_EscapeItem);
	write_int<std::uint8_t>(raw_buf, settings.bool_EscapeValue);
	write_int<std::uint8_t>(raw_buf, settings.bool_CaseSensitive);
	write_int<std::uint8_t>(raw_buf, settings.globalObject);
	write_int<std::uint8_t>(raw_buf, settings.index);
	write_int<std::uint8_t>(raw_buf, settings.autoLoad);
	write_int<std::uint8_t>(raw_buf, settings.subGroups);
	write_int<std::uint8_t>(raw_buf, settings.allowEmptyGroups);
	write_str              (raw_buf, settings.globalKey);

	std::size_t const new_size{sizeof(SerializedEditData) + std::size(raw_buf) - sizeof(raw)};
	if(auto const new_sed{static_cast<SerializedEditData*>(mvReAllocEditData(mV, serialized_edit_data, static_cast<DWORD>(new_size)))})
	{
		assert(new_sed->eHeader.extSize == new_size);
		serialized_edit_data = new_sed;
		std::ignore = std::memcpy(std::data(serialized_edit_data->raw), std::data(raw_buf), std::size(raw_buf));
		serialized_edit_data->eHeader.extVersion = CURRENT_VERSION;
	}
	else
	{
		throw std::bad_alloc{};
	}
}
#endif
struct EndOfEditdata{};
struct UnknownEditdata{};
template<typename T>
[[nodiscard]] static auto read_int(std::span<std::byte const>& s)
-> T
{
	if(std::size(s) < sizeof(T))
	{
		throw EndOfEditdata{};
	}
	T t{};
	std::ignore = std::memcpy(&t, std::data(s), sizeof(T));
	s = s.subspan<sizeof(T)>();
	return t;
}
template<typename T>
[[nodiscard]] static auto read_bool(std::span<std::byte const>& s)
-> T
{
	switch(T const t{read_int<T>(s)})
	{
		case 0: return false;
		case 1: return true;
		default: throw UnknownEditdata{};
	}
}
template<std::size_t Len>
[[nodiscard]] static auto read_str_v1(std::span<std::byte const>& s, std::optional<std::uint32_t> const codepage) //ANSI fixed array
-> lSDK::string_t
{
	if(std::size(s) < Len)
	{
		throw EndOfEditdata{};
	}
	lSDK::string_view8_t const ret(reinterpret_cast<lSDK::string_view8_t::value_type const*>(std::data(s)), static_cast<std::size_t>(Len));
	s = s.subspan(Len);
	return lSDK::wide_from_narrow(ret.substr(0, ret.find('\0')), codepage);
}
[[nodiscard]] static auto read_str_v2(std::span<std::byte const>& s) //Unicode null-terminated
-> lSDK::string_view_t
{
	lSDK::string_view_t ret(reinterpret_cast<lSDK::char_t const*>(std::data(s)), std::size(s)/sizeof(lSDK::char_t));
	ret = ret.substr(0, ret.find(TSL('\0')));
	s = s.subspan((std::size(ret) + 1)*sizeof(lSDK::char_t));
	return ret;
}
auto SerializedEditData::deserialize(std::optional<std::uint32_t> const codepage) const
-> EdittimeSettings
{
	static auto const report_unknown_version{[]
	{
		assert(false);
		return (void)MessageBox(
			NULL,
			_T("This MFA was saved with an unkown version of the Ini++ Object. Properties will be reset.\nNote that some actions, conditions, or expressions may not be valid and can cause Fusion to crash."),
			_T("Warning - Ini++"),
			MB_OK
		);
	}};
	EdittimeSettings settings;
	if(eHeader.extSize < sizeof(SerializedEditData)
	|| eHeader.extVersion < INIPP_V1_ANSI
	|| eHeader.extVersion > INIPP_V2_UNICODE)
	{
		report_unknown_version();
		return settings;
	}
	std::span<std::byte const> raw_span(raw, static_cast<std::size_t>(eHeader.extSize) - offsetof(SerializedEditData, raw));
	try
	{
		settings.b_defaultFile = read_bool<std::uint8_t>(raw_span);
		settings.b_ReadOnly    = read_bool<std::uint8_t>(raw_span);
		if(eHeader.extVersion == INIPP_V1_ANSI){ settings.defaultFile = read_str_v1<260>(raw_span, codepage); }
		else                                   { settings.defaultFile = read_str_v2(raw_span); }
		if(eHeader.extVersion == INIPP_V1_ANSI){ raw_span = raw_span.subspan(sizeof(std::byte)*2); } //unused aligment
		settings.defaultFolder = read_int<std::int32_t>(raw_span);
		if(eHeader.extVersion == INIPP_V1_ANSI){ settings.defaultText = read_str_v1<3000>(raw_span, codepage); }
		else                                   { settings.defaultText = read_str_v2(raw_span); }
		settings.bool_CanCreateFolders = read_bool<std::uint8_t>(raw_span);
		settings.bool_AutoSave         = read_bool<std::uint8_t>(raw_span);
		settings.bool_stdINI           = read_bool<std::uint8_t>(raw_span);
		settings.bool_compress         = read_bool<std::uint8_t>(raw_span);
		settings.bool_encrypt          = read_bool<std::uint8_t>(raw_span);
		if(eHeader.extVersion == INIPP_V1_ANSI){ settings.encrypt_key = read_str_v1<128>(raw_span, codepage); }
		else                                   { settings.encrypt_key = read_str_v2(raw_span); }
		settings.bool_newline = read_bool<std::uint8_t>(raw_span);
		if(eHeader.extVersion == INIPP_V1_ANSI){ settings.newline = read_str_v1<10>(raw_span, codepage); }
		else                                   { settings.newline = read_str_v2(raw_span); }
		settings.bool_QuoteStrings = read_bool<std::uint8_t>(raw_span);
		if(eHeader.extVersion == INIPP_V1_ANSI){ raw_span = raw_span.subspan(sizeof(std::byte)*3); } //unused aligment
		settings.repeatGroups = read_int<std::int32_t>(raw_span);
		settings.repeatItems  = read_int<std::int8_t>(raw_span);
		settings.undoCount    = read_int<std::int8_t>(raw_span);
		settings.redoCount    = read_int<std::int8_t>(raw_span);
		if(eHeader.extVersion == INIPP_V1_ANSI){ std::ignore  = read_int<std::int8_t>(raw_span); } //placeholder, doesn't appear to be used, probably poorly named unused alignment
		settings.saveRepeated       = read_bool<std::uint8_t>(raw_span);
		settings.bool_EscapeGroup   = read_bool<std::uint8_t>(raw_span);
		settings.bool_EscapeItem    = read_bool<std::uint8_t>(raw_span);
		settings.bool_EscapeValue   = read_bool<std::uint8_t>(raw_span);
		settings.bool_CaseSensitive = read_bool<std::uint8_t>(raw_span);
		settings.globalObject       = read_bool<std::uint8_t>(raw_span);
		settings.index              = read_bool<std::uint8_t>(raw_span);
		settings.autoLoad           = read_bool<std::uint8_t>(raw_span);
		settings.subGroups          = read_bool<std::uint8_t>(raw_span);
		settings.allowEmptyGroups   = read_bool<std::uint8_t>(raw_span);
		if(eHeader.extVersion == INIPP_V1_ANSI){ settings.globalKey = read_str_v1<32>(raw_span, codepage); }
		else                                   { settings.globalKey = read_str_v2(raw_span); }
		if(eHeader.extVersion == INIPP_V1_ANSI){ raw_span = raw_span.subspan(sizeof(std::byte)*2); } //unused aligment
		assert(std::empty(raw_span));
	}
	catch(...)
	{
		report_unknown_version();
	}
	return settings;
}

#ifdef FUSION_CREATE_OBJECT
auto FUSION_API CreateObject(mv* const mV, LO* const level_object, SerializedEditData* serialized_edit_data) noexcept
-> std::int32_t
{
	try
	{
		SerializedEditData::serialize(mV, serialized_edit_data, {});
	}
	catch(...)
	{
		return FUSION_CREATE_OBJECT_FAILURE;
	}
	return FUSION_CREATE_OBJECT_SUCCESS;
}
#endif
