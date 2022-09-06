#pragma once

#include "lSDK.hpp"

#include <cstdint>
#include <filesystem>
#include <optional>

struct RunHeader;
[[nodiscard]] std::filesystem::path get_default_path(std::int32_t type, RunHeader const& rh);

struct EdittimeSettings final
{
	bool b_defaultFile{true};
	bool b_ReadOnly{false};
	lSDK::string_t defaultFile;
	std::int32_t defaultFolder{2};
	lSDK::string_t defaultText;
	bool bool_CanCreateFolders{true};
	bool bool_AutoSave{true};
	bool bool_stdINI{true};
	bool bool_compress{false};
	bool bool_encrypt{false};
	lSDK::string_t encrypt_key;
	bool bool_newline{false};
	lSDK::string_t newline;
	bool bool_QuoteStrings{false};
	std::int32_t repeatGroups{3};
	std::int8_t repeatItems{1};
	std::int8_t undoCount{0};
	std::int8_t redoCount{0};
	bool saveRepeated{false};
	bool bool_EscapeGroup{false};
	bool bool_EscapeItem{false};
	bool bool_EscapeValue{false};
	bool bool_CaseSensitive{false};
	bool globalObject{false};
	bool index{false};
	bool autoLoad{false};
	bool subGroups{false};
	bool allowEmptyGroups{true};
	lSDK::string_t globalKey;
};
enum struct RepeatModeGroup
{
	TakeFirst,
	TakeLast,
	Rename,
	GroupMerge,
};
static constexpr auto get_repeat_mode_group(std::int32_t const m) noexcept
{
	switch(m)
	{
		case 0: return RepeatModeGroup::TakeFirst;
		case 1: return RepeatModeGroup::TakeLast;
		case 2: return RepeatModeGroup::Rename;
		default: [[fallthrough]];
		case 3: return RepeatModeGroup::GroupMerge;
	}
}
enum struct RepeatModeItem
{
	TakeFirst,
	TakeLast,
	Rename,
};
static constexpr auto get_repeat_mode_item(std::int8_t const m) noexcept
{
	switch(m)
	{
		case 0: return RepeatModeItem::TakeFirst;
		default: [[fallthrough]];
		case 1: return RepeatModeItem::TakeLast;
		case 2: return RepeatModeItem::Rename;
	}
}
struct RuntimeSettingsLocal final
{
	std::filesystem::path default_dir;
	lSDK::string_t current_group;
	bool auto_create_dirs, index_1_based, auto_load;

	RuntimeSettingsLocal(EdittimeSettings& s, RunHeader const& rh)
	: default_dir{get_default_path(s.defaultFolder, rh)}
	, auto_create_dirs{s.bool_CanCreateFolders}
	, index_1_based{s.index}
	, auto_load{s.autoLoad}
	{
	}
};
struct RuntimeSettingsGlobal final
{
	std::optional<std::filesystem::path> ini_path;
	bool read_only, auto_save, compress, never_quote, save_repeated, esc_group, esc_item, esc_value, case_sensitive, subgroups, empty_groups;
	std::optional<lSDK::string8_t> encrypt_key;
	std::optional<lSDK::string_t> newline;
	RepeatModeGroup repeat_mode_group;
	RepeatModeItem  repeat_mode_item;
	std::int32_t undos;
	std::int32_t redos;

	[[nodiscard]] static std::optional<std::filesystem::path> get_ini_path(EdittimeSettings& s, RuntimeSettingsLocal const& l)
	{
		if(!s.b_defaultFile || std::empty(s.defaultFile))
		{
			return {};
		}
		std::filesystem::path p{std::move(s.defaultFile)};
		if(!p.has_root_path())
		{
			return l.default_dir/p;
		}
		return p;
	}
	RuntimeSettingsGlobal(EdittimeSettings& s, RuntimeSettingsLocal const& l, RunHeader const& rh)
	: ini_path{get_ini_path(s, l)}
	, read_only{ini_path? s.b_ReadOnly : false}
	, auto_save{s.bool_AutoSave}
	, never_quote{s.bool_stdINI? false : s.bool_QuoteStrings}
	, compress{s.bool_compress}
	, save_repeated{s.bool_stdINI? false : s.saveRepeated}
	, esc_group{s.bool_stdINI? false : s.bool_EscapeGroup}
	, esc_item {s.bool_stdINI? false : s.bool_EscapeItem }
	, esc_value{s.bool_stdINI? false : s.bool_EscapeValue}
	, case_sensitive{s.bool_stdINI? false : s.bool_CaseSensitive}
	, subgroups{s.bool_stdINI? false : s.subGroups}
	, empty_groups{s.bool_stdINI? true : s.allowEmptyGroups}
	, encrypt_key{s.bool_encrypt && !std::empty(s.encrypt_key)? std::optional(lSDK::narrow_from_wide(s.encrypt_key)) : std::nullopt}
	, newline{!s.bool_stdINI && s.bool_newline && !std::empty(s.newline)? std::optional(std::move(s.newline)) : std::nullopt}
	, repeat_mode_group{s.bool_stdINI? RepeatModeGroup::GroupMerge : get_repeat_mode_group(s.repeatGroups)}
	, repeat_mode_item {s.bool_stdINI? RepeatModeItem ::Rename     : get_repeat_mode_item (s.repeatItems )}
	, undos{s.undoCount}
	, redos{s.redoCount}
	{
	}
};
