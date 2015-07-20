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
#include <sstream>
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
			strcpy(&(p->pextData[0]), "Ini++ v1.6");
			p->pextSize = sizeof(paramExt) - sizeof(p->pextData) + strlen(p->pextData)+1;
		} break;
	case 3: //object selector 3
		{
			strcpy(&(p->pextData[0]), "Chart");
			p->pextSize = sizeof(paramExt) - sizeof(p->pextData) + strlen(p->pextData)+1;
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

BOOL CALLBACK DLLExport ObjectSelector(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DLLExport DialogSelector(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DLLExport SSSSettings(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DLLExport SearchSettings(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DLLExport LoadFileSettings(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);

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
void MMF2Func GetParameterString(mv *mV, short ID, paramExt *p, LPSTR dest, short size)
{
#ifndef RUN_ONLY
	// Example
	// -------
	// wsprintf(pDest, "Super parameter %s", p->pextData);
#endif
}
