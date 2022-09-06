#include "FusionAPI.hpp"
#include "lSDK.hpp"
#include "lSDK/FusionMenu.hpp"
#include "lSDK/FusionUtilities.hpp"

#include "CustomParams.hpp"
#include "Resources.h"

#include <array>
#include <cassert>
#include <cstdint>
#include <string_view>

template<std::int16_t Type, ::UINT NameResource>
struct ConstantAceParam final
{
	static constexpr std::int16_t const type{Type};
	#ifndef FUSION_RUNTIME_ONLY
	static constexpr ::UINT const name_resource{NameResource};
	#endif
};
struct AceParam final
{
	std::int16_t type;
	#ifndef FUSION_RUNTIME_ONLY
	::UINT name_resource;
	#endif
};
struct AceInfo final
{
	std::int16_t const* fusion_infos;
	std::basic_string_view<AceParam> params;
	#ifndef FUSION_RUNTIME_ONLY
	::UINT menu_name_resource;
	::UINT display_string_resource;
	#endif
};
template<std::int16_t Id, std::int16_t Flags, typename... Params>
struct ConstantAceInfo final
{
	static constexpr std::array<std::int16_t, 3 + 2*sizeof...(Params)> const info{Id, Flags, sizeof...(Params), Params::type..., (0*Params::type)...};
	static constexpr std::array<AceParam, sizeof...(Params)> const params{AceParam
	{
		Params::type
		#ifndef FUSION_RUNTIME_ONLY
		, Params::name_resource
		#endif
	}...};
};
template<std::int16_t Id, ::UINT MenuNameResource, ::UINT DisplayStringResource, std::int16_t Flags, typename... Params>
static constexpr auto constant_ace_info() noexcept
-> AceInfo
{
	using Impl = ConstantAceInfo<Id, Flags, Params...>;
	return AceInfo
	{
		decltype(AceInfo::fusion_infos)(std::data(Impl::info)),
		decltype(AceInfo::params)(std::data(Impl::params), std::size(Impl::params))
		#ifndef FUSION_RUNTIME_ONLY
		, decltype(AceInfo::menu_name_resource)(MenuNameResource),
		decltype(AceInfo::display_string_resource)(DisplayStringResource)
		#endif
	};
}

static constexpr std::array const action_infos
{
	constant_ace_info<0, RIPPAM_SET_CURRENT_GROUP, RIPPA_SET_CURRENT_GROUP, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_CREATE_DUPLICATE_GROUP>
	>(),
	constant_ace_info<1, RIPPAM_SET_VALUE, RIPPA_SET_VALUE, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_TYPE>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_VALUE>
	>(),
	constant_ace_info<2, RIPPAM_SET_STRING, RIPPA_SET_STRING, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_STRING>
	>(),
	constant_ace_info<3, RIPPAM_SET_STRING_MD5, RIPPA_SET_STRING_MD5, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_STRING>
	>(),
	constant_ace_info<4, RIPPAM_SAVE_OBJECT, RIPPA_SAVE_OBJECT, 0,
		ConstantAceParam<PARAM_OBJECT, RIPPP_SAVE_OBJECT>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_SAVE_POSITION>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_SAVE_ALT_VALUES>
	>(),
	constant_ace_info<5, RIPPAM_LOAD_OBJECT, RIPPA_LOAD_OBJECT, 0,
		ConstantAceParam<PARAM_OBJECT, RIPPP_LOAD_OBJECT>
	>(),
	constant_ace_info<6, RIPPAM_SAVE_POSITION, RIPPA_SAVE_POSITION, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_POSITION, RIPPP_POSITION>
	>(),
	constant_ace_info<7, RIPPAM_SAVE_GLOBALS, RIPPA_SAVE_GLOBALS, 0>(),
	constant_ace_info<8, RIPPAM_LOAD_GLOBALS, RIPPA_LOAD_GLOBALS, 0>(),
	constant_ace_info<9, RIPPAM_DELETE_GROUP, RIPPA_DELETE_CURRENT_GROUP, 0>(),
	constant_ace_info<10, RIPPAM_DELETE_ITEM, RIPPA_DELETE_ITEM, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>
	>(),
	constant_ace_info<11, RIPPAM_RENAME_GROUP, RIPPA_RENAME_CURRENT_GROUP, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_NEW_NAME>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_OVERWRITE_MODE_012>
	>(),
	constant_ace_info<12, RIPPAM_RENAME_ITEM, RIPPA_RENAME_ITEM, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_OLD_NAME>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_NEW_NAME>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_ALLOW_OVERWRITE>
	>(),
	constant_ace_info<13, RIPPAM_MOVE_ITEM_ANOTHER, RIPPA_MOVE_ITEM_ANOTHER, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_NEW_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_ALLOW_OVERWRITE>
	>(),
	constant_ace_info<14, RIPPAM_SET_VALUE, RIPPA_GROUP_SET_VALUE, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_TYPE>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_VALUE>
	>(),
	constant_ace_info<15, RIPPAM_SET_STRING, RIPPA_GROUP_SET_STRING, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_STRING>
	>(),
	constant_ace_info<16, RIPPAM_SET_STRING_MD5, RIPPA_GROUP_SET_STRING_MD5, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_VALUE>
	>(),
	constant_ace_info<17, RIPPAM_SAVE_OBJECT, RIPPA_GROUP_SAVE_OBJECT, 0,
		ConstantAceParam<PARAM_OBJECT, RIPPP_SAVE_OBJECT>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_SAVE_POSITION>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_SAVE_ALT_VALUES>
	>(),
	constant_ace_info<18, RIPPAM_LOAD_OBJECT, RIPPA_GROUP_LOAD_OBJECT, 0,
		ConstantAceParam<PARAM_OBJECT, RIPPP_LOAD_OBJECT>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>
	>(),
	constant_ace_info<19, RIPPAM_SAVE_POSITION, RIPPA_GROUP_SAVE_POSITION, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_POSITION, RIPPP_POSITION>
	>(),
	constant_ace_info<20, RIPPAM_SAVE_GLOBALS, RIPPA_GROUP_SAVE_GLOBALS, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>
	>(),
	constant_ace_info<21, RIPPAM_LOAD_GLOBALS, RIPPA_GROUP_LOAD_GLOBALS, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>
	>(),
	constant_ace_info<22, RIPPAM_RENAME_GROUP, RIPPA_RENAME_GROUP, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_OLD_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_NEW_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_OVERWRITE_MODE_012>
	>(),
	constant_ace_info<23, RIPPAM_RENAME_ITEM, RIPPA_GROUP_RENAME_ITEM, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_OLD_ITEM_NAME>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_NEW_ITEM_NAME>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_ALLOW_OVERWRITE>
	>(),
	constant_ace_info<24, RIPPAM_MOVE_ITEM, RIPPA_MOVE_ITEM, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_OLD_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_OLD_ITEM_NAME>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_NEW_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_NEW_ITEM_NAME>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_ALLOW_OVERWRITE>
	>(),
	constant_ace_info<25, RIPPAM_MOVE_ITEM_ANOTHER, RIPPA_GROUP_MOVE_ITEM_ANOTHER, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_OLD_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_NEW_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_ALLOW_OVERWRITE>
	>(),
	constant_ace_info<26, RIPPAM_COPY_GROUP, RIPPA_COPY_GROUP, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_SRC_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_DST_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_OVERWRITE_MODE_01>
	>(),
	constant_ace_info<27, RIPPAM_COPY_ITEM, RIPPA_COPY_ITEM, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_SRC_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_SRC_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_DST_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_DST_ITEM>
	>(),
	constant_ace_info<28, RIPPAM_DELETE_GROUP, RIPPA_DELETE_GROUP, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP_TO_DELETE>
	>(),
	constant_ace_info<29, RIPPAM_DELETE_ITEM, RIPPA_GROUP_DELETE_ITEM, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>
	>(),
	constant_ace_info<30, RIPPAM_DELETE_ITEM_EVERYWHERE, RIPPA_DELETE_ITEM_EVERYWHERE, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>
	>(),
	constant_ace_info<31, RIPPAM_DELETE_PATTERN, RIPPA_DELETE_PATTERN, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_PATTERN_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_PATTERN_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_PATTERN_VALUE>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_CASE_SENSITIVE>
	>(),
	constant_ace_info<32, RIPPAM_CLEAR_ALL, RIPPA_CLEAR_ALL, 0>(),
	constant_ace_info<33, RIPPAM_PERFORM_SEARCH, RIPPA_PERFORM_SEARCH, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_CONDITION_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_CONDITION_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_CONDITION_VALUE>,
		ConstantAceParam<PARAM_SEARCH, RIPPP_SEARCH_SETTINGS>
	>(),
	constant_ace_info<34, RIPPAM_FIND_REPEATS_ITEM, RIPPA_FIND_REPEATS_ITEM, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>
	>(),
	constant_ace_info<35, RIPPAM_CLEAR_RESULTS, RIPPA_CLEAR_RESULTS, 0>(),
	constant_ace_info<36, RIPPAM_SEARCH_STORE_INI, RIPPA_SEARCH_STORE_INI, 0,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_DESTINATION_FILE>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_CONDITION_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_CONDITION_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_CONDITION_VALUE>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_CASE_SENSITIVE>
	>(),
	constant_ace_info<37, RIPPAM_SEARCH_STORE_INIPP, RIPPA_SEARCH_STORE_INIPP, 0,
		ConstantAceParam<PARAM_OBJECTSELECTOR2, RIPPP_OBJECT_SELECTOR>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_CONDITION_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_CONDITION_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_CONDITION_VALUE>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_CASE_SENSITIVE>
	>(),
	constant_ace_info<38, RIPPAM_MERGE_WITH_FILE, RIPPA_MERGE_WITH_FILE, 0,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_FILE_TO_MERGE_FROM>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_ALLOW_OVERWRITE>
	>(),
	constant_ace_info<39, RIPPAM_MERGE_GROUP_WITH_FILE, RIPPA_MERGE_GROUP_WITH_FILE, 0,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_FILE_TO_MERGE_FROM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_SRC_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_DST_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_ALLOW_OVERWRITE>
	>(),
	constant_ace_info<40, RIPPAM_MERGE_WITH_INIPP, RIPPA_MERGE_WITH_INIPP, 0,
		ConstantAceParam<PARAM_OBJECTSELECTOR, RIPPP_OTHER_INIPP>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_ALLOW_OVERWRITE>
	>(),
	constant_ace_info<41, RIPPAM_MERGE_GROUP_WITH_INIPP, RIPPA_MERGE_GROUP_WITH_INIPP, 0,
		ConstantAceParam<PARAM_OBJECTSELECTOR, RIPPP_OTHER_INIPP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_SRC_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_DST_GROUP_NAME>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_ALLOW_OVERWRITE>
	>(),
	constant_ace_info<42, /*removed*/0, RIPPA_NEW_FILE, 0,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_FILE_OR_BLANK>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_CLEAR_EXISTING>
	>(),
	constant_ace_info<43, /*removed*/0, RIPPA_LOAD_FILE, 0,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_FILE_TO_LOAD>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_READ_ONLY>
	>(),
	constant_ace_info<44, RIPPAM_SAVE_FILE, RIPPA_SAVE_FILE, 0>(),
	constant_ace_info<45, RIPPAM_SAVE_AS, RIPPA_SAVE_AS, 0,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_DESTINATION_FILE>
	>(),
	constant_ace_info<46, RIPPAM_BACKUP_TO, RIPPA_BACKUP_TO, 0,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_FILE_TO_BACKUP_TO>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_COMPRESSED>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ENCRYPTED>
	>(),
	constant_ace_info<47, RIPPAM_CLOSE, RIPPA_CLOSE, 0>(),
	constant_ace_info<48, RIPPAM_LOAD_FROM_STRING, RIPPA_LOAD_FROM_STRING, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_INI_TEXT>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_MODE_REPLACE_MERGE>
	>(),
	constant_ace_info<49, RIPPAM_SET_AUTO_LOAD_SAVE, RIPPA_SET_AUTO_LOAD_SAVE, 0,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_AUTO_SAVE>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_AUTO_LOAD>
	>(),
	constant_ace_info<50, RIPPAM_SET_COMPRESSION, RIPPA_SET_COMPRESSION, 0,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_COMPRESSED>
	>(),
	constant_ace_info<51, RIPPAM_SET_ENCRYPTION, RIPPA_SET_ENCRYPTION, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ENCRYPTED>
	>(),
	constant_ace_info<52, RIPPAM_SET_READ_ONLY, RIPPA_SET_READ_ONLY, 0,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_READ_ONLY>
	>(),
	constant_ace_info<53, RIPPAM_SET_CASE_SENSITIVE, RIPPA_SET_CASE_SENSITIVE, 0,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_CASE_SENSITIVE>
	>(),
	constant_ace_info<54, RIPPAM_SET_ESCAPE_CHARS, RIPPA_SET_ESCAPE_CHARS, 0,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_USE_IN_GROUPS>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_USE_IN_ITEMS>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_USE_IN_VALUES>
	>(),
	constant_ace_info<55, RIPPAM_SET_ALWAYS_QUOTE, RIPPA_SET_ALWAYS_QUOTE, 0,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_QUOTE_STRINGS>
	>(),
	constant_ace_info<56, RIPPAM_SET_REPEAT_MODES, RIPPA_SET_REPEAT_MODES, 0,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_REPEAT_MODE_GROUPS>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_REPEAT_MODE_ITEMS>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_SAVE_REPEATS_AS_ORIGINAL>
	>(),
	constant_ace_info<57, RIPPAM_SET_NEWLINE_STR, RIPPA_SET_NEWLINE_STR, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_NEWLINE_OR_EMPTY>
	>(),
	constant_ace_info<58, RIPPAM_SET_DEFAULT_DIR, RIPPA_SET_DEFAULT_DIR, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_DEFAULT_DIRECTORY>
	>(),
	constant_ace_info<59, RIPPAM_COMPRESS_FILE, RIPPA_COMPRESS_FILE, 0,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_FILE_SRC>,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_FILE_DST>
	>(),
	constant_ace_info<60, RIPPAM_DECOMPRESS_FILE, RIPPA_DECOMPRESS_FILE, 0,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_FILE_SRC>,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_FILE_DST>
	>(),
	constant_ace_info<61, RIPPAM_OPEN_DIALOG, RIPPA_OPEN_DIALOG, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_DIALOG_TITLE>,
		ConstantAceParam<PARAM_DIALOGSELECTOR, RIPPP_DIALOG_SETTINGS>
	>(),
	constant_ace_info<62, RIPPAM_ADD_REPEATED_ITEM, RIPPA_ADD_REPEATED_ITEM, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_STRING_OR_VALUE>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_REPEATED_MODE>
	>(),
	constant_ace_info<63, RIPPAM_CLOSE_DIALOG, RIPPA_CLOSE_DIALOG, 0>(),
	constant_ace_info<64, RIPPAM_REFRESH_DIALOG, RIPPA_REFRESH_DIALOG, 0,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_DIALOG_REFRESH_MODE>
	>(),
	constant_ace_info<65, RIPPAM_IO_CSV, RIPPA_IO_CSV, 0,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_IMPORT_EXPORT_MODE>,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_CSV_FILE>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_IMPORT_EXPORT_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_SEPARATOR_SYMBOL>
	>(),
	constant_ace_info<66, RIPPAM_IO_CSV_ARRAY, RIPPA_IO_CSV_ARRAY, 0,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_IMPORT_EXPORT_MODE>,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_CSV_FILE>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_SEPARATOR_SYMBOL>
	>(),
	constant_ace_info<67, RIPPAM_PUT_DATA_INTO_CHART, RIPPA_PUT_DATA_INTO_CHART, 0,
		ConstantAceParam<PARAM_OBJECTSELECTOR3, RIPPP_CHART_OBJECT>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>
	>(),
	constant_ace_info<68, RIPPAM_FIND_SUBGROUPS, RIPPA_FIND_SUBGROUPS, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_SUBGROUPS_GROUP>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_DEPTH>
	>(),
	constant_ace_info<69, RIPPAM_SET_GROUP_OPTS, RIPPA_SET_GROUP_OPTS, 0,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_ALLOW_SUBGROUPS>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_ALLOW_EMPTY_GROUPS>
	>(),
	constant_ace_info<70, RIPPAM_CHANGE_GLOBAL_KEY, RIPPA_CHANGE_GLOBAL_KEY, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_DATA_SLOT>
	>(),
	constant_ace_info<71, RIPPAM_REORDER_GROUP, RIPPA_REORDER_GROUP, 0,
		ConstantAceParam<PARAM_SSS, RIPPP_SSS_SETTINGS>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>
	>(),
	constant_ace_info<72, RIPPAM_SET_PART_IN_ITEM, RIPPA_SET_PART_IN_ITEM, 0,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_PART>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_STRING>
	>(),
	constant_ace_info<73, RIPPAM_GET_DATA_FROM_ARRAY, RIPPA_GET_DATA_FROM_ARRAY, 0,
		ConstantAceParam<PARAM_OBJECTSELECTOR4, RIPPP_ARRAY_OBJECT>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>
	>(),
	constant_ace_info<74, RIPPAM_PUT_DATA_INTO_ARRAY, RIPPA_PUT_DATA_INTO_ARRAY, 0,
		ConstantAceParam<PARAM_OBJECTSELECTOR5, RIPPP_ARRAY_OBJECT>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>
	>(),
	constant_ace_info<75, RIPPAM_GET_DATA_FROM_CHART, RIPPA_GET_DATA_FROM_CHART, 0,
		ConstantAceParam<PARAM_OBJECTSELECTOR3, RIPPP_CHART_OBJECT>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>
	>(),
	constant_ace_info<76, RIPPAM_GET_SETTINGS_FROM_CHART, RIPPA_GET_SETTINGS_FROM_CHART, 0,
		ConstantAceParam<PARAM_OBJECTSELECTOR3, RIPPP_CHART_OBJECT>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>
	>(),
	constant_ace_info<77, RIPPAM_PUT_SETTINGS_INTO_CHART, RIPPA_PUT_SETTINGS_INTO_CHART, 0,
		ConstantAceParam<PARAM_OBJECTSELECTOR3, RIPPP_CHART_OBJECT>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>
	>(),
	constant_ace_info<78, RIPPAM_CHANGE_FILE, RIPPA_CHANGE_FILE, 0,
		ConstantAceParam<PARAM_FILENAME2, RIPPP_FILE_TO_LOAD>,
		ConstantAceParam<PARAM_LOADFILE, RIPPP_FILE_SETTINGS>
	>(),
	constant_ace_info<79, RIPPAM_PERFORM_UNDO, RIPPA_PERFORM_UNDO, 0>(),
	constant_ace_info<80, RIPPAM_PERFORM_REDO, RIPPA_PERFORM_REDO, 0>(),
	constant_ace_info<81, RIPPAM_CLEAR_STACKS, RIPPA_CLEAR_STACKS, 0,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_UNDO_REDO_BOTH>
	>(),
	constant_ace_info<82, RIPPAM_FORCE_ADD_UNDO_BLOCK, RIPPA_FORCE_ADD_UNDO_BLOCK, 0>(),
	constant_ace_info<83, RIPPAM_SET_MODE, RIPPA_SET_MODE, 0,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_AUTOMATIC_MODE>
	>(),
};
static constexpr std::array const condition_infos
{
	constant_ace_info<0, RIPPCM_GROUP_EXISTS, RIPPC_CURRENT_GROUP_EXISTS, EVFLAGS_NOTABLE|EVFLAGS_ALWAYS>(),
	constant_ace_info<1, RIPPCM_GROUP_HAS_ITEM, RIPPC_CURRENT_GROUP_HAS_ITEM, EVFLAGS_NOTABLE|EVFLAGS_ALWAYS,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>
	>(),
	constant_ace_info<2, RIPPCM_ITEM_HAS_HASH, RIPPC_ITEM_HAS_HASH, EVFLAGS_NOTABLE|EVFLAGS_ALWAYS,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_DEFAULT_STRING>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_COMPARE_STRING>
	>(),
	constant_ace_info<3, RIPPCM_GROUP_EXISTS, RIPPC_GROUP_EXISTS, EVFLAGS_NOTABLE|EVFLAGS_ALWAYS,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>
	>(),
	constant_ace_info<4, RIPPCM_GROUP_HAS_ITEM, RIPPC_GROUP_HAS_ITEM, EVFLAGS_NOTABLE|EVFLAGS_ALWAYS,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>
	>(),
	constant_ace_info<5, RIPPCM_ITEM_HAS_HASH, RIPPC_GROUP_ITEM_HAS_HASH, EVFLAGS_NOTABLE|EVFLAGS_ALWAYS,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_GROUP>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_ITEM>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_DEFAULT_STRING>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_COMPARE_STRING>
	>(),
	constant_ace_info<6, RIPPCM_IS_WILDCAT_MATCH, RIPPC_IS_WILDCAT_MATCH, EVFLAGS_NOTABLE|EVFLAGS_ALWAYS,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_PATTERN>,
		ConstantAceParam<PARAM_EXPSTRING, RIPPP_STRING>,
		ConstantAceParam<PARAM_EXPRESSION, RIPPP_CASE_SENSITIVE>
	>(),
	constant_ace_info<7, RIPPCM_ON_OPEN, RIPPC_ON_OPEN, EVFLAGS_NOTABLE>(),
	constant_ace_info<8, RIPPCM_ON_CLOSE, RIPPC_ON_CLOSE, EVFLAGS_NOTABLE>(),
	constant_ace_info<9, RIPPCM_ON_MODIFIED, RIPPC_ON_MODIFIED, EVFLAGS_NOTABLE>(),
	constant_ace_info<10, RIPPCM_ON_DATA_MODIFIED, RIPPC_ON_DATA_MODIFIED, EVFLAGS_NOTABLE>(),
	constant_ace_info<11, RIPPCM_ON_SAVE, RIPPC_ON_SAVE, EVFLAGS_NOTABLE>(),
	constant_ace_info<12, RIPPCM_ON_NEW_ITEM, RIPPC_ON_NEW_ITEM, EVFLAGS_NOTABLE>(),
	constant_ace_info<13, RIPPCM_ON_RESULTS_FOUND, RIPPC_ON_RESULTS_FOUND, EVFLAGS_NOTABLE>(),
	constant_ace_info<14, RIPPCM_ON_NO_RESULTS_FOUND, RIPPC_ON_NO_RESULTS_FOUND, EVFLAGS_NOTABLE>(),
	constant_ace_info<15, RIPPCM_ON_MERGE, RIPPC_ON_MERGE, EVFLAGS_NOTABLE>(),
	constant_ace_info<16, RIPPCM_ON_UNCOMPRESSION_OK, RIPPC_ON_UNCOMPRESSION_OK, EVFLAGS_NOTABLE>(),
	constant_ace_info<17, RIPPCM_ON_UNCOMPRESSION_FAIL, RIPPC_ON_UNCOMPRESSION_FAIL, EVFLAGS_NOTABLE>(),
	constant_ace_info<18, RIPPCM_HAS_UNDO, RIPPC_HAS_UNDO, EVFLAGS_NOTABLE|EVFLAGS_ALWAYS>(),
	constant_ace_info<19, RIPPCM_HAS_REDO, RIPPC_HAS_REDO, EVFLAGS_NOTABLE|EVFLAGS_ALWAYS>(),
};
static constexpr std::array const expression_infos
{
	constant_ace_info<0, RIPPEM_GROUP_GET_ITEM_VALUE, RIPPE_GROUP_GET_ITEM_VALUE, 0,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_ITEM_S>,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_DEFAULT_VALUE>
	>(),
	constant_ace_info<1, RIPPEM_GROUP_GET_ITEM_STRING, RIPPE_GROUP_GET_ITEM_STRING, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_ITEM_S>,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_DEFAULT_VALUE_S>
	>(),
	constant_ace_info<2, RIPPEM_GROUP_GET_POSITION_X, RIPPE_GROUP_GET_POSITION_X, 0,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_ITEM_S>
	>(),
	constant_ace_info<3, RIPPEM_GROUP_GET_POSITION_Y, RIPPE_GROUP_GET_POSITION_Y, 0,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_ITEM_S>
	>(),
	constant_ace_info<4, RIPPEM_GROUP_GET_NTH_ITEM_NAME, RIPPE_GROUP_GET_NTH_ITEM_NAME, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<5, RIPPEM_GROUP_GET_NTH_ITEM_VALUE, RIPPE_GROUP_GET_NTH_ITEM_VALUE, 0,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<6, RIPPEM_GROUP_GET_NTH_ITEM_STRING, RIPPE_GROUP_GET_NTH_ITEM_STRING, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<7, RIPPEM_GROUP_GET_NUM_ITEMS, RIPPE_GROUP_GET_NUM_ITEMS, 0>(),
	constant_ace_info<8, RIPPEM_GET_ITEM_VALUE, RIPPE_GET_ITEM_VALUE, 0,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_GROUP_S>,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_ITEM_S>,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_DEFAULT_VALUE>
	>(),
	constant_ace_info<9, RIPPEM_GET_ITEM_STRING, RIPPE_GET_ITEM_STRING, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_GROUP_S>,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_ITEM_S>,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_DEFAULT_VALUE_S>
	>(),
	constant_ace_info<10, RIPPEM_GET_POSITION_X, RIPPE_GET_POSITION_X, 0,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_GROUP_S>,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_ITEM_S>
	>(),
	constant_ace_info<11, RIPPEM_GET_POSITION_Y, RIPPE_GET_POSITION_Y, 0,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_GROUP_S>,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_ITEM_S>
	>(),
	constant_ace_info<12, RIPPEM_GET_NTH_GROUP, RIPPE_GET_NTH_GROUP, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<13, RIPPEM_GET_NTH_ITEM_NAME, RIPPE_GET_NTH_ITEM_NAME, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_GROUP_S>,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<14, RIPPEM_GET_NTH_ITEM_STRING, RIPPE_GET_NTH_ITEM_STRING, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_GROUP_S>,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<15, RIPPEM_GET_NTH_ITEM_VALUE, RIPPE_GET_NTH_ITEM_VALUE, 0,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_GROUP_S>,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<16, RIPPEM_GET_NUM_GROUPS, RIPPE_GET_NUM_GROUPS, 0>(),
	constant_ace_info<17, RIPPEM_GET_NUM_ITEMS, RIPPE_GET_NUM_ITEMS, 0,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_GROUP_S>
	>(),
	constant_ace_info<18, RIPPEM_GET_NUM_ITEMS_EVERYWHERE, RIPPE_GET_NUM_ITEMS_EVERYWHERE, 0>(),
	constant_ace_info<19, RIPPEM_GET_NUM_SEARCH_RESULTS, RIPPE_GET_NUM_SEARCH_RESULTS, 0>(),
	constant_ace_info<20, RIPPEM_GET_NTH_SEARCH_RESULT_GROUP, RIPPE_GET_NTH_SEARCH_RESULT_GROUP, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<21, RIPPEM_GET_NTH_SEARCH_RESULT_ITEM_NAME, RIPPE_GET_NTH_SEARCH_RESULT_ITEM_NAME, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<22, RIPPEM_GET_NTH_SEARCH_RESULT_ITEM_VALUE, RIPPE_GET_NTH_SEARCH_RESULT_ITEM_VALUE, 0,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<23, RIPPEM_GET_NTH_SEARCH_RESULT_ITEM_STRING, RIPPE_GET_NTH_SEARCH_RESULT_ITEM_STRING, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<24, RIPPEM_GET_NTH_SEARCH_RESULT_PATH, RIPPE_GET_NTH_SEARCH_RESULT_PATH, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_SEPARATOR_S>
	>(),
	constant_ace_info<25, RIPPEM_GET_STRINGIFIED, RIPPE_GET_STRINGIFIED, EXPFLAG_STRING>(),
	constant_ace_info<26, RIPPEM_HASH_STRING, RIPPE_HASH_STRING, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_STRING_S>
	>(),
	constant_ace_info<27, /*removed?*/0, RIPPE_CRYPT, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_STRING_S>,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_KEY_S>
	>(),
	constant_ace_info<28, RIPPEM_ESCAPE_STRING, RIPPE_ESCAPE_STRING, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_STRING_S>
	>(),
	constant_ace_info<29, RIPPEM_UNESCAPE_STRING, RIPPE_UNESCAPE_STRING, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_STRING_S>
	>(),
	constant_ace_info<30, RIPPEM_PERFORM_CALCULATIONS, RIPPE_PERFORM_CALCULATIONS, 0,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_CONSULT_DOCUMENTATION>,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_CONSULT_DOCUMENTATION>
	>(),
	constant_ace_info<31, RIPPEM_FIND_MIN_OR_MAX_GROUP, RIPPE_FIND_MIN_OR_MAX_GROUP, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_CONSULT_DOCUMENTATION>,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_CONSULT_DOCUMENTATION>
	>(),
	constant_ace_info<32, RIPPEM_GET_SORTED_NTH_ITEM_NAME, RIPPE_GET_SORTED_NTH_ITEM_NAME, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_GROUP_S>,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<33, RIPPEM_GET_SORTED_NTH_ITEM_VALUE, RIPPE_GET_SORTED_NTH_ITEM_VALUE, 0,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_GROUP_S>,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<34, RIPPEM_GET_CSVIFIED, RIPPE_GET_CSVIFIED, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_GROUP_S>,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_SEPARATOR_S>
	>(),
	constant_ace_info<35, RIPPEM_GET_NTH_ITEM_EVERYWHERE, RIPPE_GET_NTH_ITEM_EVERYWHERE, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>
	>(),
	constant_ace_info<36, RIPPEM_GET_NUM_UNIQUE_ITEM_NAMES, RIPPE_GET_NUM_UNIQUE_ITEM_NAMES, 0>(),
	constant_ace_info<37, RIPPEM_GET_PART_IN_ITEM, RIPPE_GET_PART_IN_ITEM, EXPFLAG_STRING,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_GROUP_S>,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_ITEM_S>,
		ConstantAceParam<EXPPARAM_LONG, RIPPP_N>,
		ConstantAceParam<EXPPARAM_STRING, RIPPP_DEFAULT_VALUE_S>
	>(),
	constant_ace_info<38, RIPPEM_GROUP_GET_NAME, RIPPE_GROUP_GET_NAME, EXPFLAG_STRING>(),
	constant_ace_info<39, RIPPEM_GROUP_GET_STRINGIFIED, RIPPE_GROUP_GET_STRINGIFIED, EXPFLAG_STRING>(),
	constant_ace_info<40, RIPPEM_GET_FILENAME, RIPPE_GET_FILENAME, EXPFLAG_STRING>(),
};

#ifdef FUSION_GET_ACTION_MENU
auto FUSION_API GetActionMenu(mv* const mV, OI* const object_info, SerializedEditData* serialized_edit_data) noexcept
-> HMENU
{
	using Menu = lSDK::ActionMenu;
	static constexpr auto const mi{[](std::int16_t const id) noexcept { return Menu::Item{id, action_infos.at(id).menu_name_resource}; }};
	static /*constexpr*/ Menu const menu
	{{
		Menu::Separator{},
		mi(0), //SET_CURRENT_GROUP
		Menu::Submenu{RIPPAM_CURRENT_GROUP,
		{
			mi(1), //SET_VALUE
			mi(2), //SET_STRING
			mi(3), //SET_STRING_MD5
			Menu::Separator{},
			mi(4), //SAVE_OBJECT
			mi(5), //LOAD_OBJECT
			mi(6), //SAVE_POSITION
			mi(7), //SAVE_GLOBALS
			mi(8), //LOAD_GLOBALS
			Menu::Separator{},
			mi(9), //DELETE_GROUP
			mi(10), //DELETE_ITEM
			Menu::Separator{},
			mi(11), //RENAME_GROUP
			mi(12), //RENAME_ITEM
			mi(13), //MOVE_ITEM_ANOTHER
		}},
		Menu::Separator{},
		Menu::Submenu{RIPPAM_SETTING_ITEMS,
		{
			mi(14), //SET_VALUE
			mi(15), //SET_STRING
			mi(16), //SET_STRING_MD5
			Menu::Separator{},
			mi(17), //SAVE_OBJECT
			mi(18), //LOAD_OBJECT
			mi(19), //SAVE_POSITION
			mi(20), //SAVE_GLOBALS
			mi(21), //LOAD_GLOBALS
		}},
		Menu::Submenu{RIPPAM_MOVING_COPYING,
		{
			mi(22), //RENAME_GROUP
			mi(23), //RENAME_ITEM
			Menu::Separator{},
			mi(24), //MOVE_ITEM
			mi(25), //MOVE_ITEM_ANOTHER
			Menu::Separator{},
			mi(26), //COPY_GROUP
			mi(27), //COPY_ITEM
		}},
		Menu::Submenu{RIPPAM_DELETING,
		{
			mi(28), //DELETE_GROUP
			mi(29), //DELETE_ITEM
			mi(30), //DELETE_ITEM_EVERYWHERE
			mi(31), //DELETE_PATTERN
			Menu::Separator{},
			mi(32), //CLEAR_ALL
		}},
		Menu::Submenu{RIPPAM_SEARCHING,
		{
			mi(33), //PERFORM_SEARCH
			mi(35), //CLEAR_RESULTS
			Menu::Separator{},
			mi(36), //SEARCH_STORE_INI
			mi(37), //SEARCH_STORE_INIPP
			Menu::Separator{},
			mi(68), //FIND_SUBGROUPS
			mi(34), //FIND_REPEATS_ITEM
		}},
		Menu::Submenu{RIPPAM_MERGING,
		{
			mi(38), //MERGE_WITH_FILE
			mi(39), //MERGE_GROUP_WITH_FILE
			Menu::Separator{},
			mi(40), //MERGE_WITH_INIPP
			mi(41), //MERGE_GROUP_WITH_INIPP
		}},
		Menu::Submenu{RIPPAM_LOADING_SAVING,
		{
			mi(78), //CHANGE_FILE
			mi(44), //SAVE_FILE
			mi(47), //CLOSE
			Menu::Separator{},
			mi(48), //LOAD_FROM_STRING
			Menu::Separator{},
			mi(70), //CHANGE_GLOBAL_KEY
			Menu::Separator{},
			Menu::Submenu{RIPPAM_SETTINGS,
			{
				mi(52), //SET_READ_ONLY
				mi(51), //SET_ENCRYPTION
				mi(50), //SET_COMPRESSION
				mi(49), //SET_AUTO_LOAD_SAVE
			}},
			Menu::Submenu{RIPPAM_EXPORT,
			{
				mi(45), //SAVE_AS
				mi(46), //BACKUP_TO
				Menu::Separator{},
				mi(65), //IO_CSV
				mi(66), //IO_CSV_ARRAY
			}},
		}},
		Menu::Separator{},
		mi(71), //REORDER_GROUP
		Menu::Separator{},
		Menu::Submenu{RIPPAM_ADVANCED_FEATURES,
		{
			mi(62), //ADD_REPEATED_ITEM
			mi(72), //SET_PART_IN_ITEM
			Menu::Separator{},
			Menu::Submenu{RIPPAM_UNDO,
			{
				mi(79), //PERFORM_UNDO
				mi(80), //PERFORM_REDO
				Menu::Separator{},
				mi(81), //CLEAR_STACKS
				Menu::Submenu{RIPPAM_MANUAL_MODE,
				{
					mi(83), //SET_MODE
					mi(82), //FORCE_ADD_UNDO_BLOCK
				}},
			}},
			Menu::Submenu{RIPPAM_DIALOG_BOX,
			{
				mi(61), //OPEN_DIALOG
				mi(63), //CLOSE_DIALOG
				mi(64), //REFRESH_DIALOG
			}},
			Menu::Submenu{RIPPAM_CHART_OBJECT,
			{
				mi(67), //PUT_DATA_INTO_CHART
				mi(75), //GET_DATA_FROM_CHART
				Menu::Separator{},
				mi(77), //PUT_SETTINGS_INTO_CHART
				mi(76), //GET_SETTINGS_FROM_CHART
			}},
			Menu::Submenu{RIPPAM_ARRAY,
			{
				mi(73), //GET_DATA_FROM_ARRAY
				mi(74), //PUT_DATA_INTO_ARRAY
			}},
			Menu::Submenu{RIPPAM_ADVANCED_SETTINGS,
			{
				mi(53), //SET_CASE_SENSITIVE
				mi(54), //SET_ESCAPE_CHARS
				mi(55), //SET_ALWAYS_QUOTE
				mi(56), //SET_REPEAT_MODES
				mi(57), //SET_NEWLINE_STR
				mi(58), //SET_DEFAULT_DIR
				mi(69), //SET_GROUP_OPTS
			}},
			Menu::Submenu{RIPPAM_EXTRA_FUNCTIONS,
			{
				mi(59), //COMPRESS_FILE
				mi(60), //DECOMPRESS_FILE
			}},
		}},
		Menu::Separator{}
	}};
	return menu.generate().release();
}
#endif
#ifdef FUSION_GET_CONDITION_MENU
auto FUSION_API GetConditionMenu(mv* const mV, OI* const object_info, SerializedEditData* serialized_edit_data) noexcept
-> HMENU
{
	using Menu = lSDK::ConditionMenu;
	static constexpr auto const mi{[](std::int16_t const id) noexcept { return Menu::Item{id, condition_infos.at(id).menu_name_resource}; }};
	static /*constexpr*/ Menu const menu
	{{
		Menu::Separator{},
		Menu::Submenu{RIPPAM_CURRENT_GROUP,
		{
			mi(0), //GROUP_EXISTS
			mi(1), //GROUP_HAS_ITEM
			mi(2), //ITEM_HAS_HASH
		}},
		mi(3), //GROUP_EXISTS
		mi(4), //GROUP_HAS_ITEM
		mi(5), //ITEM_HAS_HASH
		Menu::Separator{},
		Menu::Submenu{RIPPAM_DIALOG_BOX,
		{
			mi(7), //ON_OPEN
			mi(8), //ON_CLOSE
			mi(9), //ON_MODIFIED
		}},
		Menu::Submenu{RIPPCM_SEARCH_RESULTS,
		{
			mi(13), //ON_RESULTS_FOUND
			mi(14), //ON_NO_RESULTS_FOUND
		}},
		Menu::Submenu{RIPPAM_UNDO,
		{
			mi(18), //HAS_UNDO
			mi(19), //HAS_REDO
		}},
		Menu::Separator{},
		mi(15), //ON_MERGE
		mi(10), //ON_DATA_MODIFIED
		mi(12), //ON_NEW_ITEM
		mi(11), //ON_SAVE
		Menu::Separator{},
		Menu::Submenu{RIPPAM_EXTRA_FUNCTIONS,
		{
			mi(6), //IS_WILDCAT_MATCH
			mi(16), //ON_UNCOMPRESSION_OK
			mi(17), //ON_UNCOMPRESSION_FAIL
		}},
		Menu::Separator{}
	}};
	return menu.generate().release();
}
#endif
#ifdef FUSION_GET_EXPRESSION_MENU
auto FUSION_API GetExpressionMenu(mv* const mV, OI* const object_info, SerializedEditData* serialized_edit_data) noexcept
-> HMENU
{
	using Menu = lSDK::ExpressionMenu;
	static constexpr auto const mi{[](std::int16_t const id) noexcept { return Menu::Item{id, expression_infos.at(id).menu_name_resource}; }};
	static /*constexpr*/ Menu const menu
	{{
		Menu::Separator{},
		Menu::Submenu{RIPPAM_CURRENT_GROUP,
		{
			mi(0), //GROUP_GET_ITEM_VALUE
			mi(1), //GROUP_GET_ITEM_STRING
			Menu::Separator{},
			mi(2), //GROUP_GET_POSITION_X
			mi(3), //GROUP_GET_POSITION_Y
			Menu::Separator{},
			mi(4), //GROUP_GET_NTH_ITEM_NAME
			mi(5), //GROUP_GET_NTH_ITEM_VALUE
			mi(6), //GROUP_GET_NTH_ITEM_STRING
			Menu::Separator{},
			mi(38), //GROUP_GET_NAME
			mi(7), //GROUP_GET_NUM_ITEMS
			Menu::Separator{},
			mi(39), //GROUP_GET_STRINGIFIED
		}},
		Menu::Separator{},
		mi(8), //GET_ITEM_VALUE
		mi(9), //GET_ITEM_STRING
		Menu::Separator{},
		mi(37), //GET_PART_IN_ITEM
		Menu::Separator{},
		mi(10), //GET_POSITION_X
		mi(11), //GET_POSITION_Y
		Menu::Separator{},
		mi(40), //GET_FILENAME
		mi(25), //GET_STRINGIFIED
		mi(34), //GET_CSVIFIED
		Menu::Separator{},
		Menu::Submenu{RIPPEM_COUNTING_CALCS,
		{
			mi(16), //GET_NUM_GROUPS
			mi(17), //GET_NUM_ITEMS
			mi(18), //GET_NUM_ITEMS_EVERYWHERE
			mi(36), //GET_NUM_UNIQUE_ITEM_NAMES
			mi(30), //PERFORM_CALCULATIONS
			mi(31), //FIND_MIN_OR_MAX_GROUP
		}},
		Menu::Submenu{RIPPEM_LIST_GROUPS_ITEMS,
		{
			mi(12), //GET_NTH_GROUP
			mi(13), //GET_NTH_ITEM_NAME
			mi(14), //GET_NTH_ITEM_STRING
			mi(15), //GET_NTH_ITEM_VALUE
			Menu::Separator{},
			mi(32), //GET_SORTED_NTH_ITEM_NAME
			mi(33), //GET_SORTED_NTH_ITEM_VALUE
			Menu::Separator{},
			mi(35), //GET_NTH_ITEM_EVERYWHERE
		}},
		Menu::Submenu{RIPPCM_SEARCH_RESULTS,
		{
			mi(19), //GET_NUM_SEARCH_RESULTS
			mi(20), //GET_NTH_SEARCH_RESULT_GROUP
			mi(21), //GET_NTH_SEARCH_RESULT_ITEM_NAME
			mi(22), //GET_NTH_SEARCH_RESULT_ITEM_VALUE
			mi(23), //GET_NTH_SEARCH_RESULT_ITEM_STRING
			mi(24), //GET_NTH_SEARCH_RESULT_PATH
		}},
		Menu::Separator{},
		Menu::Submenu{RIPPAM_EXTRA_FUNCTIONS,
		{
			mi(26), //HASH_STRING
			mi(28), //ESCAPE_STRING
			mi(29), //UNESCAPE_STRING
		}},
		Menu::Separator{}
	}};
	return menu.generate().release();
}
#endif

#ifdef FUSION_GET_ACTION_ID_FROM_ACTION_MENU_ID
auto FUSION_API GetActionCodeFromMenu(mv* const mV, std::int16_t const action_menu_id) noexcept
-> std::int16_t
{
	return lSDK::ActionMenu::ExtensionData::from_menu_id(action_menu_id);
}
#endif
#ifdef FUSION_GET_CONDITION_ID_FROM_CONDITION_MENU_ID
auto FUSION_API GetConditionCodeFromMenu(mv* const mV, std::int16_t const condition_menu_id) noexcept
-> std::int16_t
{
	return lSDK::ConditionMenu::ExtensionData::from_menu_id(condition_menu_id);
}
#endif
#ifdef FUSION_GET_EXPRESSION_ID_FROM_EXPRESSION_MENU_ID
auto FUSION_API GetExpressionCodeFromMenu(mv* const mV, std::int16_t const expression_menu_id) noexcept
-> std::int16_t
{
	std::int16_t const expression_id{lSDK::ExpressionMenu::ExtensionData::from_menu_id(expression_menu_id)};
	switch(expression_id)
	{
		case 30: std::ignore = ::MessageBoxW(::GetActiveWindow(), std::data(lSDK::get_resource_string(RIPPH_PERFORM_CALCULATIONS )), std::data(lSDK::get_resource_string(RIPPH_TITLE)), MB_ICONINFORMATION); break;
		case 31: std::ignore = ::MessageBoxW(::GetActiveWindow(), std::data(lSDK::get_resource_string(RIPPH_FIND_MIN_OR_MAX_GROUP)), std::data(lSDK::get_resource_string(RIPPH_TITLE)), MB_ICONINFORMATION); break;
		default: break;
	}
	return expression_id;
}
#endif

#ifdef FUSION_GET_ACTION_INFO_ARRAY
auto FUSION_API GetActionInfos(mv* const mV, std::int16_t const action_id) noexcept
-> fusion::ace_info_array
{
	return action_infos.at(action_id).fusion_infos;
}
#endif
#ifdef FUSION_GET_CONDITION_INFO_ARRAY
auto FUSION_API GetConditionInfos(mv* const mV, std::int16_t const condition_id) noexcept
-> fusion::ace_info_array
{
	return condition_infos.at(condition_id).fusion_infos;
}
#endif
#ifdef FUSION_GET_EXPRESSION_INFO_ARRAY
auto FUSION_API GetExpressionInfos(mv* const mV, std::int16_t const expression_id) noexcept
-> fusion::ace_info_array
{
	return expression_infos.at(expression_id).fusion_infos;
}
#endif

#ifdef FUSION_GET_ACTION_PARAM_NAME
void FUSION_API GetActionTitle(mv* const mV, std::int16_t const action_id, std::int16_t const param_id, fusion::string_buffer const param_name_buffer, std::int16_t const param_name_buffer_size) noexcept
{
	if(std::empty(action_infos.at(action_id).params))
	{
		return (void)lSDK::fill_buffer(param_name_buffer, param_name_buffer_size, TSL("<Fusion bug?>"));
	}
	return (void)lSDK::fill_buffer(param_name_buffer, param_name_buffer_size, lSDK::get_resource_string(action_infos.at(action_id).params.at(param_id).name_resource));
}
#endif
#ifdef FUSION_GET_CONDITION_PARAM_NAME
void FUSION_API GetConditionTitle(mv* const mV, std::int16_t const condition_id, std::int16_t const param_id, fusion::string_buffer const param_name_buffer, std::int16_t const param_name_buffer_size) noexcept
{
	if(std::empty(condition_infos.at(condition_id).params))
	{
		return (void)lSDK::fill_buffer(param_name_buffer, param_name_buffer_size, TSL("<Fusion bug?>"));
	}
	return (void)lSDK::fill_buffer(param_name_buffer, param_name_buffer_size, lSDK::get_resource_string(condition_infos.at(condition_id).params.at(param_id).name_resource));
}
#endif
#ifdef FUSION_GET_EXPRESSION_PARAM_NAME
void FUSION_API GetExpressionParam(mv* const mV, std::int16_t const expression_id, std::int16_t const param_id, fusion::string_buffer const param_name_buffer, std::int16_t const param_name_buffer_size) noexcept
{
	return (void)lSDK::fill_buffer(param_name_buffer, param_name_buffer_size, lSDK::get_resource_string(expression_infos.at(expression_id).params.at(param_id).name_resource));
}
#endif

#ifdef FUSION_GET_ACTION_DISPLAY_STRING
void FUSION_API GetActionString(mv* const mV, std::int16_t const action_id, fusion::string_buffer const display_string_buffer, std::int16_t const display_string_buffer_size) noexcept
{
	return (void)lSDK::fill_buffer(display_string_buffer, display_string_buffer_size, lSDK::get_resource_string(action_infos.at(action_id).display_string_resource));
}
#endif
#ifdef FUSION_GET_CONDITION_DISPLAY_STRING
void FUSION_API GetConditionString(mv* const mV, std::int16_t const condition_id, fusion::string_buffer const display_string_buffer, std::int16_t const display_string_buffer_size) noexcept
{
	return (void)lSDK::fill_buffer(display_string_buffer, display_string_buffer_size, lSDK::get_resource_string(condition_infos.at(condition_id).display_string_resource));
}
#endif

#ifdef FUSION_GET_EXPRESSION_EDITOR_STRING
void FUSION_API GetExpressionString(mv* const mV, std::int16_t const expression_id, fusion::string_buffer const editor_string_buffer, std::int16_t const editor_string_buffer_size) noexcept
{
	return (void)lSDK::fill_buffer(editor_string_buffer, editor_string_buffer_size, lSDK::get_resource_string(expression_infos.at(expression_id).display_string_resource));
}
#endif
