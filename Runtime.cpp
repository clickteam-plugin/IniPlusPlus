// ============================================================================
//
// This file contains routines that are handled during the Runtime.
//
// Including creating, display, and handling your object.
// 
// ============================================================================

// Common Include
#include	"common.h"
#include "common_dialog.h"
// DEBUGGER /////////////////////////////////////////////////////////////////

#if !defined(RUN_ONLY)
// Identifiers of items displayed in the debugger
enum
{
DB_FILENAME,
DB_TREE,
DB_CURRENTDATA,
DB_GLOBALSLOTS
};

// Items displayed in the debugger
WORD DebugTree[]=
{
// Example
// -------
	DB_FILENAME,
	DB_TREE|DB_EDITABLE,
	DB_CURRENTDATA|DB_EDITABLE,
	DB_GLOBALSLOTS|DB_EDITABLE,
	// End of table (required)
	DB_END
};

#endif // !defined(RUN_ONLY)


// --------------------
// GetRunObjectDataSize
// --------------------
// Returns the size of the runtime datazone of the object
// 
ushort WINAPI DLLExport GetRunObjectDataSize(fprh rhPtr, LPEDATA edPtr)
{
	return(sizeof(RUNDATA));
}


// ---------------
// CreateRunObject
// ---------------
// The routine where the object is actually created
// 
#define CSIDL_COMMON_APPDATA 0x0023

void __stdcall EventsProc( void* rdPtr , char mode )
{
	((LPRDATA)rdPtr)->duringCallback = 1;
	//mode | 1 : modified ; mode | 2 : saved ; mode | 4 : new item
	if ( mode & 1 )
	{
		callRunTimeFunction( ((LPRDATA)rdPtr), RFUNCTION_GENERATEEVENT, INI_MODIFIED , 0);
		#ifdef INCLUDE_DIALOG
		if ( ((LPRDATA)rdPtr)->dialogCurrent != NULL 
			&& !   ((((LPRDATA)rdPtr)->dialogSettings & NO_AUTO_REFRESH))     )
			SendMessage( ((LPRDATA)rdPtr)->dialogCurrent , WM_COMMAND , 14 , 0 );
		#endif
	}
	if ( mode & 2 )
		callRunTimeFunction( ((LPRDATA)rdPtr), RFUNCTION_GENERATEEVENT, INI_SAVED , 0);
	if ( mode & 4 )
	{
		//New item.
		callRunTimeFunction( ((LPRDATA)rdPtr), RFUNCTION_GENERATEEVENT, INI_NEWITEM , 0);
	}
	((LPRDATA)rdPtr)->duringCallback = 0;
	return;
}


//Get the overall parent app, even if we are in a load of sub-applications.
CRunApp* GetCRunApp(CRunApp* pApp)
{
	while (pApp->m_pParentApp != NULL)
		pApp = pApp->m_pParentApp;

	return pApp;
}

short WINAPI DLLExport CreateRunObject(LPRDATA rdPtr, LPEDATA edPtr, fpcob cobPtr)
{
/*
   This routine runs when your object is created, as you might have guessed.
   It is here that you must transfer any data you need in rdPtr from edPtr,
   because after this has finished you cannot access it again!
   Also, if you have anything to initialise (e.g. dynamic arrays, surface objects)
   you should do it here, and free your resources in DestroyRunObject.
*/
	// No errors
	rdPtr->ini = NULL;
	rdPtr->index = edPtr->index;
	rdPtr->autoLoad = edPtr->autoLoad;
	rdPtr->global = 0;
	if (! edPtr->globalObject )
		rdPtr->ini = new SuperINI();
	else
	{
	//	strcpy(rdPtr->globalKey,edPtr->globalKey);
		rdPtr->global = new SuperINI_SettingsStruct();
		LPRH rhPtr;
		rhPtr = rdPtr->rHo.hoAdRunHeader;
		CRunApp* pApp = rhPtr->rhApp;
		
		//Get the parent app.
		pApp = GetCRunApp(pApp);
		mv* mV = rhPtr->rh4.rh4Mv;
		
		//GLOBAL
		GlobalObjectManager* globalData = (GlobalObjectManager*)mV->mvGetExtUserData(pApp, hInstLib);
		rdPtr->ini = globalData->get(edPtr->globalKey);
	}
	rdPtr->results = new SearchResults();
	rdPtr->currentGroup = "";

	rdPtr->bool_CanCreateFolders = edPtr->bool_CanCreateFolders;
	rdPtr->ini->cb = 0;
	rdPtr->ini->undoLimit = edPtr->undoCount;
	rdPtr->ini->redoLimit = edPtr->redoCount;

	//Custom mode
	if ( !edPtr->bool_stdINI )
	{
		if ( edPtr->bool_newline )
			rdPtr->ini->SetNewLineCharacter( edPtr->newline );
		else
			rdPtr->ini->SetStandardNewLines();

		rdPtr->ini->SetNeverQuoteStrings( edPtr->bool_QuoteStrings );
		rdPtr->ini->SetRepeatModes( edPtr->repeatGroups , edPtr->repeatItems , edPtr->saveRepeated );
		rdPtr->ini->EscapeCharacters( edPtr->bool_EscapeGroup , edPtr->bool_EscapeItem , edPtr->bool_EscapeValue );
		rdPtr->ini->SetCaseSensitivity( edPtr->bool_CaseSensitive );
		rdPtr->ini->SetSubGroups(edPtr->subGroups,edPtr->allowEmptyGroups);
	//	subGroups = edPtr->subGroups;
	//	allowEmptyGroup = edPtr->allowEmptyGroups;
	}
	else //Default settings
		rdPtr->ini->ResetSettings();

	rdPtr->ini->SetAutosaveProperties( edPtr->bool_compress , edPtr->bool_encrypt , edPtr->encrypt_key );
	rdPtr->ini->SetAutoSave( edPtr->bool_AutoSave );

	char arr[MAX_PATH];
	switch ( edPtr->defaultFolder )
	{
	case 0:		//Windows
		GetWindowsDirectory( arr , MAX_PATH );
		break;
	case 1:		//Current
		GetCurrentDirectory( MAX_PATH , arr );
		break;
	case 2:		//Application
		{
		LPRH rhPtr;
		rhPtr = rdPtr->rHo.hoAdRunHeader;
		
		//Find out if it is ran from MMF or not
		{
			fpRunApp pApp = rhPtr->rhApp;
			while( pApp->m_pParentApp != NULL )
				pApp = pApp->m_pParentApp;
			if (  ( pApp->m_hdr.gaFlags & GA_ONEFILE ) )
			{
				//Stand alone
				CRunApp* a = rhPtr->rhApp;
				lstrcpyn( arr , a->m_appFileName , MAX_PATH );
			}
			else
			{
				//Ran from MMF
				if( pApp->m_editorFileName == 0 )
					lstrcpyn( arr , ((CRunApp*)rhPtr->rhApp)->m_appFileName , MAX_PATH );
				else
					lstrcpyn( arr , pApp->m_editorFileName , MAX_PATH );

			}

		}
		PathRemoveFileSpec( arr );
		}
		break;
	case 3:		//User data (CURRENT USER)
		GetWindowsDirectory( arr , MAX_PATH );
		SHGetSpecialFolderPath( NULL , arr , CSIDL_APPDATA , 0 );
		break;
	case 4:		//User data (ALL USERS)
		GetWindowsDirectory( arr , MAX_PATH );
		SHGetSpecialFolderPath( NULL , arr , CSIDL_COMMON_APPDATA , 0 );
		break;
	}
	rdPtr->defaultDirectory = arr;
	
	//Load the inital data
	if ( edPtr->defaultText[0] != 0 ) {
		rdPtr->ini->newUndoBlock();
		rdPtr->ini->LoadStream( edPtr->defaultText , false , false );
	}


	//Load file
	if ( edPtr->b_defaultFile && edPtr->defaultFile[0] != 0 )
	{
		rdPtr->ini->newUndoBlock();
		rdPtr->ini->LoadInAddition( FormatPath(edPtr->defaultFile) );
		if ( edPtr->b_ReadOnly )
			rdPtr->ini->SetReadOnly( true );
		rdPtr->ini->RequestAutoSave();
	}

	rdPtr->dialogCurrent = 0; //It's fine for now.
	rdPtr->ini->rdPtr.push_back( rdPtr );
	rdPtr->ini->rdPtrFavorite = rdPtr;
	rdPtr->ini->cb = &EventsProc;
	rdPtr->duringCallback = 0;

	if ( edPtr->globalObject )
		rdPtr->ini->ExportSettings( rdPtr->global );

	return 0;
}


// ----------------
// DestroyRunObject
// ----------------
// Destroys the run-time object
// 
short WINAPI DLLExport DestroyRunObject(LPRDATA rdPtr, long fast)
{
/*
   When your object is destroyed (either with a Destroy action or at the end of
   the frame) this routine is called. You must free any resources you have allocated!
*/
	// No errors
	if ( rdPtr->ini != NULL ) //Actually, it will never be null these days.
	{
		
		if ( rdPtr->dialogChild != 0 )
			EndDialog( rdPtr->dialogChild , IDCANCEL );

		if ( rdPtr->dialogCurrent != 0 )
			EndDialog( rdPtr->dialogCurrent , IDCANCEL );

		rdPtr->ini->cb = 0;

		
		if (! rdPtr->global )
			delete rdPtr->ini;
		else
		{
			rdPtr->ini->rdPtr.remove(rdPtr);
			delete rdPtr->global;
		}
		
		delete rdPtr->results;
	}
	return 0;
}


// ----------------
// HandleRunObject
// ----------------
// Called (if you want) each loop, this routine makes the object live
// 
short WINAPI DLLExport HandleRunObject(LPRDATA rdPtr)
{
/*
   If your extension will draw to the MMF window you should first 
   check if anything about its display has changed :

       if (rdPtr->roc.rcChanged)
          return REFLAG_DISPLAY;
       else
          return 0;

   You will also need to make sure you change this flag yourself 
   to 1 whenever you want to redraw your object
 
   If your extension won't draw to the window, but it still needs 
   to do something every MMF loop use :

        return 0;

   If you don't need to do something every loop, use :

        return REFLAG_ONESHOT;

   This doesn't mean this function can never run again. If you want MMF
   to handle your object again (causing this code to run) use this function:

        callRun-timeFunction(rdPtr, RFUNCTION_REHANDLE, 0, 0);

   At the end of the loop this code will run
*/
	// Will not be called next loop	
	return REFLAG_ONESHOT;
}

// ----------------
// DisplayRunObject
// ----------------
// Draw the object in the application screen.
// 
short WINAPI DLLExport DisplayRunObject(LPRDATA rdPtr)
{
/*
   If you return REFLAG_DISPLAY in HandleRunObject this routine will run.
*/
	// Ok
	return 0;
}



// ----------------
// PauseRunObject
// ----------------
// Enters the pause mode
// 
short WINAPI DLLExport PauseRunObject(LPRDATA rdPtr)
{
	// Ok
	return 0;
}


// -----------------
// ContinueRunObject
// -----------------
// Quits the pause mode
//
short WINAPI DLLExport ContinueRunObject(LPRDATA rdPtr)
{
	// Ok
	return 0;
}


// ============================================================================
//
// START APP / END APP / START FRAME / END FRAME routines
// 
// ============================================================================

// -------------------
// StartApp
// -------------------
// Called when the application starts or restarts.
// Useful for storing global data
// 
void WINAPI DLLExport StartApp(mv _far *mV, CRunApp* pApp)
{
	
	// Delete global data (if restarts application)
	if ( pApp == GetCRunApp(pApp) )
	{
		//GLOBAL
		GlobalObjectManager* globalData = (GlobalObjectManager*)mV->mvGetExtUserData(pApp, hInstLib);
		if ( globalData != NULL )
		{
			delete globalData;
		}
		mV->mvSetExtUserData(pApp, hInstLib, new GlobalObjectManager());
	}
}

// -------------------
// EndApp
// -------------------
// Called when the application ends.
// 
void WINAPI DLLExport EndApp(mv _far *mV, CRunApp* pApp)
{
	// Delete global data
	if ( pApp == GetCRunApp(pApp) )
	{
		GlobalObjectManager* globalData = (GlobalObjectManager*)mV->mvGetExtUserData(pApp, hInstLib);
		if ( globalData != NULL )
		{
			delete globalData;
			mV->mvSetExtUserData(pApp, hInstLib, NULL);
		}
	}
}

// -------------------
// StartFrame
// -------------------
// Called when the frame starts or restarts.
// 
void WINAPI DLLExport StartFrame(mv _far *mV, DWORD dwReserved, int nFrameIndex)
{

}

// -------------------
// EndFrame
// -------------------
// Called when the frame ends.
// 
void WINAPI DLLExport EndFrame(mv _far *mV, DWORD dwReserved, int nFrameIndex)
{

}

// ============================================================================
//
// TEXT ROUTINES (if OEFLAG_TEXT)
// 
// ============================================================================

// -------------------
// GetRunObjectFont
// -------------------
// Return the font used by the object.
// 
/*

  // Note: do not forget to enable the functions in the .def file 
  // if you remove the comments below.

void WINAPI GetRunObjectFont(LPRDATA rdPtr, LOGFONT* pLf)
{
	// Example
	// -------
	// GetObject(rdPtr->m_hFont, sizeof(LOGFONT), pLf);
}

// -------------------
// SetRunObjectFont
// -------------------
// Change the font used by the object.
// 
void WINAPI SetRunObjectFont(LPRDATA rdPtr, LOGFONT* pLf, RECT* pRc)
{
	// Example
	// -------
//	HFONT hFont = CreateFontIndirect(pLf);
//	if ( hFont != NULL )
//	{
//		if (rdPtr->m_hFont!=0)
//			DeleteObject(rdPtr->m_hFont);
//		rdPtr->m_hFont = hFont;
//		SendMessage(rdPtr->m_hWnd, WM_SETFONT, (WPARAM)rdPtr->m_hFont, FALSE);
//	}

}

// ---------------------
// GetRunObjectTextColor
// ---------------------
// Return the text color of the object.
// 
COLORREF WINAPI GetRunObjectTextColor(LPRDATA rdPtr)
{
	// Example
	// -------
	return 0;	// rdPtr->m_dwColor;
}

// ---------------------
// SetRunObjectTextColor
// ---------------------
// Change the text color of the object.
// 
void WINAPI SetRunObjectTextColor(LPRDATA rdPtr, COLORREF rgb)
{
	// Example
	// -------
	rdPtr->m_dwColor = rgb;
	InvalidateRect(rdPtr->m_hWnd, NULL, TRUE);
}
*/


// ============================================================================
//
// WINDOWPROC (interception of messages sent to hMainWin and hEditWin)
//
// Do not forget to enable the WindowProc function in the .def file if you implement it
// 
// ============================================================================
/*
// Get the pointer to the object's data from its window handle
// Note: the object's window must have been subclassed with a
// callRunTimeFunction(rdPtr, RFUNCTION_SUBCLASSWINDOW, 0, 0);
// See the documentation and the Simple Control example for more info.
//
LPRDATA GetRdPtr(HWND hwnd, LPRH rhPtr)
{
	return (LPRDATA)GetProp(hwnd, (LPCSTR)rhPtr->rh4.rh4AtomRd);
}

// Called from the window proc of hMainWin and hEditWin.
// You can intercept the messages and/or tell the main proc to ignore them.
//
LRESULT CALLBACK DLLExport WindowProc(LPRH rhPtr, HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	LPRDATA rdPtr = NULL;

	switch (nMsg) {

	// Example
	case WM_CTLCOLORSTATIC:
		{
			// Get the handle of the control
			HWND hWndControl = (HWND)lParam;

			// Get a pointer to the RUNDATA structure (see GetRdptr function above for more info)
			rdPtr = GetRdPtr(hWndControl, rhPtr);

			// Check if the rdPtr pointer is valid and points to an object created with this extension
			if ( rdPtr == NULL || rdPtr->rHo.hoIdentifier != IDENTIFIER )
				break;

			// OK, intercept the message
			HDC hDC = (HDC)wParam;
			SetBkColor(hDC, rdPtr->backColor);
			SetTextColor(hDC, rdPtr->fontColor);
			rhPtr->rh4.rh4KpxReturn = (long)rdPtr->hBackBrush;
			return REFLAG_MSGRETURNVALUE;
		}
		break;
	}

	return 0;
}
*/



// ============================================================================
//
// DEBUGGER ROUTINES
// 
// ============================================================================

// -----------------
// GetDebugTree
// -----------------
// This routine returns the address of the debugger tree
//
LPWORD WINAPI DLLExport GetDebugTree(LPRDATA rdPtr)
{
#if !defined(RUN_ONLY)
	return DebugTree;
#else
	return NULL;
#endif // !defined(RUN_ONLY)
}

// -----------------
// GetDebugItem
// -----------------
// This routine returns the text of a given item.
//
void WINAPI DLLExport GetDebugItem(LPSTR pBuffer, LPRDATA rdPtr, int id)
{
#if !defined(RUN_ONLY)

	switch (id)
	{
	case DB_FILENAME:
		if ( rdPtr->ini->GetFileName() != "" )
			lstrcpyn( pBuffer , ("File: " + rdPtr->ini->GetFileName()).c_str() , DB_BUFFERSIZE );
		else
			lstrcpyn( pBuffer , "(No File Loaded)" , DB_BUFFERSIZE );
		break;
	case DB_CURRENTDATA:
		lstrcpyn( pBuffer , "(Double click to view and edit data as text)" , DB_BUFFERSIZE );
		break;
	case DB_TREE:
		lstrcpyn( pBuffer , "(Double click to view data as tree)" , DB_BUFFERSIZE );
		break;
	case DB_GLOBALSLOTS:
		{
			LPRH rhPtr;
			rhPtr = rdPtr->rHo.hoAdRunHeader;
			CRunApp* pApp = rhPtr->rhApp;
			//Get the parent app.
			while (pApp->m_pParentApp != NULL)
				pApp = pApp->m_pParentApp;
			mv* mV = rhPtr->rh4.rh4Mv;
			GlobalObjectManager* globalData = (GlobalObjectManager*)mV->mvGetExtUserData(pApp, hInstLib);
			lstrcpyn( pBuffer , (rdPtr->ini->toString(globalData->size()) + " global slots (Double click to view)").c_str() , DB_BUFFERSIZE );
		}
	}


#endif // !defined(RUN_ONLY)
}

// -----------------
// EditDebugItem
// -----------------
// This routine allows to edit editable items.
//



#ifndef RUN_ONLY

BOOL CALLBACK DLLExport debugboxProc3(HWND hDlg,uint msgType,WPARAM wParam,LPARAM lParam)
{
	LPRDATA	rdPtr;
	switch (msgType)
	{
	case WM_INITDIALOG: // Init dialog
		{
		rdPtr = (LPRDATA)lParam;
		SetWindowLong(hDlg, DWL_USER, lParam);
		HWND list = GetDlgItem( hDlg , IDC_LIST1 );
		
		LPRH rhPtr;
		rhPtr = rdPtr->rHo.hoAdRunHeader;
		CRunApp* pApp = rhPtr->rhApp;
		
		//Get the parent app.
		while (pApp->m_pParentApp != NULL)
			pApp = pApp->m_pParentApp;

		mv* mV = rhPtr->rh4.rh4Mv;
		
		GlobalObjectManager* globalData = (GlobalObjectManager*)mV->mvGetExtUserData(pApp, hInstLib);
		for ( int n = 0 ; n < globalData->size() ; n++ )
			SendMessage( list ,  LB_ADDSTRING , 0 , (long)globalData->slot(n).c_str() );

		}
		return TRUE;
	case WM_COMMAND: // Command
		rdPtr = (LPRDATA)GetWindowLong(hDlg, DWL_USER);
		switch (wmCommandID)
		{
		case IDOK:
		case IDCANCEL:
			// User pressed cancel, don't save anything
			// Close the dialog
			EndDialog(hDlg, IDCANCEL);
			return 0;

		default:
			break;
		}

	break;

	default:
		break;
	}
	return FALSE;
}





BOOL CALLBACK DLLExport debugboxProc(HWND hDlg,uint msgType,WPARAM wParam,LPARAM lParam)
{
	LPRDATA	rdPtr;
	switch (msgType)
	{
	case WM_INITDIALOG: // Init dialog
		rdPtr = (LPRDATA)lParam;
		SetWindowLong(hDlg, DWL_USER, lParam);
		SetDlgItemText(hDlg, IDC_DATA, rdPtr->ini->WriteStream().c_str());
		return TRUE;

	case WM_COMMAND: // Command
		rdPtr = (LPRDATA)GetWindowLong(hDlg, DWL_USER);

		switch (wmCommandID)
		{
		case IDOK:
			{
			HWND edit = GetDlgItem( hDlg , IDC_DATA );
			int size = GetWindowTextLength( edit );
			char* buffer = (char*)malloc(size+1);
			GetWindowText( edit , buffer , size+1 );
			rdPtr->ini->newUndoBlock();
			rdPtr->ini->LoadStream( (std::string)buffer , true , true );
			free(buffer);
			}

			// Close the dialog
			EndDialog(hDlg, IDOK);
			return 0;

		case IDCANCEL:
			// User pressed cancel, don't save anything
			// Close the dialog
			EndDialog(hDlg, IDCANCEL);
			return 0;

		default:
			break;
		}
		break;
		case WM_SIZE:
		{
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
		
			SetWindowPos( GetDlgItem( hDlg , IDC_DATA ) , HWND_TOP , 0 , 0 , width , height-40 , SWP_NOMOVE | SWP_NOZORDER );
			SetWindowPos( GetDlgItem( hDlg , IDOK ) , HWND_TOP , width-108 , height-34 , 0,0 , SWP_NOSIZE | SWP_NOZORDER );
			SetWindowPos( GetDlgItem( hDlg , IDCANCEL ) , HWND_TOP , width-215 , height-34 , 0,0 , SWP_NOSIZE | SWP_NOZORDER );
			return 0;
		}
		break;

	default:
		break;
	}
	return FALSE;
}



#endif // !defined(RUN_ONLY)


void WINAPI DLLExport EditDebugItem(LPRDATA rdPtr, int id)
{
#if !defined(RUN_ONLY)

	GLOBAL_SETTINGS_INTO_MEMORY();
	// Example
	// -------

	switch (id)
	{
	case DB_CURRENTDATA:
			DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_DEBUG), NULL, debugboxProc, (LPARAM)rdPtr);
		break;
	case DB_TREE:
			rdPtr->dialogDisplay = INI_DISPLAY;	//INI_DISPLAY mode ('a = b' style)
			rdPtr->dialogSettings = DEBUG_MODE_TITLE | NO_REFRESH; //No silly disallowing settings for us, nu-hu
			DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_DEBUG2), NULL, debugboxProc2, (LPARAM)rdPtr);
		break;
	case DB_GLOBALSLOTS:
			DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_SLOTVIEW), NULL, debugboxProc3, (LPARAM)rdPtr);
	}

#endif // !defined(RUN_ONLY)
}


