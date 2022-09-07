#include "FusionAPI.hpp"
#include "lSDK.hpp"
#include "lSDK/WindowsUtilities.hpp"
#include "EditData.hpp"
#include "Resources.h"

#include <array>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <initializer_list>

#ifndef FUSION_RUNTIME_ONLY
namespace PropId
{
	enum
	: std::int32_t
	{
		Version = PROPID_EXTITEM_CUSTOM_FIRST,
		PathTitle,
		DefPath,
		ReadOnly,
		DefFolder,
		DefaultText,
		Index,
		CreateFolders,
		AutoLoad,
		AutoSave,
		IniTitle,
		GlobalData,
		GlobalDataName,
		StandardQM,
		TreeTitle,
		Compress,
		Encrypt,
		NewLine,
		QuoteStrings,
		GroupRepeat,
		RepeatSave,
		ItemRepeat,
		EscapeChar_Group,
		EscapeChar_Item,
		EscapeChar_Value,
		CaseSensitive,
		SubGroups,
		AllowEmptyGroups,
		UndoTitle,
		Undo,
		Redo,
		CompEncYTitle,
	};
}

static constexpr std::array const default_folders
{
	std::uintptr_t{},
	std::uintptr_t{RIPPPR_BASE_DIR_WINDOWS},
	std::uintptr_t{RIPPPR_BASE_DIR_CURRENT},
	std::uintptr_t{RIPPPR_BASE_DIR_INSTALL},
	std::uintptr_t{RIPPPR_BASE_DIR_APPDATA},
	std::uintptr_t{RIPPPR_BASE_DIR_PRGDATA},
	std::uintptr_t{}
};

static constexpr std::array const group_repeats
{
	LPCTSTR{},
	_T("First Only"),
	_T("Last Only"),
	_T("Rename"),
	_T("Merge"),
	LPCTSTR{}
};
static constexpr std::array const item_repeats
{
	LPCTSTR{},
	_T("First Only"),
	_T("Last Only"),
	_T("Rename"),
	LPCTSTR{}
};

static constexpr std::array<std::int32_t, 2> const undo_range{-1, 127};

static constexpr std::array const file_options
{
	std::uint32_t{RIPPPR_DEFAULT_FILE_FILTER},
	std::uint32_t{OFN_PATHMUSTEXIST},
};
static_assert(sizeof(file_options) == sizeof(FilenameCreateParam));

#define _P(s) ((UINT_PTR)_T(s))
static PropData props_general[]
{
	PropData_StaticString    (PropId::Version       , RIPPPR_VERSION_NAME       , RIPPPR_VERSION_DESC),
	PropData_CheckBox        (PropId::GlobalData    , RIPPPR_GLOBAL_TICKBOX_NAME, RIPPPR_GLOBAL_TICKBOX_DESC),
	PropData_EditString      (PropId::GlobalDataName, RIPPPR_GLOBAL_EDITBOX_NAME, RIPPPR_GLOBAL_EDITBOX_DESC),
	PropData_Group           (PropId::PathTitle     , RIPPPR_FILEPATH_HEADER    , RIPPPR_FILEPATH_HEADER),
	PropData_Filename_Check  (PropId::DefPath       , RIPPPR_DEFAULT_FILE_NAME  , RIPPPR_DEFAULT_FILE_DESC, std::data(file_options)),
	PropData_CheckBox        (PropId::ReadOnly      , RIPPPR_READ_ONLY_NAME     , RIPPPR_READ_ONLY_DESC),
	PropData_ComboBox        (PropId::DefFolder     , RIPPPR_BASE_DIR_NAME      , RIPPPR_BASE_DIR_DESC, std::data(default_folders)),
	PropData_EditMultiLine   (PropId::DefaultText   , RIPPPR_INITIAL_DATA_NAME  , RIPPPR_INITIAL_DATA_DESC),
	PropData_CheckBox        (PropId::AutoLoad      , RIPPPR_AUTO_LOAD_NAME     , RIPPPR_AUTO_LOAD_DESC),
	PropData_CheckBox        (PropId::AutoSave      , RIPPPR_AUTO_SAVE_NAME     , RIPPPR_AUTO_SAVE_DESC),
	PropData_CheckBox        (PropId::CreateFolders , RIPPPR_CREATE_DIRS_NAME   , RIPPPR_CREATE_DIRS_DESC),
	PropData_CheckBox        (PropId::Index         , RIPPPR_BASE1INDEX_NAME    , RIPPPR_BASE1INDEX_DESC),
	PropData_Group           (PropId::CompEncYTitle , RIPPPR_OBFUSCATION_HEADER , RIPPPR_OBFUSCATION_HEADER),
	PropData_CheckBox        (PropId::Compress      , RIPPPR_COMPRESS_NAME      , RIPPPR_COMPRESS_DESC),
	PropData_EditString_Check(PropId::Encrypt       , RIPPPR_ENCRYPT_NAME       , RIPPPR_ENCRYPT_DESC),
	PropData_Group           (PropId::UndoTitle     , RIPPPR_UNDO_REDO_HEADER   , RIPPPR_UNDO_REDO_HEADER),
	PropData_SpinEdit        (PropId::Undo          , RIPPPR_UNDO_NAME          , RIPPPR_UNDO_DESC, std::data(undo_range)),
	PropData_SpinEdit        (PropId::Redo          , RIPPPR_REDO_NAME          , RIPPPR_REDO_DESC, std::data(undo_range)),
	PropData_End()
};
static PropData props_advanced[]
{
	PropData_Group           (PropId::IniTitle        , RIPPPR_FORMAT_HEADER, RIPPPR_FORMAT_HEADER),
	PropData_CheckBox        (PropId::StandardQM      , RIPPPR_STANDARD_NAME, RIPPPR_STANDARD_DESC),
	PropData_Group           (PropId::TreeTitle       , RIPPPR_ADVANCED_NAME, RIPPPR_ADVANCED_DESC),
	PropData_EditString_Check(PropId::NewLine         , RIPPPR_NEW_LINE_NAME, RIPPPR_NEW_LINE_DESC),
	PropData_CheckBox        (PropId::QuoteStrings    , RIPPPR_QUOTESTR_NAME, RIPPPR_QUOTESTR_DESC),
	PropData_CheckBox        (PropId::CaseSensitive   , RIPPPR_CASESENS_NAME, RIPPPR_CASESENS_DESC),
	PropData_CheckBox        (PropId::AllowEmptyGroups, RIPPPR_EMPTYGRP_NAME, RIPPPR_EMPTYGRP_DESC),
	PropData_CheckBox        (PropId::SubGroups       , RIPPPR_SUBGROUP_NAME, RIPPPR_SUBGROUP_DESC),
	PropData_ComboBox        (PropId::GroupRepeat     , RIPPPR_REPEATSG_NAME, RIPPPR_REPEATSG_DESC, std::data(group_repeats)),
	PropData_ComboBox        (PropId::ItemRepeat      , RIPPPR_REPEATSI_NAME, RIPPPR_REPEATSI_DESC, std::data(item_repeats)),
	PropData_CheckBox        (PropId::RepeatSave      , RIPPPR_SAVERPTS_NAME, RIPPPR_SAVERPTS_DESC),
	PropData_CheckBox        (PropId::EscapeChar_Group, RIPPPR_ESCAPESG_NAME, RIPPPR_ESCAPESG_DESC),
	PropData_CheckBox        (PropId::EscapeChar_Item , RIPPPR_ESCAPESI_NAME, RIPPPR_ESCAPESI_DESC),
	PropData_CheckBox        (PropId::EscapeChar_Value, RIPPPR_ESCAPESV_NAME, RIPPPR_ESCAPESV_DESC),
	PropData_End()
};
#endif

#ifdef FUSION_GET_PROPERTIES
auto FUSION_API GetProperties(mv* const mV, SerializedEditData* serialized_edit_data, fusion::boolean const main_item) noexcept
-> fusion::boolean
{
	mvInsertProps(mV, serialized_edit_data, props_general , PROPID_TAB_GENERAL, TRUE);
	mvInsertProps(mV, serialized_edit_data, props_advanced, PROPID_TAB_CUSTOM1, TRUE);
	return FUSION_GET_PROPERTIES_SUCCESS;
}
#endif
#ifdef FUSION_GET_PROPERTY_PARAM
auto FUSION_API GetPropCreateParam(mv* const mV, SerializedEditData* serialized_edit_data, std::int32_t const property_id) noexcept
-> std::int32_t
{
	return {};
}
#endif
#ifdef FUSION_FREE_PROPERTY_PARAM
void FUSION_API ReleasePropCreateParam(mv* const mV, SerializedEditData* serialized_edit_data, std::int32_t const property_id, std::int32_t const param) noexcept
{
}
#endif
#ifdef FUSION_FREE_PROPERTIES
void FUSION_API ReleaseProperties(mv* const mV, SerializedEditData* serialized_edit_data, fusion::boolean const main_item) noexcept
{
}
#endif

#ifdef FUSION_IS_PROPERTY_ENABLED
auto FUSION_API IsPropEnabled(mv* const mV, SerializedEditData* serialized_edit_data, std::int32_t const property_id) noexcept
-> fusion::boolean
{
	switch(property_id)
	{
		case PropId::Version: return FUSION_IS_PROPERTY_ENABLED_DISABLED;
		case PropId::NewLine         : [[fallthrough]];
		case PropId::QuoteStrings    : [[fallthrough]];
		case PropId::GroupRepeat     : [[fallthrough]];
		case PropId::ItemRepeat      : [[fallthrough]];
		case PropId::EscapeChar_Group: [[fallthrough]];
		case PropId::EscapeChar_Item : [[fallthrough]];
		case PropId::EscapeChar_Value: [[fallthrough]];
		case PropId::CaseSensitive   : [[fallthrough]];
		case PropId::RepeatSave      : [[fallthrough]];
		case PropId::SubGroups       : [[fallthrough]];
		case PropId::AllowEmptyGroups: return serialized_edit_data->deserialize(std::nullopt).bool_stdINI  ? FUSION_IS_PROPERTY_ENABLED_DISABLED : FUSION_IS_PROPERTY_ENABLED_ENABLED;
		case PropId::ReadOnly        : return serialized_edit_data->deserialize(std::nullopt).b_defaultFile? FUSION_IS_PROPERTY_ENABLED_ENABLED : FUSION_IS_PROPERTY_ENABLED_DISABLED;
		case PropId::GlobalDataName  : return serialized_edit_data->deserialize(std::nullopt).globalObject ? FUSION_IS_PROPERTY_ENABLED_ENABLED : FUSION_IS_PROPERTY_ENABLED_DISABLED;
		default: return FUSION_IS_PROPERTY_ENABLED_ENABLED;
	}
}
#endif

#ifdef FUSION_GET_PROPERTY_VALUE
auto FUSION_API GetPropValue(mv* const mV, SerializedEditData* serialized_edit_data, std::int32_t const property_id) noexcept
-> CPropValue*
{
	switch(property_id)
	{
		case PropId::Version       : return new CPropStringValue(_T("Unicode, ") _T(__DATE__) _T(" (v0.4.2-popgoes)"));
		case PropId::DefPath       : return new CPropStringValue(serialized_edit_data->deserialize(std::nullopt).defaultFile.c_str());
		case PropId::DefFolder     : return new CPropDWordValue (serialized_edit_data->deserialize(std::nullopt).defaultFolder);
		case PropId::DefaultText   : return new CPropStringValue(serialized_edit_data->deserialize(std::nullopt).defaultText.c_str());
		case PropId::Encrypt       : return new CPropStringValue(serialized_edit_data->deserialize(std::nullopt).encrypt_key.c_str());
		case PropId::NewLine       : return new CPropStringValue(serialized_edit_data->deserialize(std::nullopt).newline.c_str());
		case PropId::GroupRepeat   : return new CPropDWordValue (serialized_edit_data->deserialize(std::nullopt).repeatGroups);
		case PropId::ItemRepeat    : return new CPropDWordValue (serialized_edit_data->deserialize(std::nullopt).repeatItems);
		case PropId::GlobalDataName: return new CPropStringValue(serialized_edit_data->deserialize(std::nullopt).globalKey.c_str());
		case PropId::Undo          : return new CPropDWordValue (serialized_edit_data->deserialize(std::nullopt).undoCount);
		case PropId::Redo          : return new CPropDWordValue (serialized_edit_data->deserialize(std::nullopt).redoCount);
		default: assert(false); return nullptr;
		//TODO why does Fusion ask for property values for checkbox properties? the documentation says it shouldn't...
		case PropId::GlobalData      : return nullptr;
		case PropId::ReadOnly        : return nullptr;
		case PropId::AutoLoad        : return nullptr;
		case PropId::AutoSave        : return nullptr;
		case PropId::CreateFolders   : return nullptr;
		case PropId::Index           : return nullptr;
		case PropId::Compress        : return nullptr;
		case PropId::StandardQM      : return nullptr;
		case PropId::QuoteStrings    : return nullptr;
		case PropId::CaseSensitive   : return nullptr;
		case PropId::AllowEmptyGroups: return nullptr;
		case PropId::SubGroups       : return nullptr;
		case PropId::RepeatSave      : return nullptr;
		case PropId::EscapeChar_Group: return nullptr;
		case PropId::EscapeChar_Item : return nullptr;
		case PropId::EscapeChar_Value: return nullptr;
	}
}
#endif
#ifdef FUSION_GET_PROPERTY_CHECKBOX
auto FUSION_API GetPropCheck(mv* const mV, SerializedEditData* serialized_edit_data, std::int32_t const property_id) noexcept
-> fusion::boolean
{
	switch(property_id)
	{
		case PropId::DefPath         : return serialized_edit_data->deserialize(std::nullopt).b_defaultFile        ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::CreateFolders   : return serialized_edit_data->deserialize(std::nullopt).bool_CanCreateFolders? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::AutoSave        : return serialized_edit_data->deserialize(std::nullopt).bool_AutoSave        ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::StandardQM      : return serialized_edit_data->deserialize(std::nullopt).bool_stdINI          ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::Compress        : return serialized_edit_data->deserialize(std::nullopt).bool_compress        ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::Encrypt         : return serialized_edit_data->deserialize(std::nullopt).bool_encrypt         ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::NewLine         : return serialized_edit_data->deserialize(std::nullopt).bool_newline         ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::QuoteStrings    : return serialized_edit_data->deserialize(std::nullopt).bool_QuoteStrings    ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::EscapeChar_Group: return serialized_edit_data->deserialize(std::nullopt).bool_EscapeGroup     ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::EscapeChar_Item : return serialized_edit_data->deserialize(std::nullopt).bool_EscapeItem      ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::EscapeChar_Value: return serialized_edit_data->deserialize(std::nullopt).bool_EscapeValue     ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::CaseSensitive   : return serialized_edit_data->deserialize(std::nullopt).bool_CaseSensitive   ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::ReadOnly        : return serialized_edit_data->deserialize(std::nullopt).b_ReadOnly           ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::GlobalData      : return serialized_edit_data->deserialize(std::nullopt).globalObject         ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::RepeatSave      : return serialized_edit_data->deserialize(std::nullopt).saveRepeated         ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::Index           : return serialized_edit_data->deserialize(std::nullopt).index                ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::AutoLoad        : return serialized_edit_data->deserialize(std::nullopt).autoLoad             ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::SubGroups       : return serialized_edit_data->deserialize(std::nullopt).subGroups            ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		case PropId::AllowEmptyGroups: return serialized_edit_data->deserialize(std::nullopt).allowEmptyGroups     ? FUSION_GET_PROPERTY_CHECKBOX_TICKED : FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
		default: assert(false); return FUSION_GET_PROPERTY_CHECKBOX_UNTICKED;
	}
}
#endif
#ifdef FUSION_SET_PROPERTY_VALUE
template<std::derived_from<CPropValue> T>
static auto is_of_type(CPropValue* const v) noexcept
-> bool
{
	if(auto const temp{new (std::nothrow) T{}})
	{
		bool const result{temp->GetClassID() == v->GetClassID()};
		temp->Delete();
		return result;
	}
	else
	{
		return false;
	}
}
void FUSION_API SetPropValue(mv* const mV, SerializedEditData* serialized_edit_data, std::int32_t const property_id, CPropValue* const property_value) noexcept
{
	if(property_id < PROPID_EXTITEM_CUSTOM_FIRST)
	{
		return;
	}
	auto const update_str_prop{[&](lSDK::string_t EdittimeSettings::* const member)
	{
		if(is_of_type<CPropWStringValue>(property_value) || is_of_type<CPropDataValue>(property_value))
		{
			lSDK::string_view_t const str{static_cast<CPropWStringValue*>(property_value)->GetString()};
			auto edittime_settings{serialized_edit_data->deserialize(std::nullopt)};
			if((edittime_settings.*member) != str)
			{
				(edittime_settings.*member) = str;
				return SerializedEditData::serialize(mV, serialized_edit_data, edittime_settings);
			}
		}
		else if(is_of_type<CPropAStringValue>(property_value))
		{
			auto str{lSDK::wide_from_narrow(static_cast<CPropAStringValue*>(property_value)->GetString())};
			auto edittime_settings{serialized_edit_data->deserialize(std::nullopt)};
			if((edittime_settings.*member) != str)
			{
				(edittime_settings.*member) = std::move(str);
				return SerializedEditData::serialize(mV, serialized_edit_data, edittime_settings);
			}
		}
	}};
	auto const update_int_prop_impl{[&]<std::integral T>(T EdittimeSettings::* const member, std::integral auto const v)
	{
		auto edittime_settings{serialized_edit_data->deserialize(std::nullopt)};
		if((edittime_settings.*member) != v)
		{
			(edittime_settings.*member) = static_cast<T>(v);
			assert((edittime_settings.*member) == v);
			return SerializedEditData::serialize(mV, serialized_edit_data, edittime_settings);
		}
	}};
	auto const update_int_prop{[&](std::integral auto EdittimeSettings::* const member)
	{
		if(is_of_type<CPropDWordValue>(property_value))
		{
			return update_int_prop_impl(member, static_cast<CPropDWordValue*>(property_value)->m_dwValue);
		}
		else if(is_of_type<CPropIntValue>(property_value))
		{
			return update_int_prop_impl(member, static_cast<CPropIntValue*>(property_value)->m_nValue);
		}
		else if(is_of_type<CPropInt64Value>(property_value))
		{
			return update_int_prop_impl(member, static_cast<CPropInt64Value*>(property_value)->m_nValue);
		}
	}};
	switch(property_id)
	{
		case PropId::DefPath       : return update_str_prop(&EdittimeSettings::defaultFile);
		case PropId::DefaultText   : return update_str_prop(&EdittimeSettings::defaultText);
		case PropId::DefFolder     : return update_int_prop(&EdittimeSettings::defaultFolder);
		case PropId::Encrypt       : return update_str_prop(&EdittimeSettings::encrypt_key);
		case PropId::NewLine       : return update_str_prop(&EdittimeSettings::newline);
		case PropId::GroupRepeat   : return update_int_prop(&EdittimeSettings::repeatGroups);
		case PropId::ItemRepeat    : return update_int_prop(&EdittimeSettings::repeatItems);
		case PropId::GlobalDataName: return update_str_prop(&EdittimeSettings::globalKey);
		case PropId::Undo          : return update_int_prop(&EdittimeSettings::undoCount);
		case PropId::Redo          : return update_int_prop(&EdittimeSettings::redoCount);
		default: assert(false); return;
	}
}
#endif
#ifdef FUSION_SET_PROPERTY_CHECKBOX
void FUSION_API SetPropCheck(mv* const mV, SerializedEditData* serialized_edit_data, std::int32_t const property_id, fusion::boolean const ticked_) noexcept
{
	if(property_id < PROPID_EXTITEM_CUSTOM_FIRST)
	{
		return;
	}
	bool const ticked{ticked_ == FUSION_SET_PROPERTY_CHECKBOX_TICKED};
	auto const update_prop{[&](bool EdittimeSettings::* const member, std::initializer_list<std::int32_t> const& props_to_refresh = {})
	{
		auto edittime_settings{serialized_edit_data->deserialize(std::nullopt)};
		if((edittime_settings.*member) != ticked)
		{
			(edittime_settings.*member) = ticked;
			SerializedEditData::serialize(mV, serialized_edit_data, edittime_settings);
			for(std::int32_t const prop_to_refresh : props_to_refresh)
			{
				mvRefreshProp(mV, serialized_edit_data, prop_to_refresh, FALSE);
			}
		}
	}};
	switch(property_id)
	{
		case PropId::DefPath      : return update_prop(&EdittimeSettings::b_defaultFile, {PropId::ReadOnly});
		case PropId::CreateFolders: return update_prop(&EdittimeSettings::bool_CanCreateFolders);
		case PropId::AutoSave     : return update_prop(&EdittimeSettings::bool_AutoSave);
		case PropId::StandardQM   : return update_prop(&EdittimeSettings::bool_stdINI,
		{
			PropId::NewLine,
			PropId::QuoteStrings,
			PropId::GroupRepeat,
			PropId::EscapeChar_Group,
			PropId::ItemRepeat,
			PropId::EscapeChar_Item,
			PropId::EscapeChar_Value,
			PropId::CaseSensitive,
			PropId::RepeatSave,
			PropId::SubGroups,
			PropId::AllowEmptyGroups,
		});
		case PropId::Compress        : return update_prop(&EdittimeSettings::bool_compress);
		case PropId::Encrypt         : return update_prop(&EdittimeSettings::bool_encrypt);
		case PropId::NewLine         : return update_prop(&EdittimeSettings::bool_newline);
		case PropId::QuoteStrings    : return update_prop(&EdittimeSettings::bool_QuoteStrings);
		case PropId::EscapeChar_Group: return update_prop(&EdittimeSettings::bool_EscapeGroup);
		case PropId::EscapeChar_Item : return update_prop(&EdittimeSettings::bool_EscapeItem);
		case PropId::EscapeChar_Value: return update_prop(&EdittimeSettings::bool_EscapeValue);
		case PropId::CaseSensitive   : return update_prop(&EdittimeSettings::bool_CaseSensitive);
		case PropId::ReadOnly        : return update_prop(&EdittimeSettings::b_ReadOnly);
		case PropId::GlobalData      : return update_prop(&EdittimeSettings::globalObject, {PropId::GlobalDataName});
		case PropId::RepeatSave      : return update_prop(&EdittimeSettings::saveRepeated);
		case PropId::Index           : return update_prop(&EdittimeSettings::index);
		case PropId::AutoLoad        : return update_prop(&EdittimeSettings::autoLoad);
		case PropId::SubGroups       : return update_prop(&EdittimeSettings::subGroups);
		case PropId::AllowEmptyGroups: return update_prop(&EdittimeSettings::allowEmptyGroups);
		default: assert(false); return;
	}
}
#endif
#ifdef FUSION_PROPERTY_BUTTON_PRESSED
auto FUSION_API EditProp(mv* const mV, SerializedEditData* serialized_edit_data, std::int32_t const property_id) noexcept
-> fusion::boolean
{
	return FUSION_PROPERTY_BUTTON_PRESSED_CANCEL_CHANGES;
}
#endif
