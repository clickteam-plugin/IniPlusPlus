#include "FusionAPI.hpp"
#include "lSDK.hpp"
#include "lSDK/WindowsUtilities.hpp"

#include "CustomParams.hpp"
#include "Resources.h"

#include <array>
#include <cassert>
#include <cstddef>
#include <span>

using namespace std::string_view_literals;

static constexpr std::size_t const data_start{offsetof(paramExt, pextData)};
static constexpr std::size_t const max_data_size{PARAM_EXTMAXSIZE - data_start};

#ifdef FUSION_INITIALIZE_CUSTOM_PARAMETER
static void store_text(paramExt& param_data, lSDK::string_view_t const text)
{
	//because the ANSI version doesn't store any version data in the custom param data, we have to support the same format indefinitely
	auto const utf8text{lSDK::narrow_from_wide(text)};
	lSDK::fill_buffer(param_data.pextData, max_data_size, utf8text);
	param_data.pextSize = std::min(max_data_size, data_start + (std::size(utf8text) + 1));
}
static lSDK::string_t load_text(paramExt const& param_data)
{
	if(param_data.pextSize < data_start)
	{
		assert(false);
		return {};
	}
	lSDK::string_view8_t const utf8text{param_data.pextData, param_data.pextSize - data_start};
	return lSDK::wide_from_narrow(utf8text.substr(0, utf8text.find('\0')));
}
void FUSION_API InitParameter(mv* const mV, std::int16_t const custom_id, paramExt* const param_data) noexcept
{
	assert(custom_id == param_data->pextCode);
	std::ignore = ::SecureZeroMemory(param_data->pextData, max_data_size);
	switch(custom_id)
	{
		case PARAM_OBJECTSELECTOR : store_text(*param_data, lSDK::get_resource_string(RIPP_NAME)); break;
		case PARAM_OBJECTSELECTOR2: store_text(*param_data, lSDK::get_resource_string(RIPP_NAME)); break;
		case PARAM_OBJECTSELECTOR3: store_text(*param_data, lSDK::get_resource_string(RIPPAM_CHART_OBJECT)); break;
		case PARAM_DIALOGSELECTOR : param_data->pextSize = data_start + sizeof(std::uint16_t)*2; break;
		case PARAM_SSS            : param_data->pextSize = data_start + sizeof(std::int8_t)*2; break;
		case PARAM_SEARCH         : param_data->pextSize = data_start + sizeof(std::int8_t)*2; break;
		case PARAM_OBJECTSELECTOR4: param_data->pextSize = data_start; break;
		case PARAM_OBJECTSELECTOR5: param_data->pextSize = data_start; break;
		case PARAM_LOADFILE       : param_data->pextSize = data_start + sizeof(std::int8_t)*4; break;
		default: assert(false); break;
	}
}
#endif

#ifdef FUSION_EDIT_CUSTOM_PARAMETER
struct ParamInfo final
{
	paramExt& p;
	mv& mV;
};
::BOOL CALLBACK   ObjectSelector(::HWND, ::UINT, ::WPARAM, ::LPARAM) noexcept;
::BOOL CALLBACK   DialogSelector(::HWND, ::UINT, ::WPARAM, ::LPARAM) noexcept;
::BOOL CALLBACK      SSSSettings(::HWND, ::UINT, ::WPARAM, ::LPARAM) noexcept;
::BOOL CALLBACK   SearchSettings(::HWND, ::UINT, ::WPARAM, ::LPARAM) noexcept;
::BOOL CALLBACK LoadFileSettings(::HWND, ::UINT, ::WPARAM, ::LPARAM) noexcept;
void FUSION_API EditParameter(mv* const mV, std::int16_t const custom_id, paramExt* const param_data) noexcept
{
	assert(custom_id == param_data->pextCode);
	ParamInfo pi{*param_data, *mV};
	switch(custom_id)
	{
		case PARAM_OBJECTSELECTOR : [[fallthrough]];
		case PARAM_OBJECTSELECTOR2: [[fallthrough]];
		case PARAM_OBJECTSELECTOR3: [[fallthrough]];
		case PARAM_OBJECTSELECTOR4: [[fallthrough]];
		case PARAM_OBJECTSELECTOR5: std::ignore = ::DialogBoxParam(fusion::DLL, MAKEINTRESOURCE(IDD_OBJECTSELECT), mV->mvHEditWin,   ObjectSelector, reinterpret_cast<LPARAM>(&pi)); break;
		case PARAM_DIALOGSELECTOR : std::ignore = ::DialogBoxParam(fusion::DLL, MAKEINTRESOURCE(IDD_OPENDIALOG  ), mV->mvHEditWin,   DialogSelector, reinterpret_cast<LPARAM>(&pi)); break;
		case PARAM_SSS            : std::ignore = ::DialogBoxParam(fusion::DLL, MAKEINTRESOURCE(IDD_SSS         ), mV->mvHEditWin,      SSSSettings, reinterpret_cast<LPARAM>(&pi)); break;
		case PARAM_SEARCH         : std::ignore = ::DialogBoxParam(fusion::DLL, MAKEINTRESOURCE(IDD_SEARCH      ), mV->mvHEditWin,   SearchSettings, reinterpret_cast<LPARAM>(&pi)); break;
		case PARAM_LOADFILE       : std::ignore = ::DialogBoxParam(fusion::DLL, MAKEINTRESOURCE(IDD_LOADMODE    ), mV->mvHEditWin, LoadFileSettings, reinterpret_cast<LPARAM>(&pi)); break;
		default: assert(false); break;
	}
}
#endif

#ifdef FUSION_GET_CUSTOM_PARAMETER_DISPLAY_STRING
static void append_str(std::span<lSDK::char_t>& buffer, lSDK::string_view_t const s)
{
	if(std::size(buffer) <= 1)
	{
		return;
	}
	std::size_t const copy_len{std::min(std::size(buffer) - 1, std::size(s))};
	std::ignore = std::memcpy(std::data(buffer), std::data(s), copy_len*sizeof(lSDK::char_t));
	buffer = buffer.subspan(copy_len);
	buffer.front() = TSL('\0');
}
void FUSION_API GetParameterString(mv* const mV, std::int16_t const custom_id, paramExt* const param_data, fusion::string_buffer const display_string_buffer, std::int16_t const display_string_buffer_size) noexcept
{
	assert(custom_id == param_data->pextCode);
	if(display_string_buffer_size <= 0)
	{
		return;
	}
	*display_string_buffer = TSL('\0');
	std::span dsb{display_string_buffer, static_cast<std::size_t>(display_string_buffer_size)};
	auto const report_corruption{[=]
	{
		lSDK::fill_buffer(display_string_buffer, display_string_buffer_size, TSL("<corrupt parameter>"sv));
	}};
	if(param_data->pextSize < data_start)
	{
		return report_corruption();
	}
	std::size_t const data_size{param_data->pextSize - data_start};
	switch(custom_id)
	{
		case PARAM_OBJECTSELECTOR : [[fallthrough]];
		case PARAM_OBJECTSELECTOR2: [[fallthrough]];
		case PARAM_OBJECTSELECTOR3: [[fallthrough]];
		case PARAM_OBJECTSELECTOR4: [[fallthrough]];
		case PARAM_OBJECTSELECTOR5: lSDK::fill_buffer(display_string_buffer, display_string_buffer_size, load_text(*param_data)); break;
		case PARAM_DIALOGSELECTOR:
		{
			if(data_size < sizeof(std::uint16_t)*2)
			{
				return report_corruption();
			}
			auto const flags {*reinterpret_cast<std::uint16_t const*>(param_data->pextData)};
			auto const format{*reinterpret_cast<std::uint16_t const*>(param_data->pextData + sizeof(std::uint16_t))};
			static constexpr std::array const FLAG_NAMES
			{
				RIPPCP_DIALOG_NO_DELETING_GROUPS,
				RIPPCP_DIALOG_NO_DELETING_ITEMS,
				RIPPCP_DIALOG_NO_RENAMING_GROUPS,
				RIPPCP_DIALOG_NO_RENAMING_ITEMS,
				RIPPCP_DIALOG_NO_CHANGING_VALUES,
				RIPPCP_DIALOG_NO_REFRESH_BUTTON,
				RIPPCP_DIALOG_NO_NEW_GROUPS,
				RIPPCP_DIALOG_NO_NEW_ITEMS,
				RIPPCP_DIALOG_NO_AUTO_REFRESH,
				RIPPCP_DIALOG_PAUSE_APP,
			};
			static constexpr std::array const FORMAT_NAMES
			{
				RIPPCD_DIALOG_ITEM_FORMATTING_A,
				RIPPCD_DIALOG_ITEM_FORMATTING_B,
				RIPPCD_DIALOG_ITEM_FORMATTING_C,
				RIPPCD_DIALOG_ITEM_FORMATTING_D,
				RIPPCD_DIALOG_ITEM_FORMATTING_E,
				RIPPCD_DIALOG_ITEM_FORMATTING_F,
				RIPPCD_DIALOG_ITEM_FORMATTING_G,
				RIPPCD_DIALOG_ITEM_FORMATTING_H,
			};
			if(flags >= (decltype(flags){1} << std::size(FLAG_NAMES))
			|| format >= std::size(FORMAT_NAMES))
			{
				return report_corruption();
			}
			lSDK::fill_buffer(display_string_buffer, display_string_buffer_size, lSDK::get_resource_string(RIPPCP_DIALOG_NONE));
			for(std::size_t n{0}; n < std::size(FLAG_NAMES); ++n)
			{
				if(flags & (decltype(flags){1} << n))
				{
					append_str(dsb, lSDK::get_resource_string(FLAG_NAMES.at(n)));
					append_str(dsb, lSDK::get_resource_string(RIPPCP_COMMA));
				}
			}
			append_str(dsb, lSDK::get_resource_string(FORMAT_NAMES.at(format)));
		} break;
		case PARAM_SSS:
		{
			if(data_size < sizeof(std::int8_t)*2)
			{
				return report_corruption();
			}
			auto const operation{static_cast<std::size_t>(param_data->pextData[0])};
			auto const order    {static_cast<std::size_t>(param_data->pextData[1])};
			static constexpr std::array const OPERATION_NAMES
			{
				std::make_pair(RIPPCP_SSS_SORT_BY_VALUE, true),
				std::make_pair(RIPPCP_SSS_SHUFFLE, false),
				std::make_pair(RIPPCP_SSS_REMOVE_FIRST, false),
				std::make_pair(RIPPCP_SSS_CYCLE_UP, false),
				std::make_pair(RIPPCP_SSS_REMOVE_LAST, false),
				std::make_pair(RIPPCP_SSS_CYCLE_DOWN, false),
				std::make_pair(RIPPCP_SSS_REVERSE, false),
				std::make_pair(RIPPCP_SSS_SWAP_ITEMS_VALUES_IN, false),
				std::make_pair(RIPPCP_SSS_RENUMBER, false),
				std::make_pair(RIPPCP_SSS_SORT_BY_NAME, true),
				std::make_pair(RIPPCP_SSS_SORT_BY_FIRST_PART, true),
			};
			static constexpr std::array const SORT_ORDER_NAMES
			{
				RIPPCP_SSS_SORT_ORDER_ALPHANUM,
				RIPPCP_SSS_SORT_ORDER_STANDARD_CASE_INSENSITIVE,
				RIPPCP_SSS_SORT_ORDER_STANDARD_CASE_SENSITIVE,
			};
			if(operation < std::size( OPERATION_NAMES)
			&& order     < std::size(SORT_ORDER_NAMES))
			{
				append_str(dsb, lSDK::get_resource_string(OPERATION_NAMES.at(operation).first));
				if(OPERATION_NAMES.at(operation).second)
				{
					append_str(dsb, lSDK::get_resource_string(SORT_ORDER_NAMES.at(order)));
				}
			}
			else
			{
				return report_corruption();
			}
		} break;
		case PARAM_SEARCH:
		{
			if(data_size < sizeof(std::int8_t)*2)
			{
				return report_corruption();
			}
			auto const mode{static_cast<std::size_t>(param_data->pextData[0])};
			auto const flags{param_data->pextData[1]};
			static constexpr std::array const SEARCH_MODE_NAMES
			{
				RIPPCP_SEARCH_MODE_DISREGARD,
				RIPPCP_SEARCH_MODE_OR,
				RIPPCP_SEARCH_MODE_AND,
				RIPPCP_SEARCH_MODE_XOR,
			};
			static constexpr std::array const SEARCH_SETTING_NAMES
			{
				RIPPCP_SEARCH_OPTION_UNUSED,
				RIPPCP_SEARCH_OPTION_ONLY_GROUP_NAMES,
				RIPPCP_SEARCH_OPTION_MATCH_ANY,
				RIPPCP_SEARCH_OPTION_CASE_SENSITIVE,
			};
			if(mode < std::size(SEARCH_MODE_NAMES)
			&& flags < (decltype(flags){1} << std::size(SEARCH_SETTING_NAMES)))
			{
				append_str(dsb, lSDK::get_resource_string(RIPPCP_SEARCH_MODE));
				append_str(dsb, lSDK::get_resource_string(SEARCH_MODE_NAMES.at(mode)));
				if(flags)
				{
					append_str(dsb, lSDK::get_resource_string(RIPPCP_SEARCH_OPTIONS));
					bool first{true};
					for(std::size_t n{0}; n < std::size(SEARCH_SETTING_NAMES); ++n)
					{
						if(flags & (decltype(flags){1} << n))
						{
							if(!first)
							{
								append_str(dsb, lSDK::get_resource_string(RIPPCP_COMMA));
							}
							else
							{
								first = false;
							}
							append_str(dsb, lSDK::get_resource_string(SEARCH_SETTING_NAMES.at(n)));
						}
					}
					append_str(dsb, lSDK::get_resource_string(RIPPCP_SEARCH_OPTIONS_END));
				}
			}
			else
			{
				return report_corruption();
			}
		} break;
		case PARAM_LOADFILE:
		{
			if(data_size < sizeof(std::int8_t)*4)
			{
				return report_corruption();
			}
			auto const   load_mode{static_cast<std::size_t>(param_data->pextData[0])};
			auto const change_mode{static_cast<std::size_t>(param_data->pextData[1])};
			auto const  clear_mode{static_cast<std::size_t>(param_data->pextData[2])};
			auto const flags{param_data->pextData[3]};
			static constexpr std::array const LOAD_MODE_NAMES
			{
				RIPPCP_NLC_LOAD,
				RIPPCP_NLC_CHANGE,
			};
			static constexpr std::array const CHANGE_MODE_NAMES
			{
				RIPPCP_NLC_CHANGE_AND,
				RIPPCP_NLC_KEEP_PATH_AND,
				RIPPCP_NLC_CHANGE_IF_EXISTS_AND,
			};
			static constexpr std::array const CLEAR_MODE_NAMES
			{
				RIPPCP_NLC_CLEAR_DATA,
				RIPPCP_NLC_KEEP_DATA,
				RIPPCP_NLC_CLEAR_DATA_IF_FILE_EXISTS,
			};
			static constexpr std::array const FLAG_NAMES
			{
				RIPPCP_NLC_SAVE_IMMEDIATELY,
				RIPPCP_NLC_READ_ONLY,
			};
			if(  load_mode < std::size(  LOAD_MODE_NAMES)
			&& change_mode < std::size(CHANGE_MODE_NAMES)
			&&  clear_mode < std::size( CLEAR_MODE_NAMES)
			&& flags < (decltype(flags){1} << std::size(FLAG_NAMES)))
			{
				append_str(dsb, lSDK::get_resource_string(LOAD_MODE_NAMES.at(load_mode)));
				if(load_mode == 0)
				{
					append_str(dsb, lSDK::get_resource_string(CHANGE_MODE_NAMES.at(change_mode)));
				}
				append_str(dsb, lSDK::get_resource_string(CLEAR_MODE_NAMES.at(clear_mode)));
				for(std::size_t n{0}; n < std::size(FLAG_NAMES); ++n)
				{
					if(flags & (decltype(flags){1} << n))
					{
						append_str(dsb, lSDK::get_resource_string(FLAG_NAMES.at(n)));
					}
				}
			}
			else
			{
				return report_corruption();
			}
		} break;
	}
}
#endif

#ifndef FUSION_RUNTIME_ONLY

::BOOL CALLBACK ObjectSelector(::HWND const hDlg, ::UINT const msgType, ::WPARAM const wParam, ::LPARAM const lParam) noexcept
{
	switch(msgType)
	{
		case WM_INITDIALOG:
		{
			::SetWindowLong(hDlg, DWL_USER, lParam);
			auto const& pi{*reinterpret_cast<ParamInfo const*>(lParam)};

			::SetDlgItemText(hDlg, IDC_EDIT, load_text(pi.p).c_str());

			switch(pi.p.pextCode)
			{
				case PARAM_OBJECTSELECTOR : ::SetWindowText(::GetDlgItem(hDlg, IDC_INFO), std::data(lSDK::get_resource_string(RIPPCD_OBJECTSELECTOR1))); break;
				case PARAM_OBJECTSELECTOR2: ::SetWindowText(::GetDlgItem(hDlg, IDC_INFO), std::data(lSDK::get_resource_string(RIPPCD_OBJECTSELECTOR2))); break;
				case PARAM_OBJECTSELECTOR3: ::SetWindowText(::GetDlgItem(hDlg, IDC_INFO), std::data(lSDK::get_resource_string(RIPPCD_OBJECTSELECTOR3))); break;
				case PARAM_OBJECTSELECTOR4: ::SetWindowText(::GetDlgItem(hDlg, IDC_INFO), std::data(lSDK::get_resource_string(RIPPCD_OBJECTSELECTOR4))); break;
				case PARAM_OBJECTSELECTOR5: ::SetWindowText(::GetDlgItem(hDlg, IDC_INFO), std::data(lSDK::get_resource_string(RIPPCD_OBJECTSELECTOR5))); break;
				default: assert(false);     ::SetWindowText(::GetDlgItem(hDlg, IDC_INFO), _T("<unknown custom parameter type>")); break;
			}

			return TRUE;
		} break;
		case WM_COMMAND:
		{
			auto& pi{*reinterpret_cast<ParamInfo*>(::GetWindowLong(hDlg, DWL_USER))};

			switch(wmCommandID)
			{
				case IDOK:
				{
					auto const len{::GetWindowTextLength(::GetDlgItem(hDlg, IDC_EDIT))};
					std::basic_string<TCHAR> text(static_cast<std::size_t>(len + 1), _T('\0'));
					auto const result{::GetDlgItemText(hDlg, IDC_EDIT, std::data(text), std::size(text))};
					text.resize(result);
					store_text(pi.p, text);
					::EndDialog(hDlg, TRUE);
				} break;
				case IDCANCEL:
				{
					::EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
			}
		} break;
	}
	return FALSE;
}
::BOOL CALLBACK DialogSelector(::HWND const hDlg, ::UINT const msgType, ::WPARAM const wParam, ::LPARAM const lParam) noexcept
{
	switch(msgType)
	{
		case WM_INITDIALOG:
		{
			::SetWindowLong(hDlg, DWL_USER, lParam);
			auto& pi{*reinterpret_cast<ParamInfo const*>(lParam)};
			auto& flags {*reinterpret_cast<std::uint16_t const*>(pi.p.pextData)};
			auto& format{*reinterpret_cast<std::uint16_t const*>(pi.p.pextData + sizeof(std::uint16_t))};

			for(::UINT n{IDC_CHECK1}; n <= IDC_CHECK10; ++n)
			{
				::SendDlgItemMessage(hDlg, n, BM_SETCHECK, (flags & (decltype(flags){1} << (n - IDC_CHECK1)))? BST_CHECKED : BST_UNCHECKED, 0);
			}

			for(::UINT i{RIPPCD_DIALOG_ITEM_FORMATTING_A}; i <= RIPPCD_DIALOG_ITEM_FORMATTING_H; ++i)
			{
				::SendDlgItemMessage(hDlg, IDC_COMBO, CB_ADDSTRING, 0, reinterpret_cast<::LPARAM>(std::data(lSDK::get_resource_string(i))));
			}

			::SendDlgItemMessage(hDlg, IDC_COMBO, CB_SETCURSEL, format, 0);
			::SendMessage(hDlg, WM_COMMAND, IDC_CHECK10, 0);

			return TRUE;
		} break;
		case WM_COMMAND:
		{
			auto& pi{*reinterpret_cast<ParamInfo*>(::GetWindowLong(hDlg, DWL_USER))};
			auto& flags {*reinterpret_cast<std::uint16_t*>(pi.p.pextData)};
			auto& format{*reinterpret_cast<std::uint16_t*>(pi.p.pextData + sizeof(std::uint16_t))};
			switch(wmCommandID)
			{
				case IDOK:
				{ 
					flags = 0;
					for(::UINT n{IDC_CHECK1}; n <= IDC_CHECK10; ++n)
					{
						if(::SendDlgItemMessage(hDlg, n, BM_GETCHECK, 0, 0) == BST_CHECKED)
						{
							flags |= (std::uint16_t{1} << (n - IDC_CHECK1));
						}
					}
					format = ::SendDlgItemMessage(hDlg, IDC_COMBO, CB_GETCURSEL, 0, 0);
					pi.p.pextSize = data_start + sizeof(std::uint16_t)*2;

					::EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
				case IDCANCEL:
				{
					::EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
				case IDC_CHECK10:
				{
					if(BST_CHECKED == ::SendDlgItemMessage(hDlg, IDC_CHECK10, BM_GETCHECK, 0, 0))
					{
						::SendDlgItemMessage(hDlg, IDC_CHECK6, BM_SETCHECK, BST_CHECKED, 0);
						::SendDlgItemMessage(hDlg, IDC_CHECK9, BM_SETCHECK, BST_CHECKED, 0);
						::EnableWindow(::GetDlgItem(hDlg, IDC_CHECK6), FALSE);
						::EnableWindow(::GetDlgItem(hDlg, IDC_CHECK9), FALSE);
					}
					else
					{
						::EnableWindow(::GetDlgItem(hDlg, IDC_CHECK6), TRUE);
						::EnableWindow(::GetDlgItem(hDlg, IDC_CHECK9), TRUE);
					}
				} break;
			}
		} break;
	}
	return FALSE;
}
::BOOL CALLBACK SSSSettings(::HWND const hDlg, ::UINT const msgType, ::WPARAM const wParam, ::LPARAM const lParam) noexcept
{
	static constexpr std::array const DESCRIPTIONS
	{
		RIPPCD_SSS_A,
		RIPPCD_SSS_B,
		RIPPCD_SSS_C,
		RIPPCD_SSS_D,
		RIPPCD_SSS_E,
		RIPPCD_SSS_F,
		RIPPCD_SSS_G,
		RIPPCD_SSS_H,
		RIPPCD_SSS_I,
		RIPPCD_SSS_J,
		RIPPCD_SSS_K,
	};
	switch(msgType)
	{
		case WM_INITDIALOG:
		{
			::SetWindowLong(hDlg, DWL_USER, lParam);
			auto& pi{*reinterpret_cast<ParamInfo const*>(lParam)};
			auto& n0{pi.p.pextData[0]};
			auto& n1{pi.p.pextData[1]};

			if(n0 < std::size(DESCRIPTIONS))
			{
				::SendDlgItemMessage(hDlg, IDC_DESCRIPTIONTEXT, WM_SETTEXT, 0, reinterpret_cast<::LPARAM>(std::data(lSDK::get_resource_string(DESCRIPTIONS.at(n0)))));
				::SendDlgItemMessage(hDlg, IDC_RADIO+n0, BM_SETCHECK, BST_CHECKED, 0);
			}
			else
			{
				::SendDlgItemMessage(hDlg, IDC_DESCRIPTIONTEXT, WM_SETTEXT, 0, reinterpret_cast<::LPARAM>(_T("<invalid selection>")));
			}
			if(n1 < 3) //TODO: eliminate magic number
			{
				::SendDlgItemMessage(hDlg, IDC_SORTINGRADIO1+n1, BM_SETCHECK, BST_CHECKED, 0);
			}
			switch(IDC_RADIO+n0)
			{
				case IDC_RADIO  : [[fallthrough]];
				case IDC_RADIO10: [[fallthrough]];
				case IDC_RADIO11:
				{
					::EnableWindow(::GetDlgItem(hDlg, IDC_SORTINGRADIO1), TRUE);
					::EnableWindow(::GetDlgItem(hDlg, IDC_SORTINGRADIO2), TRUE);
					::EnableWindow(::GetDlgItem(hDlg, IDC_SORTINGRADIO3), TRUE);
				} break;
				default:
				{
					::EnableWindow(::GetDlgItem(hDlg, IDC_SORTINGRADIO1), FALSE);
					::EnableWindow(::GetDlgItem(hDlg, IDC_SORTINGRADIO2), FALSE);
					::EnableWindow(::GetDlgItem(hDlg, IDC_SORTINGRADIO3), FALSE);
				} break;
			}

			return TRUE;
		} break;
		case WM_COMMAND:
		{
			auto& pi{*reinterpret_cast<ParamInfo*>(::GetWindowLong(hDlg, DWL_USER))};

			switch(wmCommandID)
			{
				case IDOK:
				{
					auto& n0{pi.p.pextData[0]};
					auto& n1{pi.p.pextData[1]};
					for(::UINT n{IDC_RADIO}; n <= IDC_RADIO11; ++n)
					{
						if(BST_CHECKED == ::SendDlgItemMessage(hDlg, n, BM_GETCHECK, 0, 0))
						{
							n0 = n - IDC_RADIO;
							break;
						}
					}
					for(::UINT n{IDC_SORTINGRADIO1}; n <= IDC_SORTINGRADIO3; ++n)
					{
						if(BST_CHECKED == ::SendDlgItemMessage(hDlg, n, BM_GETCHECK, 0, 0))
						{
							n1 = n - IDC_SORTINGRADIO1;
							break;
						}
					}
					pi.p.pextSize = data_start + sizeof(std::int8_t)*2;

					::EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
				case IDCANCEL:
				{
					::EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
				default:
				{
					for(::UINT n{IDC_RADIO}; n <= IDC_RADIO11; ++n)
					{
						if(BST_CHECKED == ::SendDlgItemMessage(hDlg, n, BM_GETCHECK, 0, 0))
						{
							::SendDlgItemMessage(hDlg, IDC_DESCRIPTIONTEXT, WM_SETTEXT, 0, reinterpret_cast<::LPARAM>(std::data(lSDK::get_resource_string(DESCRIPTIONS.at(n - IDC_RADIO)))));
							switch(n)
							{
								case IDC_RADIO  : [[fallthrough]];
								case IDC_RADIO10: [[fallthrough]];
								case IDC_RADIO11:
								{
									::EnableWindow(::GetDlgItem(hDlg, IDC_SORTINGRADIO1), TRUE);
									::EnableWindow(::GetDlgItem(hDlg, IDC_SORTINGRADIO2), TRUE);
									::EnableWindow(::GetDlgItem(hDlg, IDC_SORTINGRADIO3), TRUE);
								} break;
								default:
								{
									::EnableWindow(::GetDlgItem(hDlg, IDC_SORTINGRADIO1), FALSE);
									::EnableWindow(::GetDlgItem(hDlg, IDC_SORTINGRADIO2), FALSE);
									::EnableWindow(::GetDlgItem(hDlg, IDC_SORTINGRADIO3), FALSE);
								} break;
							}
						}
					}
				} break;
			}
		} break;
	}
	return FALSE;
}
::BOOL CALLBACK SearchSettings(::HWND const hDlg, ::UINT const msgType, ::WPARAM const wParam, ::LPARAM const lParam) noexcept
{
	switch(msgType)
	{
		case WM_INITDIALOG:
		{
			::SetWindowLong(hDlg, DWL_USER, lParam);
			auto const& pi{*reinterpret_cast<ParamInfo const*>(lParam)};
			auto const& n0{pi.p.pextData[0]};
			auto const& n1{pi.p.pextData[1]};

			::SendDlgItemMessage(hDlg, IDC_RADIO+n0, BM_SETCHECK, BST_CHECKED, 0);
			for(int n{0}; n < 3 /*TODO*/; ++n)
			{
				if(n1 & (1 << (n+1)))
				{
					::SendDlgItemMessage(hDlg, IDC_CHECK2+n, BM_SETCHECK, BST_CHECKED, 0);
				}
			}

			return TRUE;
		} break;
		case WM_COMMAND:
		{
			auto& pi{*reinterpret_cast<ParamInfo*>(::GetWindowLong(hDlg, DWL_USER))};
			auto& n0{pi.p.pextData[0]};
			auto& n1{pi.p.pextData[1]};
			switch(wmCommandID)
			{
				case IDOK:
				{
					for(n0 = 0; n0 < 4 /*TODO*/; ++n0)
					{
						if(BST_CHECKED == ::SendDlgItemMessage(hDlg, IDC_RADIO+n0, BM_GETCHECK, 0, 0))
						{
							break;
						}
					}
					n1 = 0;
					for(int n{0}; n < 3 /*TODO*/; ++n)
					{
						if(BST_CHECKED == ::SendDlgItemMessage(hDlg, IDC_CHECK2+n, BM_GETCHECK, 0, 0))
						{
							n1 |= (1 << (n+1));
						}
					}
					pi.p.pextSize = data_start + sizeof(std::int8_t)*2;

					::EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
				case IDCANCEL:
				{
					::EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
			}
		} break;
	}
	return FALSE;
}
::BOOL CALLBACK LoadFileSettings(::HWND const hDlg, ::UINT const msgType, ::WPARAM const wParam, ::LPARAM const lParam) noexcept
{
	switch(msgType)
	{
		case WM_INITDIALOG:
		{
			::SetWindowLong(hDlg, DWL_USER, lParam);
			auto const& pi{*reinterpret_cast<ParamInfo const*>(lParam)};
			auto const& n0{pi.p.pextData[0]};
			auto const& n1{pi.p.pextData[1]};
			auto const& n2{pi.p.pextData[2]};
			auto const& n3{pi.p.pextData[3]};

			::SendDlgItemMessage(hDlg, IDC_RADIO1+n0, BM_SETCHECK, BST_CHECKED, 0);
			::SendDlgItemMessage(hDlg, IDC_RADIO5+n1, BM_SETCHECK, BST_CHECKED, 0);
			::SendDlgItemMessage(hDlg, IDC_RADIO8+n2, BM_SETCHECK, BST_CHECKED, 0);
			if(n3 & (1 << 0))
			{
				::SendDlgItemMessage(hDlg, IDC_CHECK1, BM_SETCHECK, BST_CHECKED, 0);
			}
			if(n3 & (1 << 1))
			{
				::SendDlgItemMessage(hDlg, IDC_CHECK2, BM_SETCHECK, BST_CHECKED, 0);
			}

			LoadFileSettings(hDlg, WM_COMMAND, 0, 0);

			return TRUE;
		} break;
		case WM_COMMAND:
		{
			auto& pi{*reinterpret_cast<ParamInfo*>(::GetWindowLong(hDlg, DWL_USER))};
			auto& n0{pi.p.pextData[0]};
			auto& n1{pi.p.pextData[1]};
			auto& n2{pi.p.pextData[2]};
			auto& n3{pi.p.pextData[3]};
			switch(wmCommandID)
			{
				case IDOK:
				{
					for(n0 = 0; n0 < 3 /*TODO*/; ++n0)
					{
						if(BST_CHECKED == ::SendDlgItemMessage(hDlg, IDC_RADIO1+n0, BM_GETCHECK, 0, 0))
						{
							break;
						}
					}
					for(n1 = 0; n1 < 3 /*TODO*/; ++n1)
					{
						if(BST_CHECKED == ::SendDlgItemMessage(hDlg, IDC_RADIO5+n1, BM_GETCHECK, 0, 0))
						{
							break;
						}
					}
					for(n2 = 0; n2 < 3 /*TODO*/; ++n2)
					{
						if(BST_CHECKED == ::SendDlgItemMessage(hDlg, IDC_RADIO8+n2, BM_GETCHECK, 0, 0))
						{
							break;
						}
					}
					n3 = 0;
					if(BST_CHECKED == ::SendDlgItemMessage(hDlg, IDC_CHECK1, BM_GETCHECK, 0, 0))
					{
						n3 |= (1 << 0);
					}
					if(BST_CHECKED == ::SendDlgItemMessage(hDlg, IDC_CHECK2, BM_GETCHECK, 0, 0))
					{
						n3 |= (1 << 1);
					}
					pi.p.pextSize = data_start + sizeof(std::int8_t)*4;

					::EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
				case IDCANCEL:
				{
					::EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
				default:
				{
					for(int n{0}; n < 4 /*TODO*/; ++n)
					{
						if(BST_CHECKED == ::SendDlgItemMessage(hDlg, IDC_RADIO1+n, BM_GETCHECK, 0, 0))
						{
							if(n == 0 /*TODO*/)
							{
								::EnableWindow(::GetDlgItem(hDlg, IDC_RADIO6), TRUE);
								::EnableWindow(::GetDlgItem(hDlg, IDC_RADIO7), TRUE);
								::EnableWindow(::GetDlgItem(hDlg, IDC_RADIO10), TRUE);
							}
							else
							{
								::EnableWindow(::GetDlgItem(hDlg, IDC_RADIO6), FALSE);
								::EnableWindow(::GetDlgItem(hDlg, IDC_RADIO7), FALSE);
								::EnableWindow(::GetDlgItem(hDlg, IDC_RADIO10), FALSE);

								if(BST_UNCHECKED == ::SendDlgItemMessage(hDlg, IDC_RADIO5, BM_GETCHECK, 0, 0))
								{
									::SendDlgItemMessage(hDlg, IDC_RADIO5, BM_SETCHECK, BST_CHECKED, 0);
									::SendDlgItemMessage(hDlg, IDC_RADIO6, BM_SETCHECK, BST_UNCHECKED, 0);
									::SendDlgItemMessage(hDlg, IDC_RADIO7, BM_SETCHECK, BST_UNCHECKED, 0);
								}
								if(BST_CHECKED == ::SendDlgItemMessage(hDlg, IDC_RADIO10, BM_GETCHECK, 0, 0))
								{
									::SendDlgItemMessage(hDlg, IDC_RADIO8, BM_SETCHECK, BST_CHECKED, 0);
									::SendDlgItemMessage(hDlg, IDC_RADIO10, BM_SETCHECK, BST_UNCHECKED, 0);
								}
							}
							break;
						}
					}
				} break;
			}
		} break;
	}
	return FALSE;
}

#endif //FUSION_RUNTIME_ONLY
