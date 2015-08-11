// Object identifier
#define IDENTIFIER	0x12FD53A0
#pragma warning( disable : 4800 )
//Routine count definitions
#include	"shlobj.h"

#define PARAM_OBJECTSELECTOR                (PARAM_EXTBASE)
#define PARAM_OBJECTSELECTOR2               (PARAM_EXTBASE)+1
#define PARAM_DIALOGSELECTOR				(PARAM_EXTBASE)+2
#define PARAM_OBJECTSELECTOR3               (PARAM_EXTBASE)+3
#define PARAM_SSS			                (PARAM_EXTBASE)+4
#define PARAM_SEARCH		                (PARAM_EXTBASE)+5
#define PARAM_OBJECTSELECTOR4               (PARAM_EXTBASE)+6
#define PARAM_OBJECTSELECTOR5               (PARAM_EXTBASE)+7
#define PARAM_LOADFILE		                (PARAM_EXTBASE)+8


#define	CND_LAST	20
#define	ACT_LAST	84
#define	EXP_LAST    41


#include "SuperINI.h"
#include "shlwapi.h"

//Events
#define		DIALOGBOX_OPEN	7
#define		DIALOGBOX_CLOSE 8
#define		DIALOGBOX_MODIFIED 9
#define		INI_MODIFIED 10
#define		INI_SAVED 11
#define		INI_NEWITEM 12
#define		ON_SEARCH_FOUND 13
#define		ON_NOT_SEARCH_FOUND 14
#define		ON_MERGE 15
#define		ON_UNCOMPRESS_OK 16
#define		ON_UNCOMPRESS_FAIL 17




#define		GLOBAL_SETTINGS_INTO_MEMORY()		{if ( rdPtr->global != NULL )rdPtr->ini->ImportSettings( rdPtr->global );if (rdPtr->autoLoad )rdPtr->ini->RequestAutoLoad();}
#define		GLOBAL_SETTINGS_CHANGE()			{if ( rdPtr->global != NULL )rdPtr->ini->ExportSettings( rdPtr->global );}


#define FormatPath(z) FormatPathDo( rdPtr->defaultDirectory , z , rdPtr->bool_CanCreateFolders )
#define FormatPathReadOnly(z) FormatPathDo( rdPtr->defaultDirectory , z , 0 )

std::string FormatPathDo( std::string defaultDir , std::string x , bool );

#ifdef INCLUDE_DIALOG
	BOOL CALLBACK DLLExport debugboxProc2(HWND hDlg,uint msgType,WPARAM wParam,LPARAM lParam);
#endif

#define DEFAULT_TEXT_MAX_SIZE 3000
#define KEY_SIZE 32
#define NEWLINE_SYMBOL_SIZE 10
// --------------------------------
// EDITION OF OBJECT DATA STRUCTURE
// --------------------------------
typedef struct tagEDATA_V1
{
	extHeader		eHeader;

	bool			b_defaultFile;
	bool			b_ReadOnly;
	char			defaultFile[MAX_PATH];
	int				defaultFolder;
	char			defaultText[DEFAULT_TEXT_MAX_SIZE];
	bool			bool_CanCreateFolders;
	bool			bool_AutoSave;

	bool			bool_stdINI;
	bool			bool_compress;
	bool			bool_encrypt;
	char			encrypt_key[128];
	
	bool			bool_newline;
	char			newline[NEWLINE_SYMBOL_SIZE];
	bool			bool_QuoteStrings;
	int				repeatGroups;

	char			repeatItems;
	
	signed char		undoCount;
	signed char		redoCount;
	char			placeholder;

	bool			saveRepeated;

	bool			bool_EscapeGroup;
	bool			bool_EscapeItem;
	bool			bool_EscapeValue;

	bool			bool_CaseSensitive;

	bool			globalObject;
	bool			index;
	bool			autoLoad;
	bool			subGroups;
	bool			allowEmptyGroups;

	char			globalKey[KEY_SIZE];

} EDITDATA;
typedef EDITDATA _far *			LPEDATA;

// Object versions
#define	KCX_CURRENT_VERSION			1

// --------------------------------
// RUNNING OBJECT DATA STRUCTURE
// --------------------------------
typedef struct tagRDATA
{
	headerObject	rHo;					// Header
	rVal			rv;

	SuperINI*		ini;
	SearchResults*	results;
	std::string		defaultDirectory;
	std::string		currentGroup;

	short			dialogSettings;			// see common dialog.cpp for bit flags
	char			dialogDisplay;
	std::string		dialogTitle;
	HWND			dialogCurrent;			// To make sure only one open at a time. Often 0.
	HWND			dialogChild;			//	Again, often 0.
	HIMAGELIST		icons;
	
	bool			bool_CanCreateFolders;
	SuperINI_SettingsStruct* global;		// = NULL if not in global mode.
//	char			globalKey[32];
	bool			index;
	bool			autoLoad;
	char			duringCallback;			// 0 = No, 1 = Yes, 2 = Yes (dialog already shown)

} RUNDATA;
typedef	RUNDATA	_far *			LPRDATA;



// Size when editing the object under level editor
// -----------------------------------------------
#define	MAX_EDITSIZE			sizeof(EDITDATA)

// Default flags
// -------------
#define	OEFLAGS      			OEFLAG_VALUES

#define	OEPREFS      			0

// If to handle message, specify the priority of the handling procedure
// 0= low, 255= very high. You should use 100 as normal.                                                
// --------------------------------------------------------------------
#define	WINDOWPROC_PRIORITY		100

 
//Array object:
