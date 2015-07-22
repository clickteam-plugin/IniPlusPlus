/* CustomParams.cpp
 * Just as with all the other SDKs, EDIF
 * supports the creation and use of custom,
 * you-defined parameters. Your own dialog,
 * your own display string, your own sense
 * of control. Read the MMF2SDK Help file
 * for more information. To use custom
 * parameters with EDIF, the parameter type
 * must be "Custom" or "Custom#" where # is
 * the number of the custom parameter.
 * Functions defined here:
 * InitParameter
 * EditParameter
 * GetParameterString
 */

#include "Common.h"

/* InitParameter
 * Like with the editdata, this data will
 * be written as-is to disk and later read
 * back. The most you can store is defined
 * by PARAM_EXTMAXSIZE, which currently is
 * 512 bytes. Use them wisely!
 */
void MMF2Func InitParameter(mv *mV, short ID, paramExt *p) //TODO: cleanup
{
#ifndef RUN_ONLY
	//store data in p->pextData
	//store the size of the data in p->pextSize
	switch(ID - PARAM_EXTBASE)
	{
	case 0: //object selector 1
	case 1: //object selector 2
		{
			_tcscpy((TCHAR *)&(p->pextData[0]), _T("Ini++ v1.6"));
			p->pextSize = sizeof(paramExt) - sizeof(p->pextData) + (_tcslen((TCHAR *)(p->pextData))+1)*sizeof(TCHAR);
		} break;
	case 3: //object selector 3
		{
			_tcscpy((TCHAR *)&(p->pextData[0]), _T("Chart"));
			p->pextSize = sizeof(paramExt) - sizeof(p->pextData) + (_tcslen((TCHAR *)(p->pextData))+1)*sizeof(TCHAR);
		} break;
	case 2: //dialog selector
		{
			p->pextSize = sizeof(paramExt) - sizeof(p->pextData) + sizeof(int);
			(*((int *)&(p->pextData[0]))) = 0;
		} break;
	case 4: //shift/sort/shuffle setup
		{
			p->pextSize = sizeof(paramExt) - sizeof(p->pextData) + sizeof(char)*2;
			((char *)&(p->pextData[0]))[0] = 0;
			((char *)&(p->pextData[0]))[1] = 0;
		} break;
	case 5: //search setup
		{
			p->pextSize = sizeof(paramExt) - sizeof(p->pextData) + sizeof(char)*2;
			((char *)&(p->pextData[0]))[0] = 0;
			((char *)&(p->pextData[0]))[1] = 0;
		} break;
	case 8: //load file setup
		{
			p->pextSize = sizeof(paramExt) - sizeof(p->pextData) + sizeof(char)*4;
			((char *)&(p->pextData[0]))[0] = 0;
			((char *)&(p->pextData[0]))[1] = 0;
			((char *)&(p->pextData[0]))[2] = 0;
			((char *)&(p->pextData[0]))[3] = 0;
		} break;
	}
#endif
}

#ifndef RUN_ONLY

struct ParamInfo
{
	paramExt &p;
	mv &mV;
};

BOOL CALLBACK ObjectSelector(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogSelector(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SSSSettings(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SearchSettings(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK LoadFileSettings(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);

#endif

/* EditParameter
 * This is where you actually bring up
 * the dialog and take input from
 * the user and store it.
 */
void MMF2Func EditParameter(mv *mV, short ID, paramExt *p) //TODO: handle upgrade for strings
{
#ifndef RUN_ONLY
	ParamInfo pi = {*p, *mV};
	switch(ID - PARAM_EXTBASE)
	{
	case 0: //object selector 1
	case 1: //object selector 2
	case 3: //object selector 3
	case 6: //object selector 4
	case 7: //object selector 5
		{
			DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_OBJECTSELECT), mV->mvHEditWin, ObjectSelector, LPARAM(&pi));
		} break;
	case 2: //dialog selector
		{
			DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_OPENDIALOG), mV->mvHEditWin, DialogSelector, LPARAM(&pi));
		} break;
	case 4: //shift/sort/shuffle setup
		{
			DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_SSS), mV->mvHEditWin, SSSSettings, LPARAM(&pi));
		} break;
	case 5: //search setup
		{
			DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_SEARCH), mV->mvHEditWin, SearchSettings, LPARAM(&pi));
		} break;
	case 8: //load file setup
		{
			DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_LOADMODE), mV->mvHEditWin, LoadFileSettings, LPARAM(&pi));
		} break;
	}
#endif
}

/* GetParameterString
 * This is where you give MMF2 the display
 * string for your custom parameter. Remember,
 * this is the display for ONE PARAMETER. It
 * will appear amongst all the other parameter
 * displays depending on the A/C's display
 * string.
 */
void MMF2Func GetParameterString(mv *mV, short ID, paramExt const *p, LPTSTR dest, short size) //TODO: cleanup
{
#ifndef RUN_ONLY
	static stdtstring const SettingsNames[] =
	{
		_T("No deleting groups"),
		_T("No deleting items"),
		_T("No renaming groups"),
		_T("No renaming items"),
		_T("No changing values"),
		_T("No refresh button"),
		_T("No new groups"),
		_T("No new items"),
		_T("No auto refresh"),
		_T("Pause application")
	};
	static stdtstring const SSSTag[] =
	{
		_T("Sort (by value, "),
		_T("Shuffle"),
		_T("Remove first element"),
		_T("Cycle Up"),
		_T("Remove last element"),
		_T("Cycle Down"),
		_T("Reverse"),
		_T("Swap items and values in"),
		_T("Renumber"),
		_T("Sort (by name, "),
		_T("Sort (by first part, ")
	};
	static stdtstring const SSSType[] =
	{
		_T("alphanum)"),
		_T("standard, case insensitive)"),
		_T("standrad, case sensitive)")
	};
	static stdtstring const SearchModeNames[] =
	{
		_T("Disregard previous results"),
		_T("Or"),
		_T("And"),
		_T("Xor")
	};
	static stdtstring const SearchSettingsNames[] =
	{
		_T("Unused"),
		_T("Only add group names to results"),
		_T("Match any of the conditions"),
		_T("Case sensitive")
	};
	static stdtstring const LoadFileStrings[] =
	{
		_T("Load file, "),
		_T("Change the current file path, "),
		_T("changing the current file path, and "),
		_T("keeping the old path, and "),
		_T("changing the current file path if the file exists, and "),
		_T("clearing the current data in the object."),
		_T("keeping the current data."),
		_T("clearing the current data in the object if the file exists."),
		_T(" (Immedately save)"),
		_T(" (Read Only)")
	};
	switch(ID - PARAM_EXTBASE)
	{
	case 4: //shift/sort/shuffle setup
		{
			std::size_t n0 = p->pextData[0];
			std::size_t n1 = p->pextData[1];
			if(n0 < sizeof(SSSTag)/sizeof(stdtstring) && n1 < 3 /*TODO*/)
			{
				stdtstring sss = SSSTag[n0];
				if(*sss.rbegin() == ' ')
				{
					sss += SSSType[n1];
				}
				_tcsncpy(dest, sss.c_str(), size/sizeof(TCHAR));
			}
			else
			{
				_tcsncpy(dest, _T("<corrupt parameter>"), size/sizeof(TCHAR));
			}
		} break;
	case 5: //search setup
		{
			std::size_t num = p->pextData[0];
			char const &flags = p->pextData[1];
			if(num < sizeof(SearchModeNames)/sizeof(stdtstring))
			{
				stdtstring str = _T("Mode: ");
				str += SearchModeNames[num];
				if(flags)
				{
					str += _T(" (Options: ");
					bool first = true;
					for(std::size_t n = 0; n < sizeof(SearchSettingsNames)/sizeof(stdtstring); ++n)
					{
						if(flags & (1 << n))
						{
							if(!first)
							{
								str += _T(", ");
							}
							else
							{
								first = false;
							}
							str += SearchSettingsNames[n];
						}
					}
					str += _T(")");
				}
				_tcsncpy(dest, str.c_str(), size/sizeof(TCHAR));
			}
			else
			{
				_tcsncpy(dest, _T("<corrupt parameter>"), size/sizeof(TCHAR));
			}
		} break;
	case 0: //object selector 1
	case 1: //object selector 2
	case 3: //object selector 3
	case 6: //object selector 4
	case 7: //object selector 5
		{
			_tcsncpy(dest, (TCHAR *)(p->pextData), size/sizeof(TCHAR));
		} break;
	case 2: //dialog selector
		{
			int &flags = *(int *)(p->pextData);
			stdtstring str;
			bool first = true;
			for(std::size_t n = 0; n < sizeof(SettingsNames)/sizeof(stdtstring); ++n)
			{
				if(flags & (1 << n))
				{
					if(!first)
					{
						str += _T(", ");
					}
					else
					{
						first = false;
					}
					str += SettingsNames[n];
				}
			}
			if(str.empty())
			{
				str = _T("None");
			}
			_tcsncpy(dest, str.c_str(), size/sizeof(TCHAR));
		} break;
	case 8: //load file setup
		{
			std::size_t num0 = 0 + p->pextData[0];
			std::size_t num1 = 2 + p->pextData[1];
			std::size_t num2 = 5 + p->pextData[2];
			std::size_t flags = p->pextData[3];
			std::size_t const max = sizeof(LoadFileStrings)/sizeof(stdtstring);
			if(num0 < max && num1 < max && num2 < max)
			{
				stdtstring str = LoadFileStrings[num0];
				if(!num0)
				{
					str += LoadFileStrings[num1];
				}
				str += LoadFileStrings[num2];
				if(flags & (1 << 0))
				{
					str += LoadFileStrings[8];
				}
				if(flags & (1 << 1))
				{
					str += LoadFileStrings[9];
				}
				_tcsncpy(dest, str.c_str(), size/sizeof(TCHAR));
			}
			else
			{
				_tcsncpy(dest, _T("<corrupt parameter>"), size/sizeof(TCHAR));
			}
		} break;
	}
#endif
}

#ifndef RUN_ONLY

BOOL CALLBACK ObjectSelector(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	switch(msgType)
	{
	case WM_INITDIALOG:
		{
			SetWindowLong(hDlg, DWL_USER, lParam);
			ParamInfo &pi = *(ParamInfo *)lParam;

			SetDlgItemText(hDlg, IDC_EDIT, (TCHAR *)pi.p.pextData);

			switch(pi.p.pextCode - PARAM_EXTBASE)
			{
			case 0: SetWindowText(GetDlgItem(hDlg, IDC_INFO), _T("Please type the name of an Ini++ object (other than this one) that you wish to use as the source for the merge operation. Please note this is case sensitive.")); break;
			case 1: SetWindowText(GetDlgItem(hDlg, IDC_INFO), _T("Please type the name of an Ini++ object (other than this one) that you wish to save search results to. Please note this is case sensitive.")); break;
			case 3: SetWindowText(GetDlgItem(hDlg, IDC_INFO), _T("Please type the name of a Chart Object that you wish to use. Please note this is case sensitive.")); break;
			case 6: SetWindowText(GetDlgItem(hDlg, IDC_INFO), _T("Please type the name of an Array Object that you wish to recieve data from. Please note this is case sensitive.")); break;
			case 7: SetWindowText(GetDlgItem(hDlg, IDC_INFO), _T("Please type the name of an Array Object that you wish to put data into. Please note this is case sensitive.")); break;
			}

			return TRUE;
		} break;
	case WM_COMMAND:
		{
			ParamInfo &pi = *(ParamInfo *)GetWindowLong(hDlg, DWL_USER);

			switch(wmCommandID)
			{
			case IDOK:
				{
					GetDlgItemText(hDlg, IDC_EDIT, (TCHAR *)pi.p.pextData, (PARAM_EXTMAXSIZE-sizeof(paramExt))/sizeof(TCHAR));
					pi.p.pextSize = sizeof(paramExt) - sizeof(pi.p.pextData) + (_tcslen((TCHAR *)pi.p.pextData)+1)*sizeof(TCHAR);
					EndDialog(hDlg, TRUE);
				} break;
			case IDCANCEL:
				{
					EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
			}
		} break;
	}
	return FALSE;
}
BOOL CALLBACK DialogSelector(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	switch(msgType)
	{
	case WM_INITDIALOG:
		{
			SetWindowLong(hDlg, DWL_USER, lParam);
			ParamInfo &pi = *(ParamInfo *)lParam;
			int &flags = *(int *)pi.p.pextData;

			for(int n = IDC_CHECK1; n <= IDC_CHECK10; ++n)
			{
				SendDlgItemMessage(hDlg, n, BM_SETCHECK, (flags & (1 << (n - IDC_CHECK1)))? BST_CHECKED : BST_UNCHECKED, 0);
			}

			SendDlgItemMessage(hDlg, IDC_COMBO, CB_ADDSTRING, 0, (LPARAM)_T("item = 'value'"));
			SendDlgItemMessage(hDlg, IDC_COMBO, CB_ADDSTRING, 0, (LPARAM)_T("item"));
			SendDlgItemMessage(hDlg, IDC_COMBO, CB_ADDSTRING, 0, (LPARAM)_T("item ('value')"));
			SendDlgItemMessage(hDlg, IDC_COMBO, CB_ADDSTRING, 0, (LPARAM)_T("'value'"));
			SendDlgItemMessage(hDlg, IDC_COMBO, CB_ADDSTRING, 0, (LPARAM)_T("Item 'item' has value 'value'"));
			SendDlgItemMessage(hDlg, IDC_COMBO, CB_ADDSTRING, 0, (LPARAM)_T("Item: 'item' Value: 'value'"));
			SendDlgItemMessage(hDlg, IDC_COMBO, CB_ADDSTRING, 0, (LPARAM)_T("item: value"));
			SendDlgItemMessage(hDlg, IDC_COMBO, CB_ADDSTRING, 0, (LPARAM)_T("Item 'item' Valeur 'value'"));

			SendDlgItemMessage(hDlg, IDC_COMBO, CB_SETCURSEL, HIWORD(flags), 0);
			SendMessage(hDlg, WM_COMMAND, IDC_CHECK10, 0);

			return TRUE;
		} break;
	case WM_COMMAND:
		{
			ParamInfo &pi = *(ParamInfo *)GetWindowLong(hDlg, DWL_USER);
			int &flags = *(int *)pi.p.pextData;
			switch(wmCommandID)
			{
			case IDOK:
				{ 
					flags = 0;
					for(int n = IDC_CHECK1; n <= IDC_CHECK10; ++n)
					{
						flags |= (1 << (n - IDC_CHECK1))*((SendDlgItemMessage(hDlg, n, BM_GETCHECK, 0, 0) == BST_CHECKED)? 1 : 0);
					}
					flags = MAKELONG(LOWORD(flags), SendDlgItemMessage(hDlg, IDC_COMBO, CB_GETCURSEL, 0, 0));

					EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
			case IDCANCEL:
				{
					EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
			case IDC_CHECK10:
				{
					if(BST_CHECKED == SendDlgItemMessage(hDlg, IDC_CHECK10, BM_GETCHECK, 0, 0))
					{
						SendDlgItemMessage(hDlg, IDC_CHECK6, BM_SETCHECK, BST_CHECKED, 0);
						SendDlgItemMessage(hDlg, IDC_CHECK9, BM_SETCHECK, BST_CHECKED, 0);
						EnableWindow(GetDlgItem(hDlg, IDC_CHECK6), FALSE);
						EnableWindow(GetDlgItem(hDlg, IDC_CHECK9), FALSE);
					}
					else
					{
						EnableWindow(GetDlgItem(hDlg, IDC_CHECK6), TRUE);
						EnableWindow(GetDlgItem(hDlg, IDC_CHECK9), TRUE);
					}
				} break;
			}
		} break;
	}
	return FALSE;
}
BOOL CALLBACK SSSSettings(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	static stdtstring const descriptions[] =
	{
		_T("Rearranges the items so that they are in alphabetical order based on the items value"),
		_T("Randomly rearranges the items in the group"),
		_T("Removes the first item in the group"),
		_T("Cycles the elements down, so that the current last element is made the first element"),
		_T("Removes the last element in the group"),
		_T("Cycles the elements up, so the current first element is made the last element"),
		_T("Reverses the order of the group"),
		_T("Swap every item so that its new name is its old value, and its new value is its old name"),
		_T("Names the groups by their ordering, so the first item is called '1', the second '2', etc."),
		_T("Rearranges the items so that they are in alphabetical order based on the items name"),
		_T("Rearrages the items so that they are in alphabetical order based on the value before the first comma.\r\n\r\nFor instance, 'example,value' would be sorted as if its value is just 'example'")
	};
	std::size_t const descsize = sizeof(descriptions)/sizeof(stdtstring);
	switch(msgType)
	{
	case WM_INITDIALOG:
		{
			SetWindowLong(hDlg, DWL_USER, lParam);
			ParamInfo &pi = *(ParamInfo *)lParam;
			char &n0 = pi.p.pextData[0];
			char &n1 = pi.p.pextData[1];

			if(n0 < descsize)
			{
				SendDlgItemMessage(hDlg, IDC_DESCRIPTIONTEXT, WM_SETTEXT, 0, LPARAM(descriptions[n0].c_str()));
				SendDlgItemMessage(hDlg, IDC_RADIO+n0, BM_SETCHECK, BST_CHECKED, 0);
			}
			else
			{
				SendDlgItemMessage(hDlg, IDC_DESCRIPTIONTEXT, WM_SETTEXT, 0, LPARAM(_T("<invalid selection>")));
			}
			if(n1 < 3) //TODO: eliminate magic number
			{
				SendDlgItemMessage(hDlg, IDC_SORTINGRADIO1+n1, BM_SETCHECK, BST_CHECKED, 0);
			}
			switch(IDC_RADIO+n0)
			{
			case IDC_RADIO:
			case IDC_RADIO10:
			case IDC_RADIO11:
				{
					EnableWindow(GetDlgItem(hDlg, IDC_SORTINGRADIO1), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDC_SORTINGRADIO2), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDC_SORTINGRADIO3), TRUE);
				} break;
			default:
				{
					EnableWindow(GetDlgItem(hDlg, IDC_SORTINGRADIO1), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDC_SORTINGRADIO2), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDC_SORTINGRADIO3), FALSE);
				} break;
			}

			return TRUE;
		} break;
	case WM_COMMAND:
		{
			ParamInfo &pi = *(ParamInfo *)GetWindowLong(hDlg, DWL_USER);

			switch(wmCommandID)
			{
			case IDOK:
				{
					char &n0 = pi.p.pextData[0];
					char &n1 = pi.p.pextData[1];
					for(n0 = 0; n0 < descsize; ++n0)
					{
						if(BST_CHECKED == SendDlgItemMessage(hDlg, IDC_RADIO+n0, BM_GETCHECK, 0, 0))
						{
							break;
						}
					}
					for(n1 = 0; n1 < 3 /*TODO*/; ++n1)
					{
						if(BST_CHECKED == SendDlgItemMessage(hDlg, IDC_SORTINGRADIO1+n1, BM_GETCHECK, 0, 0))
						{
							break;
						}
					}

					EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
			case IDCANCEL:
				{
					EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
			default:
				{
					for(int n = 0; n < descsize; ++n)
					{
						if(BST_CHECKED == SendDlgItemMessage(hDlg, IDC_RADIO+n, BM_GETCHECK, 0, 0))
						{
							SendDlgItemMessage(hDlg, IDC_DESCRIPTIONTEXT, WM_SETTEXT, 0, LPARAM(descriptions[n].c_str()));
							switch(IDC_RADIO+n)
							{
							case IDC_RADIO:
							case IDC_RADIO10:
							case IDC_RADIO11:
								{
									EnableWindow(GetDlgItem(hDlg, IDC_SORTINGRADIO1), TRUE);
									EnableWindow(GetDlgItem(hDlg, IDC_SORTINGRADIO2), TRUE);
									EnableWindow(GetDlgItem(hDlg, IDC_SORTINGRADIO3), TRUE);
								} break;
							default:
								{
									EnableWindow(GetDlgItem(hDlg, IDC_SORTINGRADIO1), FALSE);
									EnableWindow(GetDlgItem(hDlg, IDC_SORTINGRADIO2), FALSE);
									EnableWindow(GetDlgItem(hDlg, IDC_SORTINGRADIO3), FALSE);
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
BOOL CALLBACK SearchSettings(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	switch(msgType)
	{
	case WM_INITDIALOG:
		{
			SetWindowLong(hDlg, DWL_USER, lParam);
			ParamInfo &pi = *(ParamInfo *)lParam;
			char &n0 = pi.p.pextData[0];
			char &n1 = pi.p.pextData[1];

			SendDlgItemMessage(hDlg, IDC_RADIO+n0, BM_SETCHECK, BST_CHECKED, 0);
			for(int n = 0; n < 3 /*TODO*/; ++n)
			{
				if(n1 & (1 << (n+1)))
				{
					SendDlgItemMessage(hDlg, IDC_CHECK2+n, BM_SETCHECK, BST_CHECKED, 0);
				}
			}

			return TRUE;
		} break;
	case WM_COMMAND:
		{
			ParamInfo &pi = *(ParamInfo *)GetWindowLong(hDlg, DWL_USER);
			char &n0 = pi.p.pextData[0];
			char &n1 = pi.p.pextData[1];
			switch(wmCommandID)
			{
			case IDOK:
				{
					for(n0 = 0; n0 < 4 /*TODO*/; ++n0)
					{
						if(BST_CHECKED == SendDlgItemMessage(hDlg, IDC_RADIO+n0, BM_GETCHECK, 0, 0))
						{
							break;
						}
					}
					n1 = 0;
					for(int n = 0; n < 3 /*TODO*/; ++n)
					{
						if(BST_CHECKED == SendDlgItemMessage(hDlg, IDC_CHECK2+n, BM_GETCHECK, 0, 0))
						{
							n1 |= (1 << (n+1));
						}
					}

					EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
			case IDCANCEL:
				{
					EndDialog(hDlg, TRUE);
					return TRUE;
				} break;
			}
		}
	}
	return FALSE;
}
BOOL CALLBACK LoadFileSettings(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	switch(msgType)
	{
	case WM_INITDIALOG:
		{
			SetWindowLong(hDlg, DWL_USER, lParam);
			ParamInfo &pi = *(ParamInfo *)lParam;

			//

			return TRUE;
		} break;
	}
	return FALSE;
}

#endif
