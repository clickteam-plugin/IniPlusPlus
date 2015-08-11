
// ============================================================================
//
// This file contains routines that are handled only during the Edittime,
// under the Frame and Event editors.
//
// Including creating, display, and setting up your object.
// 
// ============================================================================

// Common
#include	"common.h"

#if !defined(RUN_ONLY)
#include "common_dialog.h"
// PROPERTIES /////////////////////////////////////////////////////////////////

// Property identifiers



enum {

PROPID_PATHTITLE = PROPID_EXTITEM_CUSTOM_FIRST,
PROPID_DEFPATH,
PROPID_READONLY,
PROPID_DEFFOLDER,
PROPID_DEFAULTTEXT,
PROPID_INDEX,
PROPID_CREATEFOLDERS,
PROPID_AUTOLOAD,
PROPID_AUTOSAVE,			
PROPID_INITITLE,
PROPID_GLOBALDATA,
PROPID_GLOBALDATANAME,
PROPID_STANDARDQM,
PROPID_TREETITLE,			
	PROPID_COMPRESS,		
	PROPID_ENCRYPT,			
	PROPID_NEWLINE,
	PROPID_QUOTESTRINGS,
	PROPID_GROUPREPEAT,
	PROPID_REPEATSAVE,
	PROPID_ITEMREPEAT,
	PROPID_ESCAPECHAR_GROUP,
	PROPID_ESCAPECHAR_ITEM,
	PROPID_ESCAPECHAR_VALUE,
	PROPID_CASESENSITIVE,
	PROPID_SUBGROUPS,
	PROPID_ALLOWEMPTYGROUPS,
PROPID_UNDOTITLE,
PROPID_UNDO,
PROPID_REDO,
PROPID_COMPENCYTITLE
};


LPCSTR DefaultFolderList[] = {
0,    // reserved
"Windows Folder",
"(Initial) Current Folder",
"Program Folder",
"Application Data (User)",
"Application Data (Common)",
NULL
};

LPCSTR GroupRepeatList[] = {
0,    // reserved
"First Only",
"Last Only",
"Rename",
"Merge",
NULL
};
LPCSTR ItemRepeatList[] = {
0,    // reserved
"First Only",
"Last Only",
"Rename",
NULL
};
int UndoRange[] = { -1, 127 };

FilenameCreateParam    fcp = { MAKEINTRESOURCE("INI Files|*.ini|All Files|*.*|"), OFN_PATHMUSTEXIST };

PropData Properties[] = {
PropData_CheckBox ( PROPID_GLOBALDATA, IDS_STRING27336,IDS_STRING27337 ),
PropData_EditString( PROPID_GLOBALDATANAME , IDS_STRING27394,IDS_STRING27395 ),

PropData_Group (PROPID_PATHTITLE,IDS_STRING27298,IDS_STRING27298),
PropData_Filename_Check  (PROPID_DEFPATH, IDS_STRING27292,IDS_STRING27293 , &fcp ),
PropData_CheckBox( PROPID_READONLY , IDS_STRING27324 , IDS_STRING27325 ),
PropData_ComboBox (PROPID_DEFFOLDER, IDS_STRING27294,IDS_STRING27295,DefaultFolderList),
PropData_EditMultiLine ( PROPID_DEFAULTTEXT, IDS_STRING27296,IDS_STRING27297 ),
PropData_CheckBox( PROPID_AUTOLOAD , IDS_STRING27379 , IDS_STRING27380 ),
PropData_CheckBox( PROPID_AUTOSAVE , IDS_STRING27309 , IDS_STRING27310 ),
PropData_CheckBox( PROPID_CREATEFOLDERS , IDS_STRING27299 , IDS_STRING27300 ),
PropData_CheckBox( PROPID_INDEX , IDS_STRING27364 , IDS_STRING27365 ),

PropData_Group (PROPID_COMPENCYTITLE,IDS_STRING27402,IDS_STRING27402),
PropData_CheckBox( PROPID_COMPRESS , IDS_STRING27305 , IDS_STRING27306 ),
PropData_EditString_Check( PROPID_ENCRYPT , IDS_STRING27307 , IDS_STRING27308 ),

PropData_Group (PROPID_UNDOTITLE,IDS_STRING27421,IDS_STRING27421),
PropData_SpinEdit( PROPID_UNDO ,  IDS_STRING27419 , IDS_STRING27422, &UndoRange ),
PropData_SpinEdit( PROPID_REDO ,  IDS_STRING27420 , IDS_STRING27423, &UndoRange ),

PropData_End()

};

PropData Properties2[] = {

//put in a title here
PropData_Group (PROPID_INITITLE,IDS_STRING27301,IDS_STRING27301),
PropData_CheckBox( PROPID_STANDARDQM , IDS_STRING27302 , IDS_STRING27303 ),
PropData_Group (PROPID_TREETITLE,IDS_STRING27304,IDS_STRING27311),

	PropData_EditString_Check( PROPID_NEWLINE , IDS_STRING27312 , IDS_STRING27313 ),
	PropData_CheckBox( PROPID_QUOTESTRINGS , IDS_STRING27314 , IDS_STRING27315 ),

	PropData_CheckBox( PROPID_CASESENSITIVE , IDS_STRING27322 , IDS_STRING27323 ),

	PropData_CheckBox( PROPID_ALLOWEMPTYGROUPS , IDS_STRING27387 , IDS_STRING27388 ),
	PropData_CheckBox( PROPID_SUBGROUPS , IDS_STRING27384 , IDS_STRING27385 ),

	PropData_ComboBox (PROPID_GROUPREPEAT, IDS_STRING27316,IDS_STRING27317,GroupRepeatList),
	PropData_ComboBox (PROPID_ITEMREPEAT, IDS_STRING27318,IDS_STRING27317,ItemRepeatList),
	PropData_CheckBox( PROPID_REPEATSAVE , IDS_STRING27343 , IDS_STRING27344 ),

	PropData_CheckBox( PROPID_ESCAPECHAR_GROUP , IDS_STRING27319 , IDS_STRING27319 ),
	PropData_CheckBox( PROPID_ESCAPECHAR_ITEM , IDS_STRING27320 , IDS_STRING27320 ),
	PropData_CheckBox( PROPID_ESCAPECHAR_VALUE , IDS_STRING27321 , IDS_STRING27321 ),

//PropData_Folder_End(),
PropData_End()
};






// SETUP PROC /////////////////////////////////////////////////////////////////

#endif // !defined(RUN_ONLY)


// -----------------
// GetObjInfos
// -----------------
// Return object info
//
// Info displayed in the object's About properties
// Note: ObjComment is also displayed in the Quick Description box in the Insert Object dialog box
//
void WINAPI	DLLExport GetObjInfos (mv _far *mV, LPEDATA edPtr, LPSTR ObjName, LPSTR ObjAuthor, LPSTR ObjCopyright, LPSTR ObjComment, LPSTR ObjHttp)
{
#ifndef RUN_ONLY
	// Name
	LoadString(hInstLib, IDST_OBJNAME,ObjName, 255);

	// Author
	LoadString(hInstLib, IDST_AUTHOR,ObjAuthor,255);

	// Copyright
	LoadString(hInstLib, IDST_COPYRIGHT,ObjCopyright,255);

	// Comment
	LoadString(hInstLib, IDST_COMMENT,ObjComment,1024);

	// Internet address
	LoadString(hInstLib, IDST_HTTP,ObjHttp,255);
#endif // !defined(RUN_ONLY)
}

// -----------------
// GetHelpFileName
// -----------------
// Returns the help filename of the object.
//
LPCSTR WINAPI GetHelpFileName()//TODO
{
#ifndef RUN_ONLY
	return "Help\\Ini++\\index.htm";
#else
	return NULL;
#endif // !defined(RUN_ONLY)
}


// ============================================================================
//
// ROUTINES USED UNDER FRAME EDITOR
// 
// ============================================================================


// --------------------
// CreateObject
// --------------------
// Called when you choose "Create new object". It should display the setup box 
// and initialize everything in the datazone.

int WINAPI DLLExport CreateObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
	{

	edPtr->undoCount = 0;
	edPtr->redoCount = 0;
	edPtr->b_defaultFile = 1;
	edPtr->b_ReadOnly = 0;
	edPtr->bool_CanCreateFolders = 1;
	edPtr->bool_AutoSave = 1;
	strcpy(edPtr->defaultFile , "");
	edPtr->defaultFolder = 2;
	strcpy(edPtr->defaultText , "");

	edPtr->bool_stdINI = 1;
	edPtr->bool_compress = 0;
	edPtr->bool_encrypt = 0;
	strcpy( edPtr->encrypt_key, "" );

	edPtr->bool_newline = false;
	strcpy(edPtr->newline,"");
	edPtr->bool_QuoteStrings = false;
	edPtr->repeatGroups = 3;
	edPtr->repeatItems = 1;

	edPtr->bool_EscapeGroup = 0;
	edPtr->bool_EscapeItem = 0;
	edPtr->bool_EscapeValue = 0;

	edPtr->bool_CaseSensitive = 0;

	edPtr->globalObject = 0;
	edPtr->saveRepeated = 0;
	edPtr->autoLoad = 0;
	edPtr->index = 0;
	edPtr->subGroups = 0;
	edPtr->allowEmptyGroups = 1;
	strcpy(edPtr->globalKey,"");

	return 0;
	}
#endif // !defined(RUN_ONLY)

	// Error
	return -1;
}

// --------------------
// EditObject
// --------------------
// Called when the user selects the Edit command in the object's popup menu
//
BOOL WINAPI EditObject (mv _far *mV, fpObjInfo oiPtr, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
	{
		// Remove this if your object does not need a setup
	/*	setupParams		spa;
		spa.edpt = edPtr;
		spa.kv = mV;
		if ( DialogBoxParam(hInstLib, MAKEINTRESOURCE(DB_SETUP), mV->mvHEditWin, setupProc, (LPARAM)(LPBYTE)&spa) == IDOK )
		{
			return TRUE;
		}*/
		return true;
	}
#endif // !defined(RUN_ONLY)
	return FALSE;
}

// --------------------
// SetEditSize
// --------------------
// Called when the object has been resized
//
// Note: remove the comments if your object can be resized (and remove the comments in the .def file)
/*
BOOL WINAPI SetEditSize(LPMV mv, LPEDATA edPtr, int cx, int cy)
{
#ifndef RUN_ONLY
	edPtr->swidth = cx;
	edPtr->sheight = cy;
#endif // !defined(RUN_ONLY)
	return TRUE;	// OK
}
*/

// --------------------
// PutObject
// --------------------
// Called when each individual object is dropped in the frame.
//
void WINAPI	DLLExport PutObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, ushort cpt)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
}

// --------------------
// RemoveObject
// --------------------
// Called when each individual object is removed from the frame.
//
void WINAPI	DLLExport RemoveObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, ushort cpt)
{
#ifndef RUN_ONLY
	// Is the last object removed?
    if (0 == cpt)
	{
		// Do whatever necessary to remove our data
	}
#endif // !defined(RUN_ONLY)
}

// --------------------
// DuplicateObject
// --------------------
// Called when an object is created from another one (note: should be called CloneObject instead...)
//
void WINAPI DLLExport DuplicateObject(mv __far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
}

// --------------------
// GetObjectRect
// --------------------
// Returns the size of the rectangle of the object in the frame editor.
//
void WINAPI DLLExport GetObjectRect(mv _far *mV, RECT FAR *rc, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	rc->right = rc->left + 32;	// edPtr->swidth;
	rc->bottom = rc->top + 32;	// edPtr->sheight;
#endif // !defined(RUN_ONLY)
	return;
}


// --------------------
// EditorDisplay
// --------------------
// Displays the object under the frame editor
//
// Note: this function is optional. If it's not defined in your extension,
// MMF2 will load and display the EXO_IMAGE bitmap if it's defined in your resource file.
//
// If you need to draw the icon manually, remove the comments around this function and in the .def file.
//

/*void WINAPI DLLExport EditorDisplay(mv _far *mV, fpObjInfo oiPtr, fpLevObj loPtr, LPEDATA edPtr, RECT FAR *rc)
{
#ifndef RUN_ONLY

	// This is a simple case of drawing an image onto MMF's frame editor window
	// First, we must get a pointer to the surface used by the frame editor

	LPSURFACE ps = WinGetSurface((int)mV->mvIdEditWin);
	if ( ps != NULL )		// Do the following if this surface exists
	{
		int x = rc->left;	// get our boundaries
		int y = rc->top;
		int w = rc->right-rc->left;
		int h = rc->bottom-rc->top;

		cSurface is;			// New surface variable for us to use
		is.Create(4, 4, ps);	// Create a surface implementation from a prototype (frame editor win)
		is.LoadImage(hInstLib, EXO_IMAGE, LI_REMAP);	// Load our bitmap from the resource,
														// and remap palette if necessary
		is.Blit(*ps, x, y, BMODE_TRANSP, BOP_COPY, 0);	// Blit the image to the frame editor surface!
		// This actually blits (or copies) the whole of our surface onto the frame editor's surface
		// at a specified position.
		// We could use different image effects when we copy, e.g. invert, AND, OR, XOR,
		// blend (semi-transparent, the 6th param is amount of transparency)
		// You can 'anti-alias' with the 7th param (default=0 or off)
	}

#endif // !defined(RUN_ONLY)
}*/


// --------------------
// IsTransparent
// --------------------
// This routine tells CC&C if the mouse pointer is over a transparent zone of the object.
// 

extern "C" BOOL WINAPI DLLExport IsTransparent(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, int dx, int dy)
{
#ifndef RUN_ONLY
	// Write your code here
#endif // !defined(RUN_ONLY)
	return FALSE;
}

// --------------------
// PrepareToWriteObject
// --------------------
// Just before writing the datazone when saving the application, CC&C calls this routine.
// 
void WINAPI	DLLExport PrepareToWriteObject(mv _far *mV, LPEDATA edPtr, fpObjInfo adoi)
{
#ifndef RUN_ONLY
	// Write your code here
#endif // !defined(RUN_ONLY)
}

// --------------------
// GetFilters
// --------------------

BOOL WINAPI GetFilters(LPMV mV, LPEDATA edPtr, DWORD dwFlags, LPVOID pReserved)
{
#ifndef RUN_ONLY
	// If your extension uses image filters
//	if ( (dwFlags & GETFILTERS_IMAGES) != 0 )
//		return TRUE;

	// If your extension uses sound filters
//	if ( (dwFlags & GETFILTERS_SOUNDS) != 0 )
//		return TRUE;
#endif // RUN_ONLY
	return FALSE;
}

// --------------------
// UsesFile
// --------------------
// Triggers when a file is dropped onto the frame
// Return TRUE if you can create an object from the given file
//
BOOL WINAPI	DLLExport UsesFile (LPMV mV, LPSTR fileName)
{
	BOOL r = FALSE;
#ifndef RUN_ONLY


	LPSTR	ext, npath;

	if ( fileName != NULL )
	{
		if ( (ext=(LPSTR)malloc(_MAX_EXT)) != NULL )
		{
			if ( (npath=(LPSTR)malloc(_MAX_PATH)) != NULL )
			{
				strcpy(npath, fileName);
				_splitpath(npath, NULL, NULL, NULL, ext);
				if ( _stricmp(ext, ".ini") == 0 )
					r = TRUE;
				free(npath);
			}
			free(ext);
		}
	} 
#endif // !defined(RUN_ONLY)
	return r;
}


// --------------------
// CreateFromFile
// --------------------
// Creates a new object from file
//
void WINAPI	DLLExport CreateFromFile (LPMV mV, LPSTR fileName, LPEDATA edPtr)
{
#ifndef RUN_ONLY

	edPtr->undoCount = 0;
	edPtr->redoCount = 0;
	edPtr->b_defaultFile = 1;
	edPtr->b_ReadOnly = 0;
	edPtr->bool_CanCreateFolders = 1;
	edPtr->bool_AutoSave = 1;
	strcpy(edPtr->defaultFile , fileName);
	edPtr->defaultFolder = 2;
	strcpy(edPtr->defaultText , "");

	edPtr->bool_stdINI = 1;
	edPtr->bool_compress = 0;
	edPtr->bool_encrypt = 0;
	strcpy( edPtr->encrypt_key, "" );

	edPtr->bool_newline = false;
	strcpy(edPtr->newline,"");
	edPtr->bool_QuoteStrings = false;
	edPtr->repeatGroups = 2;
	edPtr->repeatItems = 1;

	edPtr->bool_EscapeGroup = 0;
	edPtr->bool_EscapeItem = 0;
	edPtr->bool_EscapeValue = 0;

	edPtr->bool_CaseSensitive = 0;
	edPtr->globalObject = 0;
	edPtr->saveRepeated = 0;
	edPtr->autoLoad = 0;
	edPtr->index = 0;
	edPtr->subGroups = 0;
	edPtr->allowEmptyGroups = 1;
	strcpy(edPtr->globalKey,"");

#endif // !defined(RUN_ONLY)
}

// ============================================================================
//
// PROPERTIES
// 
// ============================================================================

// --------------------
// GetProperties
// --------------------
// Inserts properties into the properties of the object.
//
BOOL WINAPI DLLExport GetProperties(LPMV mV, LPEDATA edPtr, BOOL bMasterItem)
{
#ifndef RUN_ONLY
	mvInsertProps(mV, edPtr, Properties, PROPID_TAB_GENERAL, TRUE);
	mvInsertProps(mV, edPtr, Properties2, PROPID_TAB_CUSTOM1, TRUE);

#endif // !defined(RUN_ONLY)

	// OK
	return TRUE;
}

// --------------------
// ReleaseProperties
// --------------------
// Called when the properties are removed from the property window.
//
void WINAPI DLLExport ReleaseProperties(LPMV mV, LPEDATA edPtr, BOOL bMasterItem)
{
#ifndef RUN_ONLY
	// Write your code here
#endif // !defined(RUN_ONLY)
}

// --------------------
// GetPropCreateParam
// --------------------
// Called when a property is initialized and its creation parameter is NULL (in the PropData).
// Allows you, for example, to change the content of a combobox property according to specific settings in the EDITDATA structure.
//
LPARAM WINAPI DLLExport GetPropCreateParam(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	// Example
	// -------
//	if ( nPropID == PROPID_COMBO )
//	{
//		switch (edPtr->sType)
//		{
//		case TYPE1:
//			return (LPARAM)ComboList1;
//		case TYPE2:
//			return (LPARAM)ComboList2;
//		}
//	}
#endif // !defined(RUN_ONLY)
	return NULL;
}

// ----------------------
// ReleasePropCreateParam
// ----------------------
// Called after a property has been initialized.
// Allows you, for example, to free memory allocated in GetPropCreateParam.
//
void WINAPI DLLExport ReleasePropCreateParam(LPMV mV, LPEDATA edPtr, UINT nPropID, LPARAM lParam)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
}

// --------------------
// GetPropValue
// --------------------
// Returns the value of properties that have a value.
// Note: see GetPropCheck for checkbox properties
//
LPVOID WINAPI DLLExport GetPropValue(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	// Example
	// -------

    switch (nPropID) {


    case PROPID_DEFPATH:
		return new CPropDataValue(edPtr->defaultFile);
	
	case PROPID_DEFFOLDER:
		return new CPropDWordValue(edPtr->defaultFolder);

	case PROPID_DEFAULTTEXT:
		return new CPropDataValue(edPtr->defaultText);

	case PROPID_ENCRYPT:
		return new CPropDataValue(edPtr->encrypt_key);

	case PROPID_NEWLINE:
		return new CPropDataValue(edPtr->newline);

	case PROPID_GROUPREPEAT:
		return new CPropDWordValue(edPtr->repeatGroups);

	case PROPID_ITEMREPEAT:
		return new CPropDWordValue(edPtr->repeatItems);

	case PROPID_GLOBALDATANAME:
		return new CPropDataValue(edPtr->globalKey);
	
	case PROPID_UNDO:
		return new CPropDWordValue(edPtr->undoCount);

	case PROPID_REDO:
		return new CPropDWordValue(edPtr->redoCount);

    }
   //return new CPropDataValue("BLAH");
#endif // !defined(RUN_ONLY)
	return NULL;
}

// --------------------
// GetPropCheck
// --------------------
// Returns the checked state of properties that have a check box.
//
BOOL WINAPI DLLExport GetPropCheck(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY

	switch ( nPropID )
	{
	case PROPID_DEFPATH:	//Unchecked if either the data is that, or if the text is blank
		return edPtr->b_defaultFile;
	case PROPID_CREATEFOLDERS:
		return edPtr->bool_CanCreateFolders;
	case PROPID_AUTOSAVE:
		return edPtr->bool_AutoSave;
	case PROPID_STANDARDQM:
		return edPtr->bool_stdINI;
	case PROPID_COMPRESS:
		return edPtr->bool_compress;
	case PROPID_ENCRYPT:
		return edPtr->bool_encrypt;
	case PROPID_NEWLINE:
		return edPtr->bool_newline;
	case PROPID_QUOTESTRINGS:
		return edPtr->bool_QuoteStrings;
	case PROPID_ESCAPECHAR_GROUP:
		return edPtr->bool_EscapeGroup;
	case PROPID_ESCAPECHAR_ITEM:
		return edPtr->bool_EscapeItem;
	case PROPID_ESCAPECHAR_VALUE:
		return edPtr->bool_EscapeValue;
	case PROPID_CASESENSITIVE:
		return edPtr->bool_CaseSensitive;
	case PROPID_READONLY:
		return edPtr->b_ReadOnly;
	case PROPID_GLOBALDATA:
		return edPtr->globalObject;
	case PROPID_REPEATSAVE:
		return edPtr->saveRepeated;
	case PROPID_INDEX:
		return edPtr->index;
	case PROPID_AUTOLOAD:
		return edPtr->autoLoad;
	case PROPID_SUBGROUPS:
		return edPtr->subGroups;
	case PROPID_ALLOWEMPTYGROUPS:
		return edPtr->allowEmptyGroups;


	}
    return 0;        // Unchecked

#endif // !defined(RUN_ONLY)
	return 0;		// Unchecked
}




// --------------------
// SetPropValue
// --------------------
// This routine is called by MMF after a property has been modified.
//
void WINAPI DLLExport SetPropValue(LPMV mV, LPEDATA edPtr, UINT nPropID, LPVOID lParam)
{
#ifndef RUN_ONLY
	// Gets the pointer to the CPropValue structure
	CPropValue* pValue = (CPropValue*)lParam;

    switch (nPropID) {

    case PROPID_DEFPATH:
		{
			LPSTR pStr;
            pStr = (LPSTR)((CPropDataValue*)pValue)->m_pData;
            lstrcpyn(edPtr->defaultFile, pStr,MAX_PATH);
        break;
		}
	case PROPID_DEFAULTTEXT:
		{
			LPSTR pStr;
            pStr = (LPSTR)((CPropDataValue*)pValue)->m_pData;
            lstrcpyn(edPtr->defaultText, pStr, DEFAULT_TEXT_MAX_SIZE );
			edPtr->defaultText[DEFAULT_TEXT_MAX_SIZE-1] = 0;

		break;
		}
	case PROPID_DEFFOLDER:
			edPtr->defaultFolder = ((CPropDWordValue*)pValue)->m_dwValue;
		break;
	case PROPID_ENCRYPT:
		{
			LPSTR pStr;
            pStr = (LPSTR)((CPropDataValue*)pValue)->m_pData;
            lstrcpyn(edPtr->encrypt_key, pStr,KEY_SIZE);
        break;
		}

	case PROPID_NEWLINE:
		{
			LPSTR pStr;
            pStr = (LPSTR)((CPropDataValue*)pValue)->m_pData;
            lstrcpyn(edPtr->newline, pStr,NEWLINE_SYMBOL_SIZE);
        break;
		}
	case PROPID_GROUPREPEAT:
			edPtr->repeatGroups = ((CPropDWordValue*)pValue)->m_dwValue;
			break;
	case PROPID_ITEMREPEAT:
			edPtr->repeatItems = ((CPropDWordValue*)pValue)->m_dwValue;
			break;
	case PROPID_GLOBALDATANAME:
		{
			LPSTR pStr;
            pStr = (LPSTR)((CPropDataValue*)pValue)->m_pData;
            lstrcpyn(edPtr->globalKey, pStr,KEY_SIZE);
        break;
		}
	case PROPID_UNDO:
		edPtr->undoCount = ((CPropDWordValue*)pValue)->m_dwValue;
		break;
	case PROPID_REDO:
		edPtr->redoCount = ((CPropDWordValue*)pValue)->m_dwValue;
		break;
    }
    // You may want to have your object redrawn in the frame editor after the modifications,
    // in this case, just call this function:

#endif // !defined(RUN_ONLY)
}

// --------------------
// SetPropCheck
// --------------------
// This routine is called by MMF when the user modifies a checkbox in the properties.
//
void WINAPI DLLExport SetPropCheck(LPMV mV, LPEDATA edPtr, UINT nPropID, BOOL nCheck)
{
#ifndef RUN_ONLY
	// Example
	// -------
	switch ( nPropID )
	{
	case PROPID_DEFPATH:
		edPtr->b_defaultFile = nCheck;
		mvRefreshProp(mV,edPtr,PROPID_READONLY,FALSE);
		return;
	case PROPID_CREATEFOLDERS:
		edPtr->bool_CanCreateFolders = nCheck;
		return;
	case PROPID_AUTOSAVE:
		edPtr->bool_AutoSave = nCheck;
		return;
	case PROPID_STANDARDQM:
		edPtr->bool_stdINI = nCheck;

		//mvRefreshProp(mV,edPtr,PROPID_COMPRESS,FALSE);
		//mvRefreshProp(mV,edPtr,PROPID_ENCRYPT,FALSE);
		mvRefreshProp(mV,edPtr,PROPID_NEWLINE,FALSE);
		mvRefreshProp(mV,edPtr,PROPID_QUOTESTRINGS,FALSE);
		mvRefreshProp(mV,edPtr,PROPID_GROUPREPEAT,FALSE);
		mvRefreshProp(mV,edPtr,PROPID_ITEMREPEAT,FALSE);
		mvRefreshProp(mV,edPtr,PROPID_ESCAPECHAR_GROUP,FALSE);
		mvRefreshProp(mV,edPtr,PROPID_ESCAPECHAR_ITEM,FALSE);
		mvRefreshProp(mV,edPtr,PROPID_ESCAPECHAR_VALUE,FALSE);
		mvRefreshProp(mV,edPtr,PROPID_CASESENSITIVE,FALSE);
		mvRefreshProp(mV,edPtr,PROPID_REPEATSAVE,FALSE);
		mvRefreshProp(mV,edPtr,PROPID_SUBGROUPS,FALSE);
		mvRefreshProp(mV,edPtr,PROPID_ALLOWEMPTYGROUPS,FALSE);
		
		return;
	case PROPID_COMPRESS:
		edPtr->bool_compress = nCheck;
		return;
	case PROPID_ENCRYPT:
		edPtr->bool_encrypt = nCheck;
		return;
	case PROPID_NEWLINE:
		edPtr->bool_newline = nCheck;
		return;
	case PROPID_QUOTESTRINGS:
		edPtr->bool_QuoteStrings = nCheck;
		return;
	case PROPID_ESCAPECHAR_GROUP:
		edPtr->bool_EscapeGroup = nCheck;
		return;
	case PROPID_ESCAPECHAR_ITEM:
		edPtr->bool_EscapeItem = nCheck;
		return;
	case PROPID_ESCAPECHAR_VALUE:
		edPtr->bool_EscapeValue = nCheck;
		return;
	case PROPID_CASESENSITIVE:
		edPtr->bool_CaseSensitive = nCheck;
		return;
	case PROPID_READONLY:
		edPtr->b_ReadOnly = nCheck;
		return;
	case PROPID_GLOBALDATA:
		edPtr->globalObject = nCheck;
		mvRefreshProp(mV,edPtr,PROPID_GLOBALDATANAME,FALSE);
		return;
	case PROPID_REPEATSAVE:
		edPtr->saveRepeated = nCheck;
		return;
	case PROPID_INDEX:
		edPtr->index = nCheck;
		return;
	case PROPID_AUTOLOAD:
		edPtr->autoLoad = nCheck;
		return;
	case PROPID_SUBGROUPS:
		edPtr->subGroups = nCheck;
		return;
	case PROPID_ALLOWEMPTYGROUPS:
		edPtr->allowEmptyGroups = nCheck;
	}
#endif // !defined(RUN_ONLY)
}

// --------------------
// EditProp
// --------------------
// This routine is called when the user clicks the button of a Button or EditButton property.
//
BOOL WINAPI DLLExport EditProp(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY

	// Example
	// -------
/*
	if (nPropID==PROPID_EDITCONTENT)
	{
		if ( EditObject(mV, NULL, NULL, edPtr) )
			return TRUE;
	}
*/

#endif // !defined(RUN_ONLY)
	return FALSE;
}

// --------------------
// IsPropEnabled
// --------------------
// This routine returns the enabled state of a property.
//
BOOL WINAPI IsPropEnabled(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	// Example
	// -------

	switch (nPropID)
	{

	//case PROPID_COMPRESS:
	//case PROPID_ENCRYPT:
	case PROPID_NEWLINE:
	case PROPID_QUOTESTRINGS:
	case PROPID_GROUPREPEAT:
	case PROPID_ITEMREPEAT:
	case PROPID_ESCAPECHAR_GROUP:
	case PROPID_ESCAPECHAR_ITEM:
	case PROPID_ESCAPECHAR_VALUE:
	case PROPID_CASESENSITIVE:
	case PROPID_REPEATSAVE:
	case PROPID_SUBGROUPS:
	case PROPID_ALLOWEMPTYGROUPS:
		return (!edPtr->bool_stdINI);
	case PROPID_READONLY: 
		return edPtr->b_defaultFile;
	case PROPID_GLOBALDATANAME:
		return edPtr->globalObject;
	}

#endif // !defined(RUN_ONLY)
	return TRUE;
}


// ============================================================================
//
// TEXT PROPERTIES
// 
// ============================================================================

// --------------------
// GetTextCaps
// --------------------
// Return the text capabilities of the object under the frame editor.
//
DWORD WINAPI DLLExport GetTextCaps(mv _far *mV, LPEDATA edPtr)
{
	return 0;	// (TEXT_ALIGN_LEFT|TEXT_ALIGN_HCENTER|TEXT_ALIGN_RIGHT|TEXT_ALIGN_TOP|TEXT_ALIGN_VCENTER|TEXT_ALIGN_BOTTOM|TEXT_FONT|TEXT_COLOR);
}

// --------------------
// GetTextFont
// --------------------
// Return the font used the object.
// Note: the pStyle and cbSize parameters are obsolete and passed for compatibility reasons only.
//
BOOL WINAPI DLLExport GetTextFont(mv _far *mV, LPEDATA edPtr, LPLOGFONT plf, LPSTR pStyle, UINT cbSize)
{
#if !defined(RUN_ONLY)
	// Example: copy LOGFONT structure from EDITDATA
	// memcpy(plf, &edPtr->m_lf, sizeof(LOGFONT));
#endif // !defined(RUN_ONLY)

	return TRUE;
}

// --------------------
// SetTextFont
// --------------------
// Change the font used the object.
// Note: the pStyle parameter is obsolete and passed for compatibility reasons only.
//
BOOL WINAPI DLLExport SetTextFont(mv _far *mV, LPEDATA edPtr, LPLOGFONT plf, LPCSTR pStyle)
{
#if !defined(RUN_ONLY)
	// Example: copy LOGFONT structure to EDITDATA
	// memcpy(&edPtr->m_lf, plf, sizeof(LOGFONT));
#endif // !defined(RUN_ONLY)

	return TRUE;
}

// --------------------
// GetTextClr
// --------------------
// Get the text color of the object.
//
COLORREF WINAPI DLLExport GetTextClr(mv _far *mV, LPEDATA edPtr)
{
	// Example
	return 0;	// edPtr->fontColor;
}

// --------------------
// SetTextClr
// --------------------
// Set the text color of the object.
//
void WINAPI DLLExport SetTextClr(mv _far *mV, LPEDATA edPtr, COLORREF color)
{
	// Example
	//edPtr->fontColor = color;
}

// --------------------
// GetTextAlignment
// --------------------
// Get the text alignment of the object.
//
DWORD WINAPI DLLExport GetTextAlignment(mv _far *mV, LPEDATA edPtr)
{
	DWORD dw = 0;
#if !defined(RUN_ONLY)
	// Example
	// -------
/*	if ( (edPtr->eData.dwFlags & ALIGN_LEFT) != 0 )
		dw |= TEXT_ALIGN_LEFT;
	if ( (edPtr->eData.dwFlags & ALIGN_HCENTER) != 0 )
		dw |= TEXT_ALIGN_HCENTER;
	if ( (edPtr->eData.dwFlags & ALIGN_RIGHT) != 0 )
		dw |= TEXT_ALIGN_RIGHT;
	if ( (edPtr->eData.dwFlags & ALIGN_TOP) != 0 )
		dw |= TEXT_ALIGN_TOP;
	if ( (edPtr->eData.dwFlags & ALIGN_VCENTER) != 0 )
		dw |= TEXT_ALIGN_VCENTER;
	if ( (edPtr->eData.dwFlags & ALIGN_BOTTOM) != 0 )
		dw |= TEXT_ALIGN_BOTTOM;
*/
#endif // !defined(RUN_ONLY)
	return dw;
}

// --------------------
// SetTextAlignment
// --------------------
// Set the text alignment of the object.
//
void WINAPI DLLExport SetTextAlignment(mv _far *mV, LPEDATA edPtr, DWORD dwAlignFlags)
{
#if !defined(RUN_ONLY)
	// Example
	// -------
/*	DWORD dw = edPtr->eData.dwFlags;

	if ( (dwAlignFlags & TEXT_ALIGN_LEFT) != 0 )
		dw = (dw & ~(ALIGN_LEFT|ALIGN_HCENTER|ALIGN_RIGHT)) | ALIGN_LEFT;
	if ( (dwAlignFlags & TEXT_ALIGN_HCENTER) != 0 )
		dw = (dw & ~(ALIGN_LEFT|ALIGN_HCENTER|ALIGN_RIGHT)) | ALIGN_HCENTER;
	if ( (dwAlignFlags & TEXT_ALIGN_RIGHT) != 0 )
		dw = (dw & ~(ALIGN_LEFT|ALIGN_HCENTER|ALIGN_RIGHT)) | ALIGN_RIGHT;

	if ( (dwAlignFlags & TEXT_ALIGN_TOP) != 0 )
		dw = (dw & ~(ALIGN_TOP|ALIGN_VCENTER|ALIGN_BOTTOM)) | ALIGN_TOP;
	if ( (dwAlignFlags & TEXT_ALIGN_VCENTER) != 0 )
		dw = (dw & ~(ALIGN_TOP|ALIGN_VCENTER|ALIGN_BOTTOM)) | ALIGN_VCENTER;
	if ( (dwAlignFlags & TEXT_ALIGN_BOTTOM) != 0 )
		dw = (dw & ~(ALIGN_TOP|ALIGN_VCENTER|ALIGN_BOTTOM)) | ALIGN_BOTTOM;

	edPtr->eData.dwFlags = dw;
*/
#endif // !defined(RUN_ONLY)
}


// ============================================================================
//
// ROUTINES USED UNDER EVENT / TIME / STEP-THROUGH EDITOR
// You should not need to change these routines
// 
// ============================================================================

// -----------------
// menucpy
// -----------------
// Internal routine used later, copy one menu onto another
// 
#ifndef RUN_ONLY
void menucpy(HMENU hTargetMenu, HMENU hSourceMenu)
{
	int			n, id, nMn;
	NPSTR		strBuf;
	HMENU		hSubMenu;

	nMn = GetMenuItemCount(hSourceMenu);
	strBuf = (NPSTR)LocalAlloc(LPTR, 80);
	for (n=0; n<nMn; n++)
	{
		if (0 == (id = GetMenuItemID(hSourceMenu, n)))
			AppendMenu(hTargetMenu, MF_SEPARATOR, 0, 0L);
		else
		{
			GetMenuString(hSourceMenu, n, strBuf, 80, MF_BYPOSITION);
			if (id != -1)
				AppendMenu(hTargetMenu, GetMenuState(hSourceMenu, n, MF_BYPOSITION), id, strBuf);
			else
			{
				hSubMenu = CreatePopupMenu();
				AppendMenu(hTargetMenu, MF_POPUP | MF_STRING, (uint)hSubMenu, strBuf);
				menucpy(hSubMenu, GetSubMenu(hSourceMenu, n));
			}
		}
	}
	LocalFree((HLOCAL)strBuf);
}

// -----------------
// GetPopupMenu
// -----------------
// Internal routine used later. Returns the first popup from a menu
// 
HMENU GetPopupMenu(short mn)
{
	HMENU	hMn, hSubMenu, hPopup = NULL;

	if ((hMn = LoadMenu(hInstLib, MAKEINTRESOURCE(mn))) != NULL)
	{
		if ((hSubMenu = GetSubMenu(hMn, 0)) != NULL)
		{
			if ((hPopup = CreatePopupMenu()) != NULL)
				menucpy(hPopup, hSubMenu);
		}
		DestroyMenu(hMn);
	}
	return hPopup;
}

// --------------------
// GetEventInformations
// --------------------
// Internal routine used later. Look for one event in one of the eventInfos array...
// No protection to go faster: you must properly enter the conditions/actions!
// 
static LPEVENTINFOS2 GetEventInformations(LPEVENTINFOS2 eiPtr, short code)

{
	while(eiPtr->infos.code != code)
		eiPtr = EVINFO2_NEXT(eiPtr);
	
	return eiPtr;
}
#endif // !defined(RUN_ONLY)


// ----------------------------------------------------
// GetConditionMenu / GetActionMenu / GetExpressionMenu
// ----------------------------------------------------
// Load the condition/action/expression menu from the resource, eventually
// enable or disable some options, and returns it to CC&C.
//
HMENU WINAPI DLLExport GetConditionMenu(mv _far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
		return GetPopupMenu(MN_CONDITIONS);
#endif // !defined(RUN_ONLY)
	return NULL;
}

HMENU WINAPI DLLExport GetActionMenu(mv _far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
		return GetPopupMenu(MN_ACTIONS);
#endif // !defined(RUN_ONLY)
	return NULL;
}

HMENU WINAPI DLLExport GetExpressionMenu(mv _far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
		return GetPopupMenu(MN_EXPRESSIONS);
#endif // !defined(RUN_ONLY)
	return NULL;
}


// -------------------------------------------------------
// GetConditionTitle / GetActionTitle / GetExpressionTitle
// -------------------------------------------------------
// Returns the title of the dialog box displayed when entering
// parameters for the condition, action or expressions, if any.
// Here, we simply return the title of the menu option
//

#ifndef RUN_ONLY
void GetCodeTitle(LPEVENTINFOS2 eiPtr, short code, short param, short mn, LPSTR strBuf, WORD maxLen)
{
	HMENU		hMn;

	// Finds event in array
	eiPtr=GetEventInformations(eiPtr, code);

	// If a special string is to be returned
	short strID = EVINFO2_PARAMTITLE(eiPtr, param);

	if ( strID != 0 )
		LoadString(hInstLib, strID, strBuf, maxLen);
	else
	{
		// Otherwise, returns the menu option 
		if ((hMn = LoadMenu(hInstLib, MAKEINTRESOURCE(mn))) != NULL )
		{
			GetMenuString(hMn, eiPtr->menu, strBuf, maxLen, MF_BYCOMMAND);
			DestroyMenu(hMn);
		}
	}
}
#else
#define GetCodeTitle(a,b,c,d,e,f)
#endif // !defined(RUN_ONLY)

void WINAPI DLLExport GetConditionTitle(mv _far *mV, short code, short param, LPSTR strBuf, short maxLen)
{
	GetCodeTitle((LPEVENTINFOS2)conditionsInfos, code, param, MN_CONDITIONS, strBuf, maxLen);
}
void WINAPI DLLExport GetActionTitle(mv _far *mV, short code, short param, LPSTR strBuf, short maxLen)
{
	GetCodeTitle((LPEVENTINFOS2)actionsInfos, code, param, MN_ACTIONS, strBuf, maxLen);
}
void WINAPI DLLExport GetExpressionTitle(mv _far *mV, short code, LPSTR strBuf, short maxLen)
{
	GetCodeTitle((LPEVENTINFOS2)expressionsInfos, code, 0, MN_EXPRESSIONS, strBuf, maxLen);
}

// -------------------------------------------------------
// GetConditionTitle / GetActionTitle / GetExpressionTitle
// -------------------------------------------------------
// From a menu ID, these routines returns the code of the condition,
// action or expression, as defined in the .H file
//

short WINAPI DLLExport GetConditionCodeFromMenu(mv _far *mV, short menuId)
{
#ifndef RUN_ONLY
	LPEVENTINFOS2	eiPtr;
	int				n;

	for (n=CND_LAST, eiPtr=(LPEVENTINFOS2)conditionsInfos; n>0 && eiPtr->menu!=menuId; n--)
		eiPtr = EVINFO2_NEXT(eiPtr);
	if (n>0) 
		return eiPtr->infos.code;
#endif // !defined(RUN_ONLY)
	return -1;
}

short WINAPI DLLExport GetActionCodeFromMenu(mv _far *mV, short menuId)
{
#ifndef RUN_ONLY
	LPEVENTINFOS2	eiPtr;
	int				n;

	for (n=ACT_LAST, eiPtr=(LPEVENTINFOS2)actionsInfos; n>0 && eiPtr->menu!=menuId; n--)
		eiPtr = EVINFO2_NEXT(eiPtr);
	if (n>0) 
		return eiPtr->infos.code;
#endif // !defined(RUN_ONLY)
	return -1;
}

//Automatic help.
#ifndef RUN_ONLY

const char*	helpBuffers[] =
{
	//TODO: include iavg.
"This expression goes through each group and calculates the specified operation (sum, average, maximum, etc) of all items in the group. Which operation it performs is specified by the first parameter.\n\nIt then takes all these values and calculations either the sum, average, maximum, etc., of those. Which operation it performs here is specified by the second parameter.\n\nThe possible parameters are:\n+\tSum\n*\tProduct\navg\tAverage\nmin\tMinimum\nmax\tMaximum\nn\tNumber of items (for second parameter only)\n\nYou may also write the name of an item or group and it will only look at those.\n\nFor instance, setting the first parameter to 'max' and the second to 'avg' will give you the average highest value in a group.\n\nRefer to the help file for more information.",
"This expression goes through each group and calculates the specified operation (sum, average, maximum, etc) of all items in the group. Which operation it performs is specified by the first parameter.\n\nIt then takes all these values and calculations either the sum, average, maximum, etc., of those. Which operation it performs here is specified by the second parameter.\n\nThe possible values for the first parameter are:\n+\tSum\n*\tProduct\nAvg\tAverage\nMin\tMinimum\nMax\tMaximum\nn\tNumber of items\n\nYou may also write the name of an item and will only look at items with that name. The second parameter must be either 'min' or 'max'.\n\nFor instance, setting the first parameter to 'n' and the second to 'max' will give you the name of the group with the most items.\n\nRefer to the help file for more information.\n"
};

void DisplayHelp( short menuId )
{
	switch( menuId )
	{
	case ID__GROUPANDITEMCOUNTS_INNERPRODUCT:
		MessageBox( GetActiveWindow() , helpBuffers[0] , "Quick Help for Ini++ Object" , MB_ICONINFORMATION  );
		break;
	case ID__GROUPSANDITEMS_MAXMIN:
		MessageBox( GetActiveWindow() , helpBuffers[1] , "Quick Help for Ini++ Object" , MB_ICONINFORMATION  );
		break;
	default:
		return;
	}
}
#endif // !defined(RUN_ONLY)
short WINAPI DLLExport GetExpressionCodeFromMenu(mv _far *mV, short menuId)
{
#ifndef RUN_ONLY
	LPEVENTINFOS2	eiPtr;
	int				n;

	for (n=EXP_LAST, eiPtr=(LPEVENTINFOS2)expressionsInfos; n>0 && eiPtr->menu!=menuId; n--)
		eiPtr = EVINFO2_NEXT(eiPtr);
	if (n>0) 
	{
		DisplayHelp(menuId);
		return eiPtr->infos.code;

	}
#endif // !defined(RUN_ONLY)
	return -1;
}


// -------------------------------------------------------
// GetConditionInfos / GetActionInfos / GetExpressionInfos
// -------------------------------------------------------
// From a action / condition / expression code, returns 
// an infosEvents structure. 
//

LPINFOEVENTSV2 WINAPI DLLExport GetConditionInfos(mv _far *mV, short code)
{
#ifndef RUN_ONLY
	return &GetEventInformations((LPEVENTINFOS2)conditionsInfos, code)->infos;
#else
	return NULL;
#endif // !defined(RUN_ONLY)
}

LPINFOEVENTSV2 WINAPI DLLExport GetActionInfos(mv _far *mV, short code)
{
#ifndef RUN_ONLY
	return &GetEventInformations((LPEVENTINFOS2)actionsInfos, code)->infos;
#else
	return NULL;
#endif // !defined(RUN_ONLY)
}

LPINFOEVENTSV2 WINAPI DLLExport GetExpressionInfos(mv _far *mV, short code)
{
#ifndef RUN_ONLY
	return &GetEventInformations((LPEVENTINFOS2)expressionsInfos, code)->infos;
#else
	return NULL;
#endif // !defined(RUN_ONLY)
}


// ----------------------------------------------------------
// GetConditionString / GetActionString / GetExpressionString
// ----------------------------------------------------------
// From a action / condition / expression code, returns 
// the string to use for displaying it under the event editor
//

void WINAPI DLLExport GetConditionString(mv _far *mV, short code, LPSTR strPtr, short maxLen)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
		LoadString(hInstLib, GetEventInformations((LPEVENTINFOS2)conditionsInfos, code)->string, strPtr, maxLen);
#endif // !defined(RUN_ONLY)
}

void WINAPI DLLExport GetActionString(mv _far *mV, short code, LPSTR strPtr, short maxLen)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
		LoadString(hInstLib, GetEventInformations((LPEVENTINFOS2)actionsInfos, code)->string, strPtr, maxLen);
#endif // !defined(RUN_ONLY)
}

void WINAPI DLLExport GetExpressionString(mv _far *mV, short code, LPSTR strPtr, short maxLen)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
		LoadString(hInstLib, GetEventInformations((LPEVENTINFOS2)expressionsInfos, code)->string, strPtr, maxLen);
#endif // !defined(RUN_ONLY)
}

// ----------------------------------------------------------
// GetExpressionParam
// ----------------------------------------------------------
// Returns the parameter name to display in the expression editor
//


void WINAPI DLLExport GetExpressionParam(mv _far *mV, short code, short param, LPSTR strBuf, short maxLen)
{
#if !defined(RUN_ONLY)
	short		strID;

	// Finds event in array
	LPEVENTINFOS2 eiPtr=GetEventInformations((LPEVENTINFOS2)expressionsInfos, code);

	// If a special string is to be returned
	strID = EVINFO2_PARAMTITLE(eiPtr, param);
	if ( strID != 0 )
		LoadString(hInstLib, strID, strBuf, maxLen);
	else
		*strBuf=0;
#endif // !defined(RUN_ONLY)
}

// ----------------------------------------------------------
// Custom Parameters
// ----------------------------------------------------------

// --------------------
// InitParameter
// --------------------
// Initialize the parameter.
//
void WINAPI InitParameter(mv _far *mV, short code, paramExt* pExt)
{
#if !defined(RUN_ONLY)

	switch ( code )
	{
	case PARAM_OBJECTSELECTOR:
	case PARAM_OBJECTSELECTOR2:
		strcpy(&pExt->pextData[0], "Ini++ v1.5 Object");
		pExt->pextSize = sizeof(paramExt) + strlen(pExt->pextData)+1;
		return;
	case PARAM_OBJECTSELECTOR3:
		strcpy(&pExt->pextData[0], "Chart");
		pExt->pextSize = sizeof(paramExt) + strlen(pExt->pextData)+1;
		return;
	case PARAM_DIALOGSELECTOR:
		pExt->pextSize = sizeof(paramExt)+2;
		(*((int*)pExt->pextData)) = 0;
		return;
	case PARAM_SSS:
		pExt->pextSize = sizeof(paramExt);
		(pExt->pextData[0]) = 0;
		(pExt->pextData[1]) = 0;
		return;
	case PARAM_SEARCH:
		pExt->pextSize = sizeof(paramExt);
		(pExt->pextData[0]) = 0;
		(pExt->pextData[1]) = 0;
		return;
	case PARAM_LOADFILE:
		pExt->pextSize = sizeof(paramExt)+2*sizeof(char);
		pExt->pextData[0] = pExt->pextData[1] = pExt->pextData[2] = pExt->pextData[3] = 0;
		return;
	}


#endif // !defined(RUN_ONLY)
}

// Example of custom parameter setup proc
// --------------------------------------

struct MMF_Pair
{
	paramExt*	pExt;
	CRunApp*	runApp;
	RunHeader*		mvRunHdr;
	short		code;
};


#if !defined(RUN_ONLY)
#define	ADD_STRING(str) strcpy(buf,str);SendDlgItemMessage( hDlg, IDC_COMBO , CB_ADDSTRING , 0 , (long)buf );




BOOL CALLBACK DLLExport SetupProc5(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	paramExt* pExt;
	switch (msgType)
	{
		case WM_INITDIALOG: // Init dialog
			{
			SetWindowLong(hDlg, DWL_USER, lParam);
			pExt = (paramExt*)lParam;
		
			int n = IDC_RADIO1+pExt->pextData[0];
			SendDlgItemMessage( hDlg, n , BM_SETCHECK , BST_CHECKED , 0 );
			
			n = IDC_RADIO5+pExt->pextData[1];
			SendDlgItemMessage( hDlg, n , BM_SETCHECK , BST_CHECKED , 0 );

			n = IDC_RADIO8+pExt->pextData[2];
			SendDlgItemMessage( hDlg, n , BM_SETCHECK , BST_CHECKED , 0 );

			if ( (pExt->pextData[3] & 1) != 0 )
				SendDlgItemMessage( hDlg, IDC_CHECK1 , BM_SETCHECK , BST_CHECKED , 0 );
			if ( (pExt->pextData[3] & 2) != 0 )
				SendDlgItemMessage( hDlg, IDC_CHECK2 , BM_SETCHECK , BST_CHECKED , 0 );

			SetupProc5(hDlg,WM_COMMAND,0,0);

			}
			return TRUE;

		case WM_COMMAND: // Command
			switch (wmCommandID)
			{
			case IDOK:	// Exit
				pExt = (paramExt *)GetWindowLong(hDlg, DWL_USER);
		
				{
					int n;
					for ( n = IDC_RADIO1 ; n <= IDC_RADIO3 ; n++ )
					{
						if ((SendDlgItemMessage( hDlg , n , BM_GETCHECK , 0 , 0 )==BST_CHECKED))
							pExt->pextData[0] = n - IDC_RADIO1;
					}
					for ( n = IDC_RADIO5 ; n <= IDC_RADIO7 ; n++ )
					{
						if ((SendDlgItemMessage( hDlg , n , BM_GETCHECK , 0 , 0 )==BST_CHECKED))
							pExt->pextData[1] = n - IDC_RADIO5;
					}
					for ( n = IDC_RADIO8 ; n <= IDC_RADIO10 ; n++ )
					{
						if ((SendDlgItemMessage( hDlg , n , BM_GETCHECK , 0 , 0 )==BST_CHECKED))
							pExt->pextData[2] = n - IDC_RADIO8;
					}
					pExt->pextData[3] = 0;
					pExt->pextData[3] |= 1*((SendDlgItemMessage( hDlg , IDC_CHECK1 , BM_GETCHECK , 0 , 0 )==BST_CHECKED)?1:0);
					pExt->pextData[3] |= 2*((SendDlgItemMessage( hDlg , IDC_CHECK2 , BM_GETCHECK , 0 , 0 )==BST_CHECKED)?1:0);
				}

			case IDCANCEL:
				EndDialog(hDlg, TRUE);
				return TRUE;
			default:
				{
					for ( int n = IDC_RADIO1 ; n <= IDC_RADIO4 ; n++ )
					{
							if ((SendDlgItemMessage( hDlg , n , BM_GETCHECK , 0 , 0 )==BST_CHECKED))
							{
								//SendDlgItemMessage( hDlg, IDC_DESCRIPTIONTEXT , WM_SETTEXT , 0,(unsigned int)SSSDescriptions[tmp].c_str() );

								//Disable or enable the others
								bool enable = ( n == IDC_RADIO1 );
								EnableWindow( GetDlgItem(hDlg,IDC_RADIO6) , enable );
								EnableWindow( GetDlgItem(hDlg,IDC_RADIO7) , enable );
								EnableWindow( GetDlgItem(hDlg,IDC_RADIO10) , enable );
								if ( !enable && (SendDlgItemMessage( hDlg , IDC_RADIO5 , BM_GETCHECK , 0 , 0) !=BST_CHECKED) )
								{
									SendDlgItemMessage( hDlg, IDC_RADIO5 , BM_SETCHECK , BST_CHECKED , 0 );
									SendDlgItemMessage( hDlg, IDC_RADIO6 , BM_SETCHECK , BST_UNCHECKED , 0 );
									SendDlgItemMessage( hDlg, IDC_RADIO7 , BM_SETCHECK , BST_UNCHECKED , 0 );	
								}	
								if ( !enable && (SendDlgItemMessage( hDlg , IDC_RADIO10 , BM_GETCHECK , 0 , 0) ==BST_CHECKED) )
								{
									SendDlgItemMessage( hDlg, IDC_RADIO8 , BM_SETCHECK , BST_CHECKED , 0 );
									SendDlgItemMessage( hDlg, IDC_RADIO10 , BM_SETCHECK , BST_UNCHECKED , 0 );
								}							
								break;

							}
					}
				}
				break;


			}

			//default:
				/*
				{
					int tmp = 0;

				*/
			//}
			default:
				break;
	}
	return FALSE;
}




















BOOL CALLBACK DLLExport SetupProc2(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	paramExt* pExt;
	switch (msgType)
	{
		case WM_INITDIALOG: // Init dialog
			{
			SetWindowLong(hDlg, DWL_USER, lParam);
			pExt = (paramExt*)lParam;
			//Set all the check boxes, urrrrrrrggghhh.
			int bitmask = 1;
			int n;
			for ( n = IDC_CHECK1 ; n <= IDC_CHECK10 ; n++ )
			{
				SendDlgItemMessage( hDlg , n , BM_SETCHECK , ((*(int*)(pExt->pextData)) & bitmask)?BST_CHECKED:BST_UNCHECKED , 0 );
				bitmask *= 2;
			}
			char buf[128];
			for ( n = 0 ; n <= MAX_FORMAT_MODE ; n++ )
			{
				ADD_STRING(	FormatTreeItem(n,"item","value").c_str() );
			}
			SendDlgItemMessage( hDlg, IDC_COMBO , CB_SETCURSEL , HIWORD((*(int*)(pExt->pextData))) , 0 );
			SendMessage( hDlg , WM_COMMAND , IDC_CHECK10 , 0 );
			
			}
			return TRUE;

		case WM_COMMAND: // Command
			switch (wmCommandID)
			{
			case IDOK:	// Exit
				pExt = (paramExt *)GetWindowLong(hDlg, DWL_USER);
				pExt->pextSize = sizeof(paramExt)+2;
				{
				short data = 0;
				int bitmask = 1;
				for ( int n = IDC_CHECK1 ; n <= IDC_CHECK10 ; n++ )
				{
					data |= bitmask*((SendDlgItemMessage( hDlg , n , BM_GETCHECK , 0 , 0 )==BST_CHECKED)?1:0);
					bitmask *= 2;
				}
				(*((int*)pExt->pextData)) = MAKELONG( data , SendDlgItemMessage( hDlg, IDC_COMBO , CB_GETCURSEL , 0 , 0 ) );
				}

			case IDCANCEL:
				EndDialog(hDlg, TRUE);
				return TRUE;

			case IDC_CHECK10:
				{
					int state = SendDlgItemMessage( hDlg , IDC_CHECK10 , BM_GETCHECK , 0 , 0 );
					if ( state == BST_CHECKED )
					{
						SendDlgItemMessage( hDlg , IDC_CHECK9 , BM_SETCHECK , BST_CHECKED , 0 );
						SendDlgItemMessage( hDlg , IDC_CHECK6 , BM_SETCHECK , BST_CHECKED , 0 );
					}
					EnableWindow( GetDlgItem(hDlg,IDC_CHECK6) , (state!=BST_CHECKED) );
					EnableWindow( GetDlgItem(hDlg,IDC_CHECK9) , (state!=BST_CHECKED) );
				}
				default:
					break;
			}
			break;
		default:
			break;
	}
	return FALSE;
}


std::string SSSDescriptions[] = {
"Rearranges the items so that they are in alphabetical order based on the items value",
"Randomly rearranges the items in the group",
"Removes the first item in the group",
"Cycles the elements down, so that the current last element is made the first element",
"Removes the last element in the group",
"Cycles the elements up, so the current first element is made the last element",
"Reverses the order of the group",
"Swap every item so that its new name is its old value, and its new value is its old name",
"Names the groups by their ordering, so the first item is called '1', the second '2', etc.",
"Rearranges the items so that they are in alphabetical order based on the items name",
"Rearrages the items so that they are in alphabetical order based on the value before the first comma.\r\n\r\nFor instance, 'example,value' would be sorted as if its value is just 'example'"
};





BOOL CALLBACK DLLExport SetupProc3(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	paramExt* pExt;
	switch (msgType)
	{
		case WM_INITDIALOG: // Init dialog
			{
			SetWindowLong(hDlg, DWL_USER, lParam);
			pExt = (paramExt*)lParam;
		
			int n = IDC_RADIO+pExt->pextData[0];
			SendDlgItemMessage( hDlg, n , BM_SETCHECK , BST_CHECKED , 0 );
			SendDlgItemMessage( hDlg, IDC_DESCRIPTIONTEXT , WM_SETTEXT , 0,(unsigned int)SSSDescriptions[(pExt->pextData[0])].c_str() );
			SendDlgItemMessage( hDlg, IDC_SORTINGRADIO1+pExt->pextData[1] , BM_SETCHECK , BST_CHECKED , 0 );

			
			bool enable = ( n == IDC_RADIO || n == IDC_RADIO10 || n == IDC_RADIO11 );
			EnableWindow( GetDlgItem(hDlg,IDC_SORTINGRADIO1) , enable );
			EnableWindow( GetDlgItem(hDlg,IDC_SORTINGRADIO2) , enable );
			EnableWindow( GetDlgItem(hDlg,IDC_SORTINGRADIO3) , enable );
			}
			return TRUE;

		case WM_COMMAND: // Command
			switch (wmCommandID)
			{
			case IDOK:	// Exit
				pExt = (paramExt *)GetWindowLong(hDlg, DWL_USER);
				pExt->pextSize = sizeof(paramExt);
				{
				short data = 0;
				int bitmask = 1;
				int n;
				for ( n = IDC_RADIO ; n <= IDC_RADIO11 ; n++ )
				{
					if ((SendDlgItemMessage( hDlg , n , BM_GETCHECK , 0 , 0 )==BST_CHECKED))
						pExt->pextData[0] = n - IDC_RADIO;
				}
				for ( n = IDC_SORTINGRADIO1 ; n <= IDC_SORTINGRADIO3 ; n++ )
				{
					if ((SendDlgItemMessage( hDlg , n , BM_GETCHECK , 0 , 0 )==BST_CHECKED))
						pExt->pextData[1] = n - IDC_SORTINGRADIO1;
				}

			}
			case IDCANCEL:
				EndDialog(hDlg, TRUE);
				return TRUE;
			default:
				{
					int tmp = 0;
					for ( int n = IDC_RADIO ; n <= IDC_RADIO11 ; n++ )
					{
						if ((SendDlgItemMessage( hDlg , n , BM_GETCHECK , 0 , 0 )==BST_CHECKED))
						{
							SendDlgItemMessage( hDlg, IDC_DESCRIPTIONTEXT , WM_SETTEXT , 0,(unsigned int)SSSDescriptions[tmp].c_str() );

							
							//Disable or enable the others
							bool enable = ( n == IDC_RADIO || n == IDC_RADIO10 || n == IDC_RADIO11 );
							EnableWindow( GetDlgItem(hDlg,IDC_SORTINGRADIO1) , enable );
							EnableWindow( GetDlgItem(hDlg,IDC_SORTINGRADIO2) , enable );
							EnableWindow( GetDlgItem(hDlg,IDC_SORTINGRADIO3) , enable );
							
							break;

						}
						tmp++;
					}
				}
				


			}
	}
	return FALSE;
}

BOOL CALLBACK DLLExport SetupProc4(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	paramExt* pExt;
	switch (msgType)
	{
		case WM_INITDIALOG: // Init dialog
			{
			SetWindowLong(hDlg, DWL_USER, lParam);
			pExt = (paramExt*)lParam;
		
			SendDlgItemMessage( hDlg, IDC_RADIO+pExt->pextData[0] , BM_SETCHECK , BST_CHECKED , 0 );
			
			}
			{
				int bitmask = 2;
			for ( int n = 1 ; n < 4 ; n++ )
			{
				if ( pExt->pextData[1] & bitmask )
					SendDlgItemMessage( hDlg, IDC_CHECK1 + n , BM_SETCHECK , BST_CHECKED , 0 );
				bitmask *= 2;
			}
			}
			return TRUE;

		case WM_COMMAND: // Command
			switch (wmCommandID)
			{
			case IDOK:	// Exit
				pExt = (paramExt *)GetWindowLong(hDlg, DWL_USER);
				pExt->pextSize = sizeof(paramExt);
				{
					short data = 0;
					int bitmask = 1;
					for ( int n = IDC_RADIO ; n <= IDC_RADIO4 ; n++ )
					{
						if ((SendDlgItemMessage( hDlg , n , BM_GETCHECK , 0 , 0 )==BST_CHECKED))
							pExt->pextData[0] = n - IDC_RADIO;
					}
					{
					int bitmask = 2;
					pExt->pextData[1] = 0;
					for ( int n = 1 ; n < 4 ; n++ )
					{
						if ((SendDlgItemMessage( hDlg , IDC_CHECK1 +  n , BM_GETCHECK , 0 , 0 )==BST_CHECKED))
							pExt->pextData[1] |= bitmask;
						bitmask *= 2;
					}
				}
			}
			case IDCANCEL:
				EndDialog(hDlg, TRUE);
				return TRUE;
			}
	}
	return FALSE;
}

char strA[] = "Please type the name of an Ini++ object (other than this one) that you wish to use as the source for the merge operation. Please note this is case sensitive.";
char strB[] = "Please type the name of an Ini++ object (other than this one) that you wish to save search results to. Please note this is case sensitive.";
char strC[] = "Please type the name of a Chart Object that you wish to use. Please note this is case sensitive.";
char strD[] = "Please type the name of an Array Object that you wish to recieve data from. Please note this is case sensitive.";
char strE[] = "Please type the name of an Array Object that you wish to put data into. Please note this is case sensitive.";

BOOL CALLBACK DLLExport SetupProc(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	MMF_Pair*			pair;
	paramExt*			pExt;

	switch (msgType)
	{
		case WM_INITDIALOG: // Init dialog

			// Save edptr
			SetWindowLong(hDlg, DWL_USER, lParam);
			pair=(MMF_Pair*)lParam;
			pExt=pair->pExt;

			SetDlgItemText(hDlg, IDC_EDIT, pExt->pextData);

			//Work around to the 'no getting the title string problem'.
			if ( ((MMF_Pair*)lParam)->code == PARAM_OBJECTSELECTOR )
				SetWindowText( GetDlgItem( hDlg , IDC_INFO) , strA ) ;
			else if ( ((MMF_Pair*)lParam)->code == PARAM_OBJECTSELECTOR2 )
				SetWindowText( GetDlgItem( hDlg , IDC_INFO) , strB ) ;
			else if ( ((MMF_Pair*)lParam)->code == PARAM_OBJECTSELECTOR3 )
				SetWindowText( GetDlgItem( hDlg , IDC_INFO) , strC ) ;
			else if ( ((MMF_Pair*)lParam)->code == PARAM_OBJECTSELECTOR4 )
				SetWindowText( GetDlgItem( hDlg , IDC_INFO) , strD ) ;
			else if ( ((MMF_Pair*)lParam)->code == PARAM_OBJECTSELECTOR5 )
				SetWindowText( GetDlgItem( hDlg , IDC_INFO) , strE ) ;

			return TRUE;

		case WM_COMMAND: // Command

			// Retrieve edptr
			pair = (MMF_Pair *)GetWindowLong(hDlg, DWL_USER);
			pExt=pair->pExt;

			switch (wmCommandID)
			{
			case IDOK:	// Exit
				GetDlgItemText(hDlg, IDC_EDIT, pExt->pextData, 500);
				pExt->pextSize=sizeof(paramExt)+strlen(pExt->pextData)+1;
				EndDialog(hDlg, TRUE);
				return TRUE;

				default:
					break;
			}
			break;

		default:
			break;
	}
	return FALSE;
}
#endif // !defined(RUN_ONLY)


// --------------------
// EditParameter
// --------------------
// Edit the parameter.
//
//========= TODO ==========  FOR NEXT VERSION! Make the dialog list objects.
void WINAPI EditParameter(mv _far *mV, short code, paramExt* pExt)
{
#if !defined(RUN_ONLY)

	switch( code )
	{
	case PARAM_OBJECTSELECTOR:
	case PARAM_OBJECTSELECTOR2:
	case PARAM_OBJECTSELECTOR3:
	case PARAM_OBJECTSELECTOR4:
	case PARAM_OBJECTSELECTOR5:
		{
		MMF_Pair pair = { pExt , mV->mvRunApp , mV->mvRunHdr , code };
		DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_OBJECTSELECT), mV->mvHEditWin, SetupProc, (LPARAM)(LPBYTE)&pair);
		}
		return;
	case PARAM_DIALOGSELECTOR:
		DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_OPENDIALOG), mV->mvHEditWin, SetupProc2, (LPARAM)pExt);
		return;
	case PARAM_SSS:
		DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_SSS), mV->mvHEditWin, SetupProc3, (LPARAM)pExt);
		return;
	case PARAM_SEARCH:
		DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_SEARCH), mV->mvHEditWin, SetupProc4, (LPARAM)pExt);
		return;
	case PARAM_LOADFILE:
		DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_LOADMODE), mV->mvHEditWin, SetupProc5, (LPARAM)pExt);
		return;


	}

#endif // !defined(RUN_ONLY)
}

// --------------------
// GetParameterString
// --------------------
// Initialize the parameter.
//

#if !defined(RUN_ONLY)
std::string SettingsNames[] = {
"No deleting groups",
"No deleting items",
"No renaming groups",
"No renaming items",
"No changing values",
"No refresh button",
"No new groups",
"No new items",
"No auto refresh",
"Pause application"
};

std::string SSSTag[] = {
"Sort (by value)",
"Shuffle",
"Remove first element",
"Cycle Up",
"Remove last element",
"Cycle Down",
"Reverse",
"Swap items and values in",
"Renumber",
"Sort (by name)",
"Sort (by first part)"
};

std::string SearchModeNames[] = {
"Disregard previous results",
"Or",
"And",
"Xor"
};
std::string SearchSettingsNames[] = {
"Unused",
"Only add group names to results",
"Match any of the conditions",
"Case sensitive"
};

std::string LoadFileStrings[] = {
//0
"Load file, ",
"Change the current file path, ",
//2
"changing the current file path, and ",
"keeping the old path, and ",
"changing the current file path if the file exists, and ",
//5
"clearing the current data in the object.",
"keeping the current data.",
"clearing the current data in the object if the file exists.",
" (Immedately save)",	//8
" (Read Only)"	//9
};



#endif // !defined(RUN_ONLY)

void WINAPI GetParameterString(mv _far *mV, short code, paramExt* pExt, LPSTR pDest, short size)
{
#if !defined(RUN_ONLY)

	switch ( code )
	{
	case PARAM_SSS:
		wsprintf(pDest,SSSTag[pExt->pextData[0]].c_str());
		return;
	case PARAM_SEARCH:
		{
			std::string str = "Mode: ";
			str += SearchModeNames[pExt->pextData[0]];
			if ( pExt->pextData[1] != 0 )
			{
				str += " (Options: ";
				for ( int n = 0 ; n < 4 ; n++ )
				{
					if ( pExt->pextData[1] & (int)pow(2,n) )
						str += SearchSettingsNames[n] + ", ";
				}
				str = str.substr(0,str.length()-2);
				str += ")";
			}
			wsprintf(pDest,str.c_str());
		}
		
		return;
	case PARAM_OBJECTSELECTOR:
	case PARAM_OBJECTSELECTOR2:
	case PARAM_OBJECTSELECTOR3:
	case PARAM_OBJECTSELECTOR4:
	case PARAM_OBJECTSELECTOR5:
		wsprintf(pDest, "\"%s\"", pExt->pextData);
		return;
	case PARAM_DIALOGSELECTOR:
		{
			int doubles = 1;
			std::string out = "";
			for ( int n = 0 ; n <= 10 ; n++ )
			{
				if ( *(int*)pExt->pextData & doubles )
					out += SettingsNames[n] + ", ";
				doubles *= 2;
			}
			out = out.substr( 0 , out.length()-2 );
			if ( out == "" ) out = "None";
			wsprintf(pDest, "%s", out.c_str());
		}
		return;
	case PARAM_LOADFILE:
		std::string out = "";
		out += LoadFileStrings[pExt->pextData[0]];
		if ( pExt->pextData[0] == 0 )
			out += LoadFileStrings[2+pExt->pextData[1]];
		out += LoadFileStrings[5+pExt->pextData[2]];

		if ( pExt->pextData[3] & 1 )
			out += LoadFileStrings[8];
		if ( pExt->pextData[3] & 2 )
			out += LoadFileStrings[9];

		wsprintf(pDest, "%s", out.c_str());
		return;
	}

#endif // !defined(RUN_ONLY)
}
//#endif // !defined(RUN_ONLY)

