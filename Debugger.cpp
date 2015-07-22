/* Debugger.cpp
 * Here you will be able to interact
 * with the MMF2 debugger (that thing
 * in the top left when you run your
 * MFA from MMF2). You can add your own
 * fields of your choice to your object's
 * debugger tree.
 * Functions defined here:
 * GetDebugTree
 * GetDebugItem
 * EditDebugItem
 */

#include "Common.h"

#ifndef RUN_ONLY //The debugger is only available when running from MMF2, which uses the Edittime MFX

/* DB
 * This is the namespace the debugger identifiers
 * are stored in. It keeps them out of the global
 * namespace and it helps with intellisense.
 */
namespace DB
{
	/* <enum>
	 * The debugger identitifers.
	 */
	enum
	{
		Filename,
		Tree,
		CurrentData,
		GlobalSlots,
	};
}

/* DebugTree
 * The list of items to be displayed
 * in the debugger tree.
 */
WORD DebugTree[] =
{
	DB::Filename,
	DB::Tree|DB_EDITABLE,
	DB::CurrentData|DB_EDITABLE,
	DB::GlobalSlots|DB_EDITABLE,
	DB_END
};

#endif

/* GetDerbugTree
 * Plain and simple, just return the debug
 * tree pointer from above. If you really
 * want to you can create the debug tree
 * dynamically and return that, but you'll
 * need to store the pointer in your extension
 * class every time so that you can release
 * the memory in your extension class' destructor.
 */
LPWORD MMF2Func GetDebugTree(RD *rd)
{
#ifndef RUN_ONLY
	return DebugTree;
#endif
	return NULL;
}

/* GetDebugItem
 * With respect to DB_BUFFERSIZE, give
 * MMF2 the string to display in the
 * debugger. You must include both the
 * name and the value in the string,
 * or any format you want.
 */
void MMF2Func GetDebugItem(LPTSTR Buffer, RD *rd, int ID)
{
#ifndef RUN_ONLY
	std::basic_ostringstream<TCHAR> oss;
	switch (ID)
	{
	case DB::Filename:
		{
			//TODO
		} break;
	case DB::CurrentData:
		{
			oss << _T("(Double click to view/edit data as text)");
		} break;
	case DB::Tree:
		{
			oss << _T("(Double click to view data as tree)");
		} break;
	case DB::GlobalSlots:
		{
			//TODO
		} break;
	}
	_tcsncpy(Buffer, oss.str().c_str(), DB_BUFFERSIZE/sizeof(TCHAR));
#endif
}

#ifndef RUN_ONLY
BOOL CALLBACK CurrentDataDebug(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK TreeDebug(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GlobalSlotsDebug(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);
#endif

/* EditDebugItem
 * When the user chooses to edit an
 * editable debug item, this function
 * is called. You can use the RFUNCTIONs
 * provided by MMF2 to do simple text and
 * number edits, or you can do whatever you
 * want, such as creating a dialog that lets
 * the user enter more sophisticated input to
 * the debug item, or simply display more info.
 */
void MMF2Func EditDebugItem(RD *rd, int ID)
{
#ifndef RUN_ONLY
	switch(ID)
	{
	case DB::CurrentData:
		{
			DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_DEBUG), NULL, CurrentDataDebug, LPARAM(rd->pExtension));
		} break;
	case DB::Tree:
		{
			//TODO
			//DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_DEBUG), NULL, TreeDebug, LPARAM(rd->pExtension));
		} break;
	case DB::GlobalSlots:
		{
			DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_DEBUG), NULL, GlobalSlotsDebug, LPARAM(rd->pExtension));
		} break;
	}
#endif
}

BOOL CALLBACK CurrentDataDebug(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	//TODO
	return FALSE;
}

BOOL CALLBACK GlobalSlotsDebug(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	//TODO
	return FALSE;
}
