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
void MMF2Func EditParameter(mv *mV, short ID, paramExt *p)
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

#ifndef RUN_ONLY

stdtstring SettingsNames[] =
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
stdtstring SSSTag[] =
{
	_T("Sort (by value)"),
	_T("Shuffle"),
	_T("Remove first element"),
	_T("Cycle Up"),
	_T("Remove last element"),
	_T("Cycle Down"),
	_T("Reverse"),
	_T("Swap items and values in"),
	_T("Renumber"),
	_T("Sort (by name)"),
	_T("Sort (by first part)")
};
stdtstring SearchModeNames[] =
{
	_T("Disregard previous results"),
	_T("Or"),
	_T("And"),
	_T("Xor")
};
stdtstring SearchSettingsNames[] =
{
	_T("Unused"),
	_T("Only add group names to results"),
	_T("Match any of the conditions"),
	_T("Case sensitive")
};
stdtstring LoadFileStrings[] =
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

#endif

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
	switch(ID - PARAM_EXTBASE)
	{
	case 4: //shift/sort/shuffle setup
		{
			std::size_t num = p->pextData[0];
			if(num < sizeof(SSSTag)/sizeof(stdtstring))
			{
				_tcsncpy(dest, SSSTag[num].c_str(), size/sizeof(TCHAR));
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
	case 8:
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
					GetDlgItemText(hDlg, IDC_EDIT, (TCHAR *)pi.p.pextData, PARAM_EXTMAXSIZE);
					pi.p.pextSize = sizeof(paramExt) - sizeof(pi.p.pextData) + (_tcslen((TCHAR *)pi.p.pextData)+1)*sizeof(TCHAR);
					EndDialog(hDlg, TRUE);
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

			//

			return TRUE;
		} break;
	}
	return FALSE;
}
BOOL CALLBACK SSSSettings(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
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
BOOL CALLBACK SearchSettings(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
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
