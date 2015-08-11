//Welcome to your auto-generated extension template!
//The first thing you should do is set the active build to debug in the projects menu
//Then build! If there are errors try and find out what caused them
//then check to see it is because of what you typed in in your .aces file

//Use the handy transfer.bat file in the debug directory of the SDK to transfer the cox files to MMF (Close down any projects using the extension first)
//You can get all sorts of crashes if your interface code is wrong so check everything thouroughly or pay later!
//Check each condition, action and expression can be placed and make sure each expression is valid by using the validify button in the expression editor

//If all goes well, happy making!
//What you still need to do :
//	Change the 3 extension build filenames (Project->Settings->V2Template32->Link)
//	Make your icons
//	Make your setup box
//	Fill out your extension info and author info in the string tables
//	Change your version info in the resource file
//	Change your IDENTIFIER, flags and prefrences in main.h
//	Change your runtime data and edittime data structures to suit your needs
//	Fill out all the required routines in Runtime.cpp and Edittime.cpp
//	Flesh out your actions, conditions and expressions
//	Write help files, examples and documentation!

//	***TODO (LIST)***
//	Custom dialog needs to list objects
//
//
//

// ============================================================================
//
// This file are where the Conditions/Actions/Expressions are defined.
//
// ============================================================================

// Common Include
#include	"common.h"
#include	"common_dialog.h"
#include	"helper_to_main.h"

#include	"MMFArrayWrapper.h"
// Quick memo: content of the eventInformations arrays
// ---------------------------------------------------
// Menu ID
// String ID
// Code
// Flags
// Number_of_parameters
// Parameter_type [Number_of_parameters]
// Parameter_TitleString [Number_of_parameters]

short conditionsInfos[]=
	{ 
	CID_conditionHasGroupG,	CID_conditionHasGroupG,	0,	EVFLAGS_ALWAYS | EVFLAGS_NOTABLE,	0,
	CID_conditionHasItemG,	CID_conditionHasItemG,	1,	EVFLAGS_ALWAYS | EVFLAGS_NOTABLE,	1,	PARAM_EXPSTRING,	CP0ID_conditionHasItemG,
	CID_conditionCompareMD5G,	CID_conditionCompareMD5G,	2,	EVFLAGS_ALWAYS | EVFLAGS_NOTABLE,	3,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	CP0ID_conditionCompareMD5G,	CP1ID_conditionCompareMD5G,	CP2ID_conditionCompareMD5G,
	CID_conditionHasGroup,	CID_conditionHasGroup,	3,	EVFLAGS_ALWAYS | EVFLAGS_NOTABLE,	1,	PARAM_EXPSTRING,	CP0ID_conditionHasGroup,
	CID_conditionHasItem,	CID_conditionHasItem,	4,	EVFLAGS_ALWAYS | EVFLAGS_NOTABLE,	2,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	CP0ID_conditionHasItem,	CP1ID_conditionHasItem,
	CID_conditionCompareMD5,	CID_conditionCompareMD5,	5,	EVFLAGS_ALWAYS | EVFLAGS_NOTABLE,	4,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	CP0ID_conditionCompareMD5,	CP1ID_conditionCompareMD5,	CP2ID_conditionCompareMD5,	CP3ID_conditionCompareMD5,
	CID_conditionWildcatMatch,	CID_conditionWildcatMatch,	6,	EVFLAGS_ALWAYS | EVFLAGS_NOTABLE,	3,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	CP0ID_conditionWildcatMatch,	CP1ID_conditionWildcatMatch,	CP2ID_conditionWildcatMatch,

	ID__DIALOGBOX_ONOPEN,	IDS_STRING27338,	DIALOGBOX_OPEN,	0,	0,	
	ID__DIALOGBOX_ONCLOSE,	IDS_STRING27339,	DIALOGBOX_CLOSE,	0,	0,	
	ID__DIALOGBOX_ONMODIFIED,	IDS_STRING27340,	DIALOGBOX_MODIFIED,	0,	0,	
	ID__ONMODIFIED,	IDS_STRING27341,	10,	0,	0,	
	ID__ONAUTOSAVE,	IDS_STRING27342,	11,	0,	0,
	ID__ONNEWITEM,	IDS_STRING27351,	12,	0,	0,	

	ID__ONRESULTSFOUND,	IDS_STRING27354,	13,	0,	0,	
	ID__ONNORESULTSFOUND,	IDS_STRING27355,	14,	0,	0,	
	ID__ONMERGE,	IDS_STRING27356,	15,	0,	0,	
	ID__EXTRAFUNCTIONS_ONUNCOMPRESSIONOK,	IDS_STRING27408,	16,	0,	0,
	ID__EXTRAFUNCTIONS_ONUNCOMPRESSIONFAIL,	IDS_STRING27409,	17,	0,	0,
	ID__UNDO_HASUNDO, IDS_STRING27426, 18,EVFLAGS_ALWAYS | EVFLAGS_NOTABLE,0,
	ID__UNDO_HASREDO, IDS_STRING27427, 19,EVFLAGS_ALWAYS | EVFLAGS_NOTABLE,0,

	0 };


	//Old line 2:
	//AID_actionSetValueG,	AID_actionSetValueG,	1,	0,	3,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_actionSetValueG,	AP1ID_actionSetValueG,
short actionsInfos[]=
	{ 
	AID_actionSetCurrentGroup,	AID_actionSetCurrentGroup,	0,	0,	2,	PARAM_EXPSTRING,PARAM_EXPRESSION,	AP0ID_actionSetCurrentGroup,IDS_STRING27335,
	AID_actionSetValueG,	AID_actionSetValueG,	1,	0,	3,	PARAM_EXPSTRING,	PARAM_EXPRESSION, PARAM_EXPRESSION,	AP0ID_actionSetValueG,	AP2ID_actionSetValueG, AP1ID_actionSetValueG,
	AID_actionSetStringG,	AID_actionSetStringG,	2,	0,	2,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	AP0ID_actionSetStringG,	AP1ID_actionSetStringG,
	AID_actionSetStringMD5G,	AID_actionSetStringMD5G,	3,	0,	2,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	AP0ID_actionSetStringMD5G,	AP1ID_actionSetStringMD5G,
	AID_actionSaveObjectG,	AID_actionSaveObjectG,	4,	0,	3,	PARAM_OBJECT,	PARAM_EXPRESSION,	PARAM_EXPRESSION,	AP0ID_actionSaveObjectG,	AP1ID_actionSaveObjectG,	AP2ID_actionSaveObjectG,
	AID_actionLoadObjectG,	AID_actionLoadObjectG,	5,	0,	1,	PARAM_OBJECT,	AP0ID_actionLoadObjectG,
	AID_actionSavePositionG,	AID_actionSavePositionG,	6,	0,	2,	PARAM_EXPSTRING, PARAM_POSITION,	AP0ID2_actionSavePositionG, AP0ID_actionSavePositionG,
	AID_actionSaveGlobalValuesG,	AID_actionSaveGlobalValuesG,	7,	0,	0,
	AID_actionLoadGlobalValuesG,	AID_actionLoadGlobalValuesG,	8,	0,	0,
	AID_actionDeleteGroupG,	AID_actionDeleteGroupG,	9,	0,	0,
	AID_actionDeleteItemG,	AID_actionDeleteItemG,	10,	0,	1,	PARAM_EXPSTRING,	AP0ID_actionDeleteItemG,
	AID_actionRenameGroupG,	AID_actionRenameGroupG,	11,	0,	2,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_actionRenameGroupG,	AP1ID_actionRenameGroupG,
	AID_actionRenameItemG,	AID_actionRenameItemG,	12,	0,	3,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_actionRenameItemG,	AP1ID_actionRenameItemG,	AP2ID_actionRenameItemG,
	AID_actionMoveItemToGroupG,	AID_actionMoveItemToGroupG,	13,	0,	3,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_actionMoveItemToGroupG,	AP1ID_actionMoveItemToGroupG,	AP2ID_actionMoveItemToGroupG,
	
	AID_actionSetValue,	AID_actionSetValue,	14,	0,	4,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPRESSION, PARAM_EXPRESSION,	AP0ID_actionSetValue,	AP1ID_actionSetValue,	AP2ID_actionSetValueG, AP2ID_actionSetValue,

	AID_actionSetString,	AID_actionSetString,	15,	0,	3,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	AP0ID_actionSetString,	AP1ID_actionSetString,	AP2ID_actionSetString,
	AID_actionSetStringMD5,	AID_actionSetStringMD5,	16,	0,	3,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	AP0ID_actionSetStringMD5,	AP1ID_actionSetStringMD5,	AP2ID_actionSetStringMD5,
	AID_actionSaveObject,	AID_actionSaveObject,	17,	0,	4,	PARAM_OBJECT,PARAM_EXPSTRING,	PARAM_EXPRESSION,PARAM_EXPRESSION,	AP1ID_actionSaveObject,AP0ID_actionSaveObject,AP1ID_actionSaveObjectG,	AP2ID_actionSaveObjectG,
	AID_actionLoadObject,	AID_actionLoadObject,	18,	0,	2,	PARAM_OBJECT,PARAM_EXPSTRING,	AP1ID_actionLoadObject,AP0ID_actionLoadObject,
	
	AID_actionSavePosition,	AID_actionSavePosition,	19,	0,	3,	PARAM_EXPSTRING,PARAM_EXPSTRING,	PARAM_POSITION, AP0ID_actionSavePosition,	AP1ID_actionSetValue, AP1ID_actionSavePosition,

	AID_actionSaveAltValues,	AID_actionSaveAltValues,	20,	0,	1,	PARAM_EXPSTRING,	AP0ID_actionSaveAltValues,
	AID_actionLoadAltValues,	AID_actionLoadAltValues,	21,	0,	1,	PARAM_EXPSTRING,	AP0ID_actionLoadAltValues,
	AID_actionRenameGroup,	AID_actionRenameGroup,	22,	0,	3,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_actionRenameGroup,	AP1ID_actionRenameGroup,	AP2ID_actionRenameGroup,

	AID_actionRenameItem,	AID_actionRenameItem,	23,	0,	4,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_actionRenameItem,	AP1ID_actionRenameItem,	AP2ID_actionRenameItem,	AP4ID_actionRenameItem,

	AID_actionMoveItem,	AID_actionMoveItem,	24,	0,	5,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_actionMoveItem,	AP1ID_actionMoveItem,	AP2ID_actionMoveItem,	AP3ID_actionMoveItem,	AP4ID_actionMoveItem,
	AID_actionMoveItemToGroup,	AID_actionMoveItemToGroup,	25,	0,	4,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_actionMoveItemToGroup,	AP1ID_actionMoveItemToGroup,	AP2ID_actionMoveItemToGroup,	AP3ID_actionMoveItemToGroup,
	AID_actionCopyGroup,	AID_actionCopyGroup,	26,	0,	3,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_actionCopyGroup,	AP1ID_actionCopyGroup,	AP2ID_actionCopyGroup,
	AID_actionCopyItem,	AID_actionCopyItem,	27,	0,	4,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	AP0ID_actionCopyItem,	AP1ID_actionCopyItem,	AP2ID_actionCopyItem,	AP3ID_actionCopyItem,
	AID_actionDeleteGroup,	AID_actionDeleteGroup,	28,	0,	1,	PARAM_EXPSTRING,	AP0ID_actionDeleteGroup,
	AID_actionDeleteItem,	AID_actionDeleteItem,	29,	0,	2,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	AP0ID_actionDeleteItem,	AP1ID_actionDeleteItem,
	AID_actionDeleteItemInAllGroups,	AID_actionDeleteItemInAllGroups,	30,	0,	1,	PARAM_EXPSTRING,	AP0ID_actionDeleteItemInAllGroups,
	AID_actionDeleteFromPattern,	AID_actionDeleteFromPattern,	31,	0,	4,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_actionDeleteFromPattern,	AP1ID_actionDeleteFromPattern,	AP2ID_actionDeleteFromPattern,	AP3ID_actionDeleteFromPattern,
	AID_actionClearAll,	AID_actionClearAll,	32,	0,	0,
	
	//AID_actionPerformSearch,	AID_actionPerformSearch,	33,	0,	5,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	PARAM_EXPRESSION,	AP0ID_actionPerformSearch,	AP1ID_actionPerformSearch,	AP2ID_actionPerformSearch,	AP3ID_actionPerformSearch,	AP4ID_actionPerformSearch,
	AID_actionPerformSearch,	AID_actionPerformSearch,	33,	0,	4,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_SEARCH,	AP0ID_actionPerformSearch,	AP1ID_actionPerformSearch,	AP2ID_actionPerformSearch,	AP2ID_actionPerformSearch,

	AID_actionPerformMultiplesSearch,	AID_actionPerformMultiplesSearch,	34,	0,	2,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	AP0ID_actionPerformMultiplesSearch,	AP1ID_actionPerformMultiplesSearch,
	AID_actionClearResults,	AID_actionClearResults,	35,	0,	0,
	
	AID_actionCreateSubINI,	AID_actionCreateSubINI,	36,	0,	5,	PARAM_FILENAME2,PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,PARAM_EXPRESSION,	AP0ID_actionCreateSubINI, AP1ID_actionCreateSubINI,	AP2ID_actionCreateSubINI,	AP3ID_actionCreateSubINI, AP3ID_actionPerformSearch,
	AID_actionCreateSubINIObject,	AID_actionCreateSubINIObject,	37,	0,	5,	PARAM_OBJECTSELECTOR2,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPSTRING,PARAM_EXPRESSION,	AP0ID_actionCreateSubINIObject, AP1ID_actionCreateSubINIObject,	AP2ID_actionCreateSubINIObject,	AP3ID_actionCreateSubINIObject,AP3ID_actionPerformSearch,
	
	AID_actionMergeFile,	AID_actionMergeFile,	38,	0,	2,	PARAM_FILENAME2,	PARAM_EXPRESSION,	AP0ID_actionMergeFile,	AP1ID_actionMergeFile,
	AID_actionMergeGroupFile,	AID_actionMergeGroupFile,	39,	0,	4,	PARAM_FILENAME2,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_actionMergeGroupFile,	AP1ID_actionMergeGroupFile,	AP2ID_actionMergeGroupFile,	AP3ID_actionMergeGroupFile,
	AID_actionMerge,	AID_actionMerge,	40,	0,	2,	PARAM_OBJECTSELECTOR,	PARAM_EXPRESSION,	AP0ID_actionMerge,	AP1ID_actionMerge,
	AID_actionMergeGroupObject,	AID_actionMergeGroupObject,	41,	0,	4,	PARAM_OBJECTSELECTOR,	PARAM_EXPSTRING,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_actionMergeGroupObject,	AP1ID_actionMergeGroupObject,	AP2ID_actionMergeGroupObject,	AP3ID_actionMergeGroupObject,

	AID_actionNew,	AID_actionNew,	42,	0,	2,	PARAM_FILENAME2,	PARAM_EXPRESSION , AP0ID_actionNew,IDS_STRING27389,
	
	AID_actionLoad,	AID_actionLoad,	43,	0,	2,	PARAM_FILENAME2,	PARAM_EXPRESSION,	AP0ID_actionLoad,	AP1ID_actionLoad,
	AID_actionSave,	AID_actionSave,	44,	0,	0,
	AID_actionSaveAs,	AID_actionSaveAs,	45,	0,	1,	PARAM_FILENAME2,	AP0ID_actionSaveAs,
	AID_actionBackupTo,	AID_actionBackupTo,	46,	0,	3,	PARAM_FILENAME2,	PARAM_EXPRESSION,	PARAM_EXPSTRING,	AP0ID_actionBackupTo,	AP1ID_actionBackupTo,	AP2ID_actionBackupTo,
	AID_actionClose,	AID_actionClose,	47,	0,	0,

	AID_actionLoadFromString,	AID_actionLoadFromString,	48,	0,	2,	PARAM_EXPSTRING,PARAM_EXPRESSION,	AP0ID_actionLoadFromString, IDS_LoadFromString_Clear ,

	AID_actionSetAutoSave,	AID_actionSetAutoSave,	49,	0,	2,	PARAM_EXPRESSION,PARAM_EXPRESSION,	AP0ID_actionSetAutoSave,IDS_STRING27386,
	AID_actionSetCompresson,	AID_actionSetCompresson,	50,	0,	1,	PARAM_EXPRESSION,	AP0ID_actionSetCompresson,
	AID_actionSetEncryption,	AID_actionSetEncryption,	51,	0,	1,	PARAM_EXPSTRING,	AP0ID_actionSetEncryption,
	AID_actionSetReadOnly,	AID_actionSetReadOnly,	52,	0,	1,	PARAM_EXPRESSION,	AP0ID_actionSetReadOnly,
	AID_actionSetCaseSensitive,	AID_actionSetCaseSensitive,	53,	0,	1,	PARAM_EXPRESSION,	AP0ID_actionSetCaseSensitive,
	AID_actionSetEscapeChars,	AID_actionSetEscapeChars,	54,	0,	3,	PARAM_EXPRESSION,	PARAM_EXPRESSION,	PARAM_EXPRESSION,	AP0ID_actionSetEscapeChars,	AP1ID_actionSetEscapeChars,	AP2ID_actionSetEscapeChars,
	AID_actionNeverQuoteStrings,	AID_actionNeverQuoteStrings,	55,	0,	1,	PARAM_EXPRESSION,	AP0ID_actionNeverQuoteStrings,
	AID_actionSetRepeatModes,	AID_actionSetRepeatModes,	56,	0,	3,	PARAM_EXPRESSION,	PARAM_EXPRESSION,PARAM_EXPRESSION,	AP0ID_actionSetRepeatModes,	AP1ID_actionSetRepeatModes,IDS_STRING27345,
	AID_actionSetNewLineChar,	AID_actionSetNewLineChar,	57,	0,	1,	PARAM_EXPSTRING,	AP0ID_actionSetNewLineChar,
	AID_actionSetDefaultDirectory,	AID_actionSetDefaultDirectory,	58,	0,	1,	PARAM_EXPSTRING,	AP0ID_actionSetDefaultDirectory,
	AID_actionCompressFile,	AID_actionCompressFile,	59,	0,	2,	PARAM_FILENAME2,	PARAM_FILENAME2,	AP0ID_actionCompressFile,	AP1ID_actionCompressFile,
	AID_actionDeCompressFile,	AID_actionDeCompressFile,	60,	0,	2,	PARAM_FILENAME2,	PARAM_FILENAME2,	AP0ID_actionDeCompressFile,	AP1ID_actionDeCompressFile,

	AID_actionOpenDialog, AID_actionOpenDialog, 61, 0, 2, PARAM_EXPSTRING, PARAM_DIALOGSELECTOR,IDS_STRING27328,IDS_STRING27326,

	AID_actionAddRepeat, IDS_STRING27334, 62, 0, 4, PARAM_EXPSTRING, PARAM_EXPSTRING,PARAM_EXPSTRING,PARAM_EXPRESSION,IDS_STRING27330,IDS_STRING27331,IDS_STRING27332,IDS_STRING27333,

	ID__DIALOG_CLOSEDIALOG , IDS_STRING27352 , 63 , 0 , 0 ,
	ID__DIALOG_REFRESHDIALOG , IDS_STRING27353 , 64 , 0 , 1 ,PARAM_EXPRESSION, IDS_STRING27357,

	ID__LOADINGANDSAVING_SAVEASCSV, IDS_STRING27370, 65, 0, 4, PARAM_EXPRESSION, PARAM_FILENAME2,PARAM_EXPSTRING,PARAM_EXPSTRING,IDS_STRING27393,IDS_STRING27367,IDS_STRING27368,IDS_STRING27369,

	ID__LOADINGANDSAVING_IMPORTCSV,IDS_STRING27371,66,0,3,	PARAM_EXPRESSION,PARAM_FILENAME2,PARAM_EXPSTRING,IDS_STRING27373,IDS_STRING27372,IDS_STRING27374,
	ID__CHARTOBJECT_DISPLAYDATAINCHARTOBJECT , IDS_STRING27375, 67, 0, 2, PARAM_OBJECTSELECTOR3,PARAM_EXPSTRING,IDS_STRING27357,IDS_STRING27376,
	ID__SEARCHING_FINDSUBGROUPS,IDS_STRING27381,68,0,2,PARAM_EXPSTRING,PARAM_EXPRESSION,IDS_STRING27382,IDS_STRING27383,

	ID__ADVANCEDSETTINGS_SETSUBGROUPSMODE,IDS_STRING27390,69,0,2,PARAM_EXPRESSION,PARAM_EXPRESSION,IDS_STRING27391,IDS_STRING27392,
	
	ID__LOADINGANDSAVING_SWITCHTOGLOBALDATASLOT, IDS_STRING27396,70,0,1,PARAM_EXPSTRING,IDS_STRING27397,

	ID__MOVINGANDCOPYING_SHIFTSORTSHUFFLEGROUP, IDS_STRING27398, 71, 0, 2, PARAM_SSS, PARAM_EXPSTRING,IDS_STRING27399,IDS_STRING27399,

	ID__SETARRAY , IDS_STRING27405 , 72 , 0,4, PARAM_EXPSTRING , PARAM_EXPSTRING , PARAM_EXPRESSION , PARAM_EXPSTRING, IDS_STRING27399 , AP1ID_actionSetString , IDS_STRING27404 , AP2ID_actionSetString ,


	ID__ADVANCEDFEATURES_LOADFROMARRAY, IDS_STRING27406, 73, 0, 2, PARAM_OBJECTSELECTOR4, PARAM_EXPSTRING, AP1ID_actionSetString, AP0ID_actionSetStringMD5,
	ID__ADVANCEDFEATURES_SAVETOANARRAY, IDS_STRING27407, 74, 0, 2, PARAM_OBJECTSELECTOR5, PARAM_EXPSTRING, AP1ID_actionSetString, AP0ID_actionSetStringMD5,
	ID__ADVANCEDFEATURES_CHARTOBJECT_GETDATAFROMCHARTOBJECT,IDS_STRING27410, 75,0,2, PARAM_OBJECTSELECTOR3,PARAM_EXPSTRING,IDS_STRING27357,IDS_STRING27376,

	ID_SAVECHARTSETTINGS,IDS_STRING27412, 76,0,2, PARAM_OBJECTSELECTOR3,PARAM_EXPSTRING,IDS_STRING27357,IDS_STRING27376,
	ID_LOADCHARTSETTINGS,IDS_STRING27411, 77,0,2, PARAM_OBJECTSELECTOR3,PARAM_EXPSTRING,IDS_STRING27357,IDS_STRING27376,

	ID__FILELOADCHANGE,IDS_STRING27413, 78,0,2, PARAM_FILENAME2,PARAM_LOADFILE,AP0ID_actionLoad,IDS_STRING27376,
	ID__ADVANCEDFEATURES_UNDO_PERFORMUNDO, IDS_STRING27417, 79,0,0,
	ID__ADVANCEDFEATURES_UNDO_PERFORMREDO, IDS_STRING27418, 80,0,0,
	ID__ADVANCEDFEATURES_UNDO_CLEARSTACKS, IDS_STRING27424, 81,0,1,PARAM_EXPRESSION,IDS_STRING27425,
	ID__ADVANCEDFEATURES_UNDO_FORCEADDNEWUNDOBLOCK, IDS_STRING27428, 82, 0, 0,
	ID__ADVANCEDFEATURES_UNDO_SETAUTOMATICMODE, IDS_STRING27429, 83, 0, 1, PARAM_EXPRESSION, IDS_STRING27430,
	0 };

short expressionsInfos[]=
	{ 
	EID_expressionGetItemValueG,	EID_expressionGetItemValueG,	0,	0,	2,	EXPPARAM_STRING,	EXPPARAM_LONG,	0,	0,
	EID_expressionGetItemStringG,	EID_expressionGetItemStringG,	1,	EXPFLAG_STRING,	2,	EXPPARAM_STRING,	EXPPARAM_STRING,	0,	0,
	EID_expressionGetXG,	EID_expressionGetXG,	2,	0,	1,	EXPPARAM_STRING,	0,
	EID_expressionGetYG,	EID_expressionGetYG,	3,	0,	1,	EXPPARAM_STRING,	0,
	EID_expressionNthItemG,	EID_expressionNthItemG,	4,	EXPFLAG_STRING,	1,	EXPPARAM_LONG,	0,
	EID_expressionNthItemValueG,	EID_expressionNthItemValueG,	5,	0,	1,	EXPPARAM_LONG,	0,
	EID_expressionNthItemStringG,	EID_expressionNthItemStringG,	6,	EXPFLAG_STRING,	1,	EXPPARAM_LONG,	0,
	EID_expressionItemCountG,	EID_expressionItemCountG,	7,	0,	0,
	EID_expressionGetItemValue,	EID_expressionGetItemValue,	8,	0,	3,	EXPPARAM_STRING,	EXPPARAM_STRING,	EXPPARAM_LONG,	0,	0,	0,
	EID_expressionGetItemString,	EID_expressionGetItemString,	9,	EXPFLAG_STRING,	3,	EXPPARAM_STRING,	EXPPARAM_STRING,	EXPPARAM_STRING,	0,	0,	0,
	EID_expressionGetX,	EID_expressionGetX,	10,	0,	2,	EXPPARAM_STRING,	EXPPARAM_STRING,	0,	0,
	EID_expressionGetY,	EID_expressionGetY,	11,	0,	2,	EXPPARAM_STRING,	EXPPARAM_STRING,	0,	0,

	EID_expressionNthGroup,	EID_expressionNthGroup,	12,	EXPFLAG_STRING,	1,	EXPPARAM_LONG,	0,
	EID_expressionNthItem,	EID_expressionNthItem,	13,	EXPFLAG_STRING,	2,	EXPPARAM_STRING, EXPPARAM_LONG,	0, 0,
	EID_expressionNthItemString,	EID_expressionNthItemString,	14,	EXPFLAG_STRING,	2,	EXPPARAM_STRING, EXPPARAM_LONG,	0,0,
	EID_expressionNthItemValue,	EID_expressionNthItemValue,	15,	0,	2,	EXPPARAM_STRING, EXPPARAM_LONG,	0,0,

	EID_expressionGroupCount,	EID_expressionGroupCount,	16,	0,	0,
	EID_expressionItemCount,	EID_expressionItemCount,	17,	0,	1,	EXPPARAM_STRING,	0,
	EID_expressionTotalItems,	EID_expressionTotalItems,	18,	0,	0,
	EID_expressionSearchResultCounts,	EID_expressionSearchResultCounts,	19,	0,	0,
	EID_expressionGetNthSearchResultGroup,	EID_expressionGetNthSearchResultGroup,	20,	EXPFLAG_STRING,	1,	EXPPARAM_LONG,	0,
	EID_expressionGetNthSearchResultItem,	EID_expressionGetNthSearchResultItem,	21,	EXPFLAG_STRING,	1,	EXPPARAM_LONG,	0,
	EID_expressionGetNthSearchResultItemValue,	EID_expressionGetNthSearchResultItemValue,	22,	0,	1,	EXPPARAM_LONG,	0,
	EID_expressionGetNthSearchResultItemString,	EID_expressionGetNthSearchResultItemString,	23,	EXPFLAG_STRING,	1,	EXPPARAM_LONG,	0,
	EID_expressionGetNthSearchResultPath,	EID_expressionGetNthSearchResultPath,	24,	EXPFLAG_STRING,	2,	EXPPARAM_LONG,	EXPPARAM_STRING,	0,	0,
	EID_expressionWriteStream,	EID_expressionWriteStream,	25,	EXPFLAG_STRING,	0,
	EID_expressionHashString,	EID_expressionHashString,	26,	EXPFLAG_STRING,	1,	EXPPARAM_STRING,	0,
	EID_expressionEncryptString,	EID_expressionEncryptString,	27,	EXPFLAG_STRING,	2,	EXPPARAM_STRING,	EXPPARAM_STRING,	0,	0,
	EID_expressionEscapeString,	EID_expressionEscapeString,	28,	EXPFLAG_STRING,	1,	EXPPARAM_STRING,	0,
	EID_expressionUnEscapeString,	EID_expressionUnEscapeString,	29,	EXPFLAG_STRING,	1,	EXPPARAM_STRING,	0,
	ID__GROUPANDITEMCOUNTS_INNERPRODUCT,	IDS_STRING27358,	30,	0,	2,	EXPPARAM_STRING,EXPPARAM_STRING,	0, 0,
	ID__GROUPSANDITEMS_MAXMIN,	IDS_STRING27360,	31,	EXPPARAM_STRING,	2,	EXPPARAM_STRING,EXPPARAM_STRING,	0, 0 ,

	ID__LISTGROUPSANDITEMS_GETNTHSORTEDITEMNAMEGROUPN,	IDS_STRING27362,	32,	EXPPARAM_STRING,	2,	EXPPARAM_STRING,EXPPARAM_LONG,	0, 0 ,
	ID__LISTGROUPSANDITEMS_GETNTHSORTEDITEMVALUEGROUPN,	IDS_STRING27363,	33,	0,	2,	EXPPARAM_STRING,EXPPARAM_LONG,	0, 0 ,
	ID__GETINIASCSVSTRING, IDS_STRING27366, 34, EXPFLAG_STRING , 2 , EXPPARAM_STRING,EXPPARAM_STRING,	0, 0 ,

	ID__LISTGROUPSANDITEMS_GETNTHITEMUSEDINANYGROUPN, IDS_STRING27400, 35, EXPFLAG_STRING, 1, EXPPARAM_LONG, 0 ,
	ID__COUNTINGANDCALCULATIONS_GETNUMBEROFUNIQUEITEMNAMES,IDS_STRING27401,36, 0 , 0,
	ID__GETPARTINITEM, IDS_STRING27403 , 37 , EXPFLAG_STRING , 4 , EXPPARAM_STRING , EXPPARAM_STRING , EXPPARAM_LONG , EXPPARAM_STRING , 0 , 0 ,0,0,

	ID__CURRENTGROUP_GETCURRENTGROUPNAME,	IDS_STRING27414,	38,	EXPFLAG_STRING,	0,
	ID__CURRENTGROUP_GETCURRENTGROUPASSTRING,	IDS_STRING27415,	39,EXPFLAG_STRING,	0,
	ID__GETCURRENTFILENAME,	IDS_STRING27416,	40,EXPFLAG_STRING,	0,
	0 };


#define ADJUST_FOR_INDEX(x)	{if ( rdPtr->index == 0 ) x++;}
#define RETURN_STDSTRING(str) {LPSTR x = (LPSTR)callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE, str.length()+1 , 0);lstrcpyn( x , str.c_str() , str.length()+1 );return (long)x;}

#ifndef		RUN_ONLY
	bool DuringCallback(LPRDATA rdPtr)
	{
		if ( rdPtr->duringCallback != 2 )
		{
			rdPtr->duringCallback = 2;
			if ( MessageBox(NULL,"Error: You attempted to use an action that may modify an Ini++ object during its 'On Data Modified', 'On New Item' or 'On Save' events. This can cause infinite loops and so is not possible.\n\nThis dialog will not be shown when the application is built.\n\nPress 'Cancel' to end the application or 'OK' to resume.","Ini++ Error",MB_OKCANCEL|MB_ICONEXCLAMATION)
				== IDCANCEL )
				exit(0);
		}
		return 0;
	}
	#define		MODIFIES_DATA()				{if ( rdPtr->duringCallback ) return DuringCallback(rdPtr);rdPtr->ini->newUndoBlock();}
#else
	#define		MODIFIES_DATA()				{if ( rdPtr->duringCallback ) return 0;rdPtr->ini->newUndoBlock();}
#endif


//============================================================================
//
// CONDITION ROUTINES
// 
// ============================================================================




long WINAPI DLLExport conditionHasUndo(LPRDATA rdPtr, long param1, long param2)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	return rdPtr->ini->hasUndo();
}
long WINAPI DLLExport conditionHasRedo(LPRDATA rdPtr, long param1, long param2)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	return rdPtr->ini->hasRedo();
}


//DONE!
long WINAPI DLLExport conditionHasGroupG(LPRDATA rdPtr, long param1, long param2)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	return rdPtr->ini->HasGroup(rdPtr->currentGroup);
}

//DONE!
long WINAPI DLLExport conditionHasItemG(LPRDATA rdPtr, long param1, long param2)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)param1;
	return rdPtr->ini->HasItem(rdPtr->currentGroup,p1);
}

long WINAPI DLLExport conditionCompareMD5G(LPRDATA rdPtr, long param1, long param2)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	return rdPtr->ini->CompareItemMD5( rdPtr->currentGroup , p1 , p2 , p3 );
}


long WINAPI DLLExport conditionHasGroup(LPRDATA rdPtr, long param1, long param2)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)param1;
	return rdPtr->ini->HasGroup(p1);
}

long WINAPI DLLExport conditionHasItem(LPRDATA rdPtr, long param1, long param2)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)param1;
	char * p2=(LPSTR)param2;
	return rdPtr->ini->HasItem(p1,p2);
}

long WINAPI DLLExport conditionCompareMD5(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	char* p4=(LPSTR)CNC_GetParameter(rdPtr);
	return rdPtr->ini->CompareItemMD5( p1 , p2 , p3 , p4);
}

long WINAPI DLLExport conditionWildcatMatch(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	long p3=CNC_GetIntParameter(rdPtr);
	return rdPtr->ini->WildcatCompare( p1 , p2 , p3 );
}



//============================================================================
//
// ACTIONS ROUTINES
// 
// ============================================================================


short WINAPI DLLExport actionSetCurrentGroup(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)param1;
	long p2 = param2;
	if ( p2 == 0  ||  !rdPtr->ini->HasGroup( p1 ) )
	{
		rdPtr->currentGroup = p1;
	}
	else
	{
		int n = 1;
		while ( rdPtr->ini->HasGroup(  (std::string)p1 + "." + rdPtr->ini->toString(n) ) )
			n++;
		rdPtr->currentGroup = (std::string)p1 + "." + rdPtr->ini->toString(n);
	}
	return 0;
}

short WINAPI DLLExport actionFindSubGroups(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)param1;
	long p2 = param2;
	rdPtr->ini->FindSubgroups( rdPtr->results , p1 , p2 );
	return 0;
}


//DONE		+ tested
short WINAPI DLLExport actionSetValueG(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();

	char* p1=(LPSTR)CNC_GetStringParameter(rdPtr);
	long p2=CNC_GetIntParameter(rdPtr);
	char stringVersion[120]; //Well, we got lots of RAM, right?

	//Integer
	if ( !p2 )
	{
		long p3i = CNC_GetIntParameter(rdPtr);
		_ltoa( p3i , stringVersion , 10 );
	}
	//Float
	else
	{
		long tmp = CNC_GetFloatParameter(rdPtr);
		float p3f=*(float *)&tmp;
		

		FloatToString( stringVersion , p3f );
	}
	rdPtr->ini->SetItem( rdPtr->currentGroup , p1 , stringVersion );
	return 0;
}


//DONE		+ tested
short WINAPI DLLExport actionSetStringG(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char * p1=(LPSTR)param1;
	char * p2=(LPSTR)param2;
	rdPtr->ini->SetItem( rdPtr->currentGroup , p1 , p2 );
	return 0;
}

//DONE		+ tested
short WINAPI DLLExport actionSetStringMD5G(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char * p1=(LPSTR)param1;
	char * p2=(LPSTR)param2;
	rdPtr->ini->SetItemMD5( rdPtr->currentGroup , p1 , p2 );
	return 0;
}




short WINAPI DLLExport actionSaveObjectG(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	LPRO p1=(LPRO)CNC_GetParameter(rdPtr);  // Caution! RunObject structures vary! See Matt's FAQ
	long p2=CNC_GetIntParameter(rdPtr);		// Position and direction
	long p3=CNC_GetIntParameter(rdPtr);		// Alt Values
	iSaveObject( rdPtr, rdPtr->currentGroup , p1 , p2 , p3 );
	return 0;
	
}




short WINAPI DLLExport actionLoadObjectG(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	LPRO p1=(LPRO)param1; // Caution! RunObject structures vary! See Matt's FAQ
	iLoadObject( rdPtr , rdPtr->currentGroup , p1 );
	return 0;
}

//DONE
short WINAPI DLLExport actionSavePositionG(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char * p1=(LPSTR)param1;
	//Put some code for getting input item 1 here!
	short x = HIWORD(param2);
	short y = LOWORD(param2);
	rdPtr->ini->SetItem( rdPtr->currentGroup , p1 , rdPtr->ini->toString(x) + "," + rdPtr->ini->toString(y) );
	return 0;
}

//DONE
short WINAPI DLLExport actionSaveGlobalValuesG(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	SaveGlobals( rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv, rdPtr , rdPtr->currentGroup );
	return 0;
}
//DONE
short WINAPI DLLExport actionLoadGlobalValuesG(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	LoadGlobals( rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv, rdPtr , rdPtr->currentGroup );
	return 0;
}

//DONE		+ tested
short WINAPI DLLExport actionDeleteGroupG(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	rdPtr->ini->DeleteGroup( rdPtr->currentGroup );
	return 0;
}

//DONE		+ tested
short WINAPI DLLExport actionDeleteItemG(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char * p1=(LPSTR)param1;
	rdPtr->ini->DeleteItem( rdPtr->currentGroup , p1 );
	return 0;
}

//DONE + Tested
short WINAPI DLLExport actionRenameGroupG(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char * p1=(LPSTR)param1;
	//param2 contains the number inputed by the user
	if ( rdPtr->ini->RenameGroup( rdPtr->currentGroup , p1 , param2 ) )
	{
		rdPtr->currentGroup = p1;
	}
	return 0;
}

//Done and tested
short WINAPI DLLExport actionRenameItemG(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	long p3=CNC_GetIntParameter(rdPtr);
	rdPtr->ini->RenameItem( rdPtr->currentGroup , p1 , p2 , p3 );
	return 0;
}

//Done
short WINAPI DLLExport actionMoveItemToGroupG(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	long p3=CNC_GetIntParameter(rdPtr);
	rdPtr->ini->MoveItemToGroup( rdPtr->currentGroup , p2 , p1 , p3 );
	return 0;
}

//Done and tested
short WINAPI DLLExport actionSetValue(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	int type = 	CNC_GetIntParameter(rdPtr);

	char stringVersion[120]; //Well, we got lots of RAM, right?

	//Integer
	if ( !type )
	{
		long p3i = CNC_GetIntParameter(rdPtr);
		_ltoa( p3i , stringVersion , 10 );
	}
	//Float
	else
	{
		long tmp = CNC_GetFloatParameter(rdPtr);
		float p3f=*(float *)&tmp;
		FloatToString( stringVersion , p3f );
	}
	rdPtr->ini->SetItem( p1 , p2 , stringVersion );
	return 0;
}

//Done and tested
short WINAPI DLLExport actionSetString(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	rdPtr->ini->SetItem( p1 , p2 , p3 );
	return 0;
}
//Done and tested
short WINAPI DLLExport actionSetStringMD5(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	rdPtr->ini->SetItemMD5( p1 , p2 , p3 );
	return 0;
}

short WINAPI DLLExport actionAddRepeatedItem(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	int	mode = CNC_GetIntParameter(rdPtr);
	//mode 0:	Make sure item is new
	//mode 1:	Make sure group is new

	//In either case, we are done if the 'main' one doesn't exist...
	if (! rdPtr->ini->HasItem( p1 , p2 ) )
		rdPtr->ini->SetItem( p1 , p2 , p3 );
	else
	{
		if ( mode == 0 )
		{
			int n = 1;
			while ( rdPtr->ini->HasItem( p1 , (std::string)p2 + "." + rdPtr->ini->toString(n) ) )
				n++;
			rdPtr->ini->SetItem( p1 ,  (std::string)p2 + "." + rdPtr->ini->toString(n) , p3 );
		}
		else
		{
			int n = 1;
			while ( rdPtr->ini->HasItem(  (std::string)p1 + "." + rdPtr->ini->toString(n) , p2 ) )
				n++;
			rdPtr->ini->SetItem(  (std::string)p1 + "." + rdPtr->ini->toString(n) , p2 , p3 );		
		}
	}
	return 0;
}


//===============
short WINAPI DLLExport actionSaveObject(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	LPRO p1=(LPRO)CNC_GetParameter(rdPtr);  // Caution! RunObject structures vary! See Matt's FAQ
	char* grp = (char*)CNC_GetStringParameter(rdPtr);
	long p2=CNC_GetIntParameter(rdPtr);		// Position and direction
	long p3=CNC_GetIntParameter(rdPtr);		// Alt Values
	iSaveObject( rdPtr, grp , p1 , p2 , p3 );

	return 0;
}

short WINAPI DLLExport actionLoadObject(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	LPRO p1=(LPRO)param1; // Caution! RunObject structures vary! See Matt's FAQ
	char * p2=(LPSTR)param2;

	iLoadObject( rdPtr , p2 , p1 );
	return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//Done and tested
short WINAPI DLLExport actionSavePosition(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	long p3=CNC_GetParameter(rdPtr);


	//Put some code for getting input item 1 here!
	short x = HIWORD(p3);
	short y = LOWORD(p3);
	rdPtr->ini->SetItem( p1 , p2 , rdPtr->ini->toString(x) + "," + rdPtr->ini->toString(y) );
	return 0;
}

//Done and tested
short WINAPI DLLExport actionSaveAltValues(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char * p1=(LPSTR)param1;
	SaveGlobals( rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv, rdPtr , p1 );
	return 0;
}

//Done and tested
short WINAPI DLLExport actionLoadAltValues(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	char * p1=(LPSTR)param1;
	LoadGlobals( rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv, rdPtr , p1 );
	return 0;
}

//Done and tested
short WINAPI DLLExport actionRenameGroup(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	long p3=CNC_GetIntParameter(rdPtr);
	rdPtr->ini->RenameGroup( p1 , p2 , p3 );
	return 0;
}


short WINAPI DLLExport actionRenameItem(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	long p5=CNC_GetIntParameter(rdPtr);
	rdPtr->ini->RenameItem( p1 , p2 , p3 , p5 );
	return 0;
}

//Done and tested
short WINAPI DLLExport actionMoveItem(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	char* p4=(LPSTR)CNC_GetParameter(rdPtr);
	long p5=CNC_GetIntParameter(rdPtr);
	rdPtr->ini->MoveItem( p1 , p3 , p2 , p4 , p5 );
	return 0;
}

//Done and tested
short WINAPI DLLExport actionMoveItemToGroup(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	long p4=CNC_GetIntParameter(rdPtr);
	rdPtr->ini->MoveItemToGroup( p1 , p3 , p2 , p4 );
	return 0;
}
//Done and tested
short WINAPI DLLExport actionCopyGroup(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	long p3=CNC_GetIntParameter(rdPtr);
	rdPtr->ini->CopyGroup( p1 , p2 , p3 );
	return 0;
}
//Done and tested
short WINAPI DLLExport actionCopyItem(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	char* p4=(LPSTR)CNC_GetParameter(rdPtr);
	rdPtr->ini->CopyItem( p1 , p2 , p3 , p4 );
	return 0;
}
//Done and tested
short WINAPI DLLExport actionDeleteGroup(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char * p1=(LPSTR)param1;
	rdPtr->ini->DeleteGroup( p1 );
	return 0;
}
//Done and tested
short WINAPI DLLExport actionDeleteItem(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char * p1=(LPSTR)param1;
	char * p2=(LPSTR)param2;
	rdPtr->ini->DeleteItem( p1 , p2 );
	return 0;
}
//Done and tested
short WINAPI DLLExport actionDeleteItemInAllGroups(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char * p1=(LPSTR)param1;
	rdPtr->ini->DeleteItemFromAllGroups( p1 );
	return 0;
}


short WINAPI DLLExport actionDeleteFromPattern(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	long p4=CNC_GetIntParameter(rdPtr);
	rdPtr->ini->DeleteByPattern( p1 , p2 , p3 , p4 );
	return 0;
}
//Done and tested
short WINAPI DLLExport actionClearAll(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	rdPtr->ini->ClearAll();
	return 0;
}
//Done and tested
short WINAPI DLLExport actionPerformSearch(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	char* p1=(LPSTR)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	paramExt* p4=(paramExt*)CNC_GetParameter(rdPtr);

	rdPtr->ini->PerformSearch( rdPtr->results , p1 , p2 , p3 , p4->pextData[1]  , p4->pextData[0] );

	if ( rdPtr->results->CountItems() == 0 )
		callRunTimeFunction( rdPtr, RFUNCTION_GENERATEEVENT, ON_NOT_SEARCH_FOUND , 0);
	else
		callRunTimeFunction( rdPtr, RFUNCTION_GENERATEEVENT, ON_SEARCH_FOUND , 0);

	return 0;
}


short WINAPI DLLExport actionPerformMultiplesSearch(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	char * p1=(LPSTR)param1;
	char * p2=(LPSTR)param2;
	rdPtr->results->ClearAll();
	rdPtr->ini->PerformMultiplesSearch( rdPtr->results , p1 , p2 );
	return 0;
}

//DONE
short WINAPI DLLExport actionClearResults(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	rdPtr->results->ClearAll();
	return 0;
}
//DONE
short WINAPI DLLExport actionCreateSubINI(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	char* p1=(LPSTR)CNC_GetParameter(rdPtr); //String is path of file
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	char* p4=(LPSTR)CNC_GetParameter(rdPtr);
	int	  p5 = CNC_GetIntParameter(rdPtr);

	SuperINI_SettingsStruct x;
	rdPtr->ini->ExportSettings(&x);

	SuperINI myINI;
	myINI.ImportSettings(&x);
	rdPtr->ini->CreateSubINI( &myINI , p2 , p3 , p4 , p5 );
	myINI.SaveAs( p1 );
	return 0;
}


//==========

short WINAPI DLLExport actionCreateSubINIObject(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	//LPRDATA p1=(LPRDATA)CNC_GetParameter(rdPtr);
	paramExt* p1=(paramExt*)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	char* p4=(LPSTR)CNC_GetParameter(rdPtr);
	int cs = CNC_GetIntParameter(rdPtr);

	LPRDATA  obj = FINDOBJECTNOTTHIS( p1->pextData , "Error: The INI++ object you have selected for use with either the 'Merge' or 'Search' action does not exist. The action will fail.\n\nThis error message will not be shown when the program is built." );

	if (obj != NULL )
		rdPtr->ini->CreateSubINI( obj->ini , p2 , p3 , p4 , cs );
	return 0;
}


// ~~~~~~~~~~~~~~~~~~


//DONE and TESTED
short WINAPI DLLExport actionMergeFile(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char * p1=(LPSTR)param1;  //String is path of file
	//param2 contains the number inputed by the user

	SuperINI_SettingsStruct x;
	rdPtr->ini->ExportSettings(&x);

	SuperINI ini;
	ini.ImportSettings(&x);

	if ( ini.Load( FormatPathReadOnly(p1) ) )
	{
		rdPtr->ini->Merge( &ini , param2 );
		callRunTimeFunction( rdPtr, RFUNCTION_GENERATEEVENT, ON_MERGE , 0);
	}
	return 0;
}

short WINAPI DLLExport actionMergeGroupFile(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char* p1=(LPSTR)CNC_GetParameter(rdPtr); //String is path of file
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	long p4=CNC_GetIntParameter(rdPtr);

	SuperINI_SettingsStruct x;
	rdPtr->ini->ExportSettings(&x);

	SuperINI ini;
	ini.ImportSettings(&x);

	if ( ini.Load( FormatPathReadOnly(p1) ) )
	{
		rdPtr->ini->MergeGroup( &ini , p2 , p3 , p4 );
		callRunTimeFunction( rdPtr, RFUNCTION_GENERATEEVENT, ON_MERGE , 0);
	}
	return 0;
}

short WINAPI DLLExport actionMerge(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	paramExt* p1=(paramExt*)CNC_GetParameter(rdPtr);

	//param2 contains the number inputed by the user

	LPRDATA  obj = FINDOBJECTNOTTHIS( p1->pextData , "Error: The INI++ object you have selected for use with either the 'Merge' or 'Search' action does not exist. The action will fail.\n\nThis error message will not be shown when the program is built." );

	if (obj != NULL )
	{
		rdPtr->ini->Merge( obj->ini , param2 );
		callRunTimeFunction( rdPtr, RFUNCTION_GENERATEEVENT, ON_MERGE , 0);
	}

	return 0;
}

short WINAPI DLLExport actionMergeGroupObject(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	paramExt* p1=(paramExt*)CNC_GetParameter(rdPtr);
	char* p2=(LPSTR)CNC_GetParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	long p4=CNC_GetIntParameter(rdPtr);

	LPRDATA  obj = FINDOBJECTNOTTHIS( p1->pextData , "Error: The INI++ object you have selected for use with either the 'Merge' or 'Search' action does not exist. The action will fail.\n\nThis error message will not be shown when the program is built." );

	if (obj != NULL )
	{
		rdPtr->ini->MergeGroup( obj->ini , p2 , p3 , p4 );
		callRunTimeFunction( rdPtr, RFUNCTION_GENERATEEVENT, ON_MERGE , 0);
	}
	return 0;
}

short WINAPI DLLExport actionNew(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char * p1=(LPSTR)param1;  //String is path of file
	//param2 contains should clear or not

	rdPtr->ini->New(FormatPath(p1),param2);
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}

short WINAPI DLLExport actionLoad(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char * p1=(LPSTR)param1;  //String is path of file
	//param2 is true if it is read-only.
	if ( param2 )
	{
		rdPtr->ini->SetReadOnly(true);
		rdPtr->ini->Load( FormatPathReadOnly(p1) );
	}
	else
	{
		rdPtr->ini->SetReadOnly(false);
		rdPtr->ini->Load( FormatPath(p1) );
	}
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}

short WINAPI DLLExport actionSave(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	rdPtr->ini->Save();
	return 0;
}

short WINAPI DLLExport actionSaveAs(LPRDATA rdPtr, long param1, long param2){
	
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)param1;  //String is path of file
	rdPtr->ini->SaveAs(FormatPath(p1));
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}

short WINAPI DLLExport actionBackupTo(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	char* p1=(LPSTR)CNC_GetParameter(rdPtr); //String is path of file
	long p2=CNC_GetIntParameter(rdPtr);
	char* p3=(LPSTR)CNC_GetParameter(rdPtr);
	rdPtr->ini->SaveToFile( FormatPath(p1) , p2 , (std::string(p3)!="") , p3 );
	return 0;
}

short WINAPI DLLExport actionClose(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	rdPtr->ini->Close();
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}

short WINAPI DLLExport actionLoadFromString(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	char * p1=(LPSTR)param1;
	rdPtr->ini->LoadStream( p1 , !param2 , !param2 );
	return 0;
}

short WINAPI DLLExport actionSetAutoSave(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	//param1 contains the number inputed by the user
	rdPtr->ini->SetAutoSave( param1 );
	rdPtr->autoLoad = param2;
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}

short WINAPI DLLExport actionSetCompresson(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	//param1 contains the number inputed by the user
	rdPtr->ini->SetAutosaveProperties( param1 , -1 , "" );
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}

short WINAPI DLLExport actionSetEncryption(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	char * p1=(LPSTR)param1;
	rdPtr->ini->SetAutosaveProperties( -1 , (strcmp(p1,"") != 0) , p1 );
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}

short WINAPI DLLExport actionSetReadOnly(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	rdPtr->ini->SetReadOnly( param1 );
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}

short WINAPI DLLExport actionSetCaseSensitive(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();
	MODIFIES_DATA();
	//param1 contains the number inputed by the user
	rdPtr->ini->SetCaseSensitivity( param1 );
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}

short WINAPI DLLExport actionSetEscapeChars(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	long p1=CNC_GetIntParameter(rdPtr);
	long p2=CNC_GetIntParameter(rdPtr);
	long p3=CNC_GetIntParameter(rdPtr);
	rdPtr->ini->EscapeCharacters(p1,p2,p3);
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}

short WINAPI DLLExport actionNeverQuoteStrings(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	//param1 contains the number inputed by the user
	rdPtr->ini->SetNeverQuoteStrings(param1);
	GLOBAL_SETTINGS_CHANGE();
	
	return 0;
}

short WINAPI DLLExport actionSetRepeatModes(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	param1=CNC_GetIntParameter(rdPtr);
	param2=CNC_GetIntParameter(rdPtr);
	long param3=CNC_GetIntParameter(rdPtr);
	//param1 contains the number inputed by the user
	//param2 contains the number inputed by the user
	rdPtr->ini->SetRepeatModes( param1 , param2 , param3 );
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}

//Done and tested
short WINAPI DLLExport actionSetNewLineChar(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	char * p1=(LPSTR)param1;
	rdPtr->ini->SetNewLineCharacter( p1 );
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}

short WINAPI DLLExport actionSetDefaultDirectory(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	char * p1=(LPSTR)param1;
	PathRemoveFileSpec(p1);
	rdPtr->defaultDirectory = p1;
	return 0;
}

short WINAPI DLLExport actionCompressFile(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	char * p1=(LPSTR)param1;  //String is path of file
	char * p2=(LPSTR)param2;  //String is path of file
	rdPtr->ini->CompressTo( FormatPathReadOnly(p1) , FormatPath(p2) );
	return 0;
}

short WINAPI DLLExport actionDeCompressFile(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	char * p1=(LPSTR)param1;  //String is path of file
	char * p2=(LPSTR)param2;  //String is path of file
	
	(rdPtr)->duringCallback = 1;
	if ( !rdPtr->ini->UncompressTo( FormatPathReadOnly(p1) , FormatPath(p2) ) )	
		callRunTimeFunction( (rdPtr), RFUNCTION_GENERATEEVENT, ON_UNCOMPRESS_FAIL , 0);
	else
		callRunTimeFunction( (rdPtr), RFUNCTION_GENERATEEVENT, ON_UNCOMPRESS_OK , 0);
	(rdPtr)->duringCallback = 0;
	return 0;
}

#ifdef INCLUDE_DIALOG
DWORD WINAPI DoBox( LPVOID rdPtr )
{
	HWND hWnd = NULL; //((RunHeader*)(((LPRDATA)rdPtr)->rHo.hoAdRunHeader))->rhHMainWin;
	
	callRunTimeFunction( ((LPRDATA)rdPtr), RFUNCTION_GENERATEEVENT, DIALOGBOX_OPEN , 0);
	DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_DEBUG2), hWnd, debugboxProc2, (LPARAM)rdPtr);
	callRunTimeFunction( ((LPRDATA)rdPtr), RFUNCTION_GENERATEEVENT, DIALOGBOX_CLOSE , 0);
	return 0;
}
#endif


short WINAPI DLLExport actionOpenDialog(LPRDATA rdPtr, long param1, long param2){GLOBAL_SETTINGS_INTO_MEMORY();

	#ifdef		INCLUDE_DIALOG
	char* p1		= (char*)CNC_GetStringParameter(rdPtr);
	paramExt*  p2	= (paramExt*)CNC_GetParameter(rdPtr);

	int settings = *(int*)p2->pextData;

	rdPtr->dialogTitle = p1;
	rdPtr->dialogSettings = LOWORD(settings);
	rdPtr->dialogDisplay = HIWORD(settings);

	if ( ! (settings & PAUSE_MMF) )
	{
		unsigned long id;
		CreateThread( NULL , 0 , DoBox , (void*)rdPtr , 0 , &id );
	}
	else
	{
		callRunTimeFunction(rdPtr, RFUNCTION_PAUSE, 0,0);
		DoBox(rdPtr);
		callRunTimeFunction(rdPtr, RFUNCTION_CONTINUE , 0,0);
	}
	#endif
	return 0;
}

short WINAPI DLLExport actionCloseDialog(LPRDATA rdPtr, long param1, long param2)
{
	if ( rdPtr->dialogCurrent != 0 )
		SendMessage( rdPtr->dialogCurrent , WM_COMMAND , IDCANCEL , 0 );
	return 0;
}
short WINAPI DLLExport actionRefreshDialog(LPRDATA rdPtr, long param1, long param2)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	if ( rdPtr->dialogCurrent != 0 )
	{
		if ( param1 == 0 )
			SendMessage( rdPtr->dialogCurrent , WM_COMMAND , 10 , 0 );
		else
			SendMessage( rdPtr->dialogCurrent , WM_COMMAND , 14 , 0 );

	}
	return 0;
}


//============================================================================
//
// EXPRESSIONS ROUTINES
// 
// ============================================================================


//Done + tested
long WINAPI DLLExport expressionGetItemValueG(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	long p2=CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_FLOAT);
	
	if ( !rdPtr->ini->HasItem( rdPtr->currentGroup , p1 ) )
	{
		//float f2 = *(float*)&p2;
		rdPtr->rHo.hoFlags |= HOF_FLOAT;
		return p2;
	}
	std::string value = rdPtr->ini->GetItem( rdPtr->currentGroup , p1 , "" );
	if ( value.find(".") == -1 )
	{
		long answer = atoi( value.c_str() );
		return answer;
	}
	else
	{
		rdPtr->rHo.hoFlags |= HOF_FLOAT;
		float answer = StringToFloat(value.c_str());
		long retVal = 0;
		memcpy( (void*)&retVal , (void*)&answer , sizeof(float) );
		return retVal;
	}
}

//DONE + tested
long WINAPI DLLExport expressionGetItemStringG(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	char * p2=(LPSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);
	rdPtr->rHo.hoFlags |= HOF_STRING;

	std::string value = rdPtr->ini->GetItem( rdPtr->currentGroup , p1 , p2 );

	LPSTR x = (LPSTR)callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE, value.length()+1 , 0);
	lstrcpyn( x , value.c_str() , value.length()+1 );
	return (long)x;
}

//Done
long WINAPI DLLExport expressionGetXG(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	std::string value = rdPtr->ini->GetItem( rdPtr->currentGroup , p1 , "0,0" );
	value = value.substr( 0 , value.find(",") );
	return atoi(value.c_str());
}

long WINAPI DLLExport expressionGetYG(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	std::string value = rdPtr->ini->GetItem( rdPtr->currentGroup , p1 , "0,0" );
	value = value.substr( value.find(",") + 1 );
	return atoi(value.c_str());
}

long WINAPI DLLExport expressionNthItemG(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	long p1=CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);
	ADJUST_FOR_INDEX(p1);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	std::string ans = rdPtr->ini->GetNthItemName( rdPtr->currentGroup , p1 , "" );
	RETURN_STDSTRING( ans );
}

long WINAPI DLLExport expressionNthItemValueG(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	long p1=CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);
	ADJUST_FOR_INDEX(p1);
	std::string value = rdPtr->ini->GetNthItemValue( rdPtr->currentGroup , p1 , "" );
	if ( value.find(".") == -1 )
	{
		long answer = atoi( value.c_str() );
		return answer;
	}
	else
	{
		rdPtr->rHo.hoFlags |= HOF_FLOAT;
		float answer = StringToFloat(value.c_str());
		long retVal = 0;
		memcpy( (void*)&retVal , (void*)&answer , sizeof(float) );
		return retVal;
	}	
}

long WINAPI DLLExport expressionNthItemStringG(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	long p1=CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);
	ADJUST_FOR_INDEX(p1);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	std::string ans = rdPtr->ini->GetNthItemValue( rdPtr->currentGroup , p1 , "" );
	RETURN_STDSTRING( ans );
}

long WINAPI DLLExport expressionItemCountG(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	return rdPtr->ini->ItemCount(rdPtr->currentGroup);
}

long WINAPI DLLExport expressionGetItemValue(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	char * p2=(LPSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);
	long p3=CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_FLOAT);

	
	if ( !rdPtr->ini->HasItem( p1 , p2 ) )
	{
		rdPtr->rHo.hoFlags |= HOF_FLOAT;
		return p3;
	}
	std::string value = rdPtr->ini->GetItem( p1 , p2 , "" );
	if ( value.find(".") == -1 )
	{
		long answer = atoi( value.c_str() );
		return answer;
	}
	else
	{
		rdPtr->rHo.hoFlags |= HOF_FLOAT;
		float answer = StringToFloat(value.c_str());
		long retVal = 0;
		memcpy( (void*)&retVal , (void*)&answer , sizeof(float) );
		return retVal;
	}
	return 0;
}

long WINAPI DLLExport expressionGetItemString(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	char * p2=(LPSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);
	char * p3=(LPSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);
	rdPtr->rHo.hoFlags |= HOF_STRING;

	std::string value = rdPtr->ini->GetItem( p1 , p2 , p3 );

	LPSTR x = (LPSTR)callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE, value.length()+1 , 0);
	lstrcpyn( x , value.c_str() , value.length()+1 );
	x[value.length()] = 0;

	return (long)x;
}






long WINAPI DLLExport expressionGetX(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	char * p2=(LPSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);
	std::string value = rdPtr->ini->GetItem( p1 , p2 , "0,0" );
	value = value.substr( 0 , value.find(",") );
	return atoi(value.c_str());	
}

long WINAPI DLLExport expressionGetY(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	char * p2=(LPSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);

	std::string value = rdPtr->ini->GetItem( p1 , p2 , "0,0" );
	value = value.substr( value.find(",") + 1 );
	return atoi(value.c_str());	
}

long WINAPI DLLExport expressionNthGroup(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	long p1=CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	ADJUST_FOR_INDEX(p1);
	std::string name = rdPtr->ini->GetNthGroupName( p1 , "" );
	RETURN_STDSTRING( name );	
	return 0;
}

long WINAPI DLLExport expressionNthItem(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	long p2=CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_INT);

	ADJUST_FOR_INDEX(p2);

	rdPtr->rHo.hoFlags |= HOF_STRING;
	std::string name = rdPtr->ini->GetNthItemName( p1 , p2 , "" );
	RETURN_STDSTRING( name );	
	return 0;
}

long WINAPI DLLExport expressionNthItemString(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	long p2=CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_INT);
	ADJUST_FOR_INDEX(p2);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	std::string name = rdPtr->ini->GetNthItemValue( p1 , p2 , "" );
	RETURN_STDSTRING( name );	
	return 0;
}

long WINAPI DLLExport expressionNthItemValue(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	long p2=CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_INT);
	ADJUST_FOR_INDEX(p2);
	//rdPtr->rHo.hoFlags |= HOF_STRING;		// Yves fix: function doesn't return a string
	std::string value = rdPtr->ini->GetNthItemValue( p1 , p2 , "" );
	
	if ( value.find(".") == -1 )
	{
		long answer = atoi( value.c_str() );
		return answer;
	}
	else
	{
		rdPtr->rHo.hoFlags |= HOF_FLOAT;
		float answer = StringToFloat(value.c_str());
		long retVal = 0;
		memcpy( (void*)&retVal , (void*)&answer , sizeof(float) );
		return retVal;
	}	
	return 0;
}

long WINAPI DLLExport expressionGroupCount(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	return rdPtr->ini->GroupCount();
}

long WINAPI DLLExport expressionItemCount(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	return rdPtr->ini->ItemCount( p1 );
	return 0;
}

//Function changed back
long WINAPI DLLExport expressionTotalItems(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	return rdPtr->ini->TotalItemCount();
}

long WINAPI DLLExport expressionSearchResultCounts(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	return rdPtr->results->CountItems();
}

//DONE AND TESTED
long WINAPI DLLExport expressionGetNthSearchResultGroup(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	long p1=CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);
	ADJUST_FOR_INDEX(p1);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	RETURN_STDSTRING( rdPtr->results->GetItem(p1-1,0) );
	return 0;
}
//DONE AND TESTED
long WINAPI DLLExport expressionGetNthSearchResultItem(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	long p1=CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);
	ADJUST_FOR_INDEX(p1);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	RETURN_STDSTRING( rdPtr->results->GetItem(p1-1,1) );
	return 0;
}

//DONE
long WINAPI DLLExport expressionGetNthSearchResultItemValue(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	long p1=CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);
	ADJUST_FOR_INDEX(p1);
	std::string value = rdPtr->results->GetItem( p1-1 , 2 );

	if ( value.find(".") == -1 )
	{
		long answer = atoi( value.c_str() );
		return answer;
	}
	else
	{
		rdPtr->rHo.hoFlags |= HOF_FLOAT;
		float answer = StringToFloat(value.c_str());
		long retVal = 0;
		memcpy( (void*)&retVal , (void*)&answer , sizeof(float) );
		return retVal;
	}
}

//DONE AND TESTED
long WINAPI DLLExport expressionGetNthSearchResultItemString(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	long p1=CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);
	ADJUST_FOR_INDEX(p1);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	RETURN_STDSTRING( rdPtr->results->GetItem(p1-1,2) );
	return 0;
}

//DONE
long WINAPI DLLExport expressionGetNthSearchResultPath(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	long p1=CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_INT);
	ADJUST_FOR_INDEX(p1);
	char * p2=(LPSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	std::string sep,path;
	sep = std::string(p2);
	path = (rdPtr->results->GetItem(p1-1,0) + sep + rdPtr->results->GetItem(p1-1,1));
	RETURN_STDSTRING( path );
	return 0;
}


//DONE and tested
long WINAPI DLLExport expressionWriteStream(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	rdPtr->rHo.hoFlags |= HOF_STRING;

	std::string value = rdPtr->ini->WriteStream();

	LPSTR x = (LPSTR)callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE, value.length()+1 , 0);
	lstrcpyn( x , value.c_str() , value.length()+1 );

	return (long)x;
}

long WINAPI DLLExport expressionHashString(LPRDATA rdPtr, long param1)
{
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	std::string val = rdPtr->ini->HashMD5( p1 );
	RETURN_STDSTRING( val );
}

long WINAPI DLLExport expressionEncryptString(LPRDATA rdPtr, long param1)
{
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	char * p2=(LPSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	std::string val = rdPtr->ini->ToggleEncryption( p1 , p2 );
	RETURN_STDSTRING( val );
}

long WINAPI DLLExport expressionEscapeString(LPRDATA rdPtr, long param1)
{
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	std::string val = rdPtr->ini->EscapeCharacters( p1 );
	RETURN_STDSTRING( val );
}

long WINAPI DLLExport expressionUnEscapeString(LPRDATA rdPtr, long param1)
{
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	std::string val = rdPtr->ini->UnEscapeCharacters( p1 );
	RETURN_STDSTRING( val );
}


const char* Operator_Names[] = {"+","*","~","avg","\\","min","/","max","iavg","~~","#"};

bool IsGroupOrItemName( char* data , bool allowN )
{
	if ( data[0] == '%' )
		return true;
	
	for ( int n = 0 ; n < sizeof(Operator_Names)/sizeof(char*) ; n++ )
		if ( _stricmp(data,Operator_Names[n]) == 0 )
			return false;

	if ( _stricmp(data,"n") == 0 )
		return false;

	return true;
}

float DoGroup( LPRDATA rdPtr , std::string grpName , char* p2 )
{
	float itemTotal = 0;

	if ( _stricmp(p2,"*") == 0 )
		itemTotal = 1;

	//Find the item total
	if ( _stricmp(p2,"n") == 0 )
		itemTotal = rdPtr->ini->ItemCount(grpName);
	else
		for ( int itm = 1 ; itm <= rdPtr->ini->ItemCount(grpName) ; itm++ )
		{
			float val = StringToFloat(rdPtr->ini->GetNthItemValue(grpName,itm,"").c_str());
			if ( strcmp(p2,"*") == 0 )
				itemTotal *= val;
			else if ( _stricmp(p2,"max") == 0 || _stricmp(p2,"/") == 0){
				if ( val > itemTotal || itm == 1 ) itemTotal = val;}
			else if ( _stricmp(p2,"min") == 0 || _stricmp(p2,"\\") == 0){
				if ( val < itemTotal || itm == 1 ) itemTotal = val;}
			else
				itemTotal += val;
		}
	if( _stricmp(p2,"~") == 0 || _stricmp(p2,"avg") == 0)
		itemTotal = itemTotal/((float)rdPtr->ini->ItemCount(grpName));

	return itemTotal;
}

long WINAPI DLLExport expressionInnerProduct(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	//Group combo
	char * p2=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	//Overall combo
	char * p1=(LPSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);

	bool p2_name = IsGroupOrItemName( p2 , true  );
	bool p1_name = IsGroupOrItemName( p1 , false );

	std::string p2s = p2;
	if(p2s[0]=='%')p2s = p2s.substr(1);

	float runningTotal = 0;

	if ( _stricmp(p1,"*") == 0 )
			runningTotal = 1;

	if ( !p1_name ) //p1 is an operator
	{
		int usedGroups = 0;
		for ( int grp = 1 ; grp <= rdPtr->ini->GroupCount() ; grp++ )
		{
			std::string grpName = rdPtr->ini->GetNthGroupName(grp,"");
			//Check if p2 is just a group name. If so, only look at that one.
			float itemTotal = 0;
			if ( !p2_name)
				itemTotal = DoGroup( rdPtr , grpName , p2 );
			else
			{
				if (! rdPtr->ini->HasItem( grpName , p2s ) )
					goto BottomOfLoop;	//I'M SO SORRY! But it is the most optimal code.
				itemTotal = StringToFloat( rdPtr->ini->GetItem(grpName , p2s , "" ).c_str() );
			}
			usedGroups++;

			if ( _stricmp(p1,"*") == 0 )
				runningTotal *= itemTotal;
			else if ( _stricmp(p1,"max") == 0 || _stricmp(p1,"/") == 0){
				if ( itemTotal > runningTotal || grp == 1 ) runningTotal = itemTotal;}
			else if ( _stricmp(p1,"min") == 0 || _stricmp(p1,"\\") == 0){
				if ( itemTotal < runningTotal || grp == 1 ) runningTotal = itemTotal;}
			else
				runningTotal += itemTotal;
			BottomOfLoop:;		//I know, I know.
		}
		if( _stricmp(p1,"~") == 0 || _stricmp(p1,"avg") == 0)
			runningTotal = runningTotal/((float)usedGroups);
		if( _stricmp(p1,"iavg") == 0 || _stricmp(p1,"~~")  == 0 || _stricmp(p1,"#") == 0)
			runningTotal = runningTotal/((float)rdPtr->ini->TotalItemCount());
	}
	else
	{
		std::string str = p1;
		if(str[0]=='%')str = str.substr(1);

		if ( !p2_name)
			runningTotal = DoGroup( rdPtr , str , p2 );
		else
			runningTotal = StringToFloat( rdPtr->ini->GetItem(str , p2s , "" ).c_str() );
	}

	long runningTotalRet;
	memcpy( (void*)&runningTotalRet,(void*)&runningTotal,sizeof(float));
	rdPtr->rHo.hoFlags |= HOF_FLOAT;
	return runningTotalRet;

}


long WINAPI DLLExport expressionInnerProductStr(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	//Group combo
	char * p2=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	//Overall combo
	char * p1=(LPSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);

	bool p2_name = IsGroupOrItemName( p2 , true  );
	std::string p2s = p2;
	if(p2s[0]=='%')p2s = p2s.substr(1);

	#ifndef RUN_ONLY
	if( _stricmp(p1,"min") != 0 && _stricmp(p1,"max") != 0 && _stricmp(p1,"\\") != 0 && _stricmp(p1,"/") != 0 )
		MessageBox(NULL,"Error: The 'Max or Min Group' (StrCalc$(...)) expression required the first parameter to be either 'max', '/', 'min' or '\\'.\n\nThis message will not be shown when the program is built.","Error in Ini++",0);
	#endif

	float currentBest = 0;
	std::string currentRet = "";

	//p1 is ALWAYS an operator
	int usedGroups = 0;
	for ( int grp = 1 ; grp <= rdPtr->ini->GroupCount() ; grp++ )
	{
		std::string grpName = rdPtr->ini->GetNthGroupName(grp,"");
		//Check if p2 is just a group name. If so, only look at that one.
		float itemTotal = 0;
		if ( !p2_name)
			itemTotal = DoGroup( rdPtr , grpName , p2 );
		else
		{
			if (! rdPtr->ini->HasItem( grpName , p2s ) )
				goto BottomOfLoop;	//I'M SO SORRY! But it is the most optimal code.
			itemTotal = StringToFloat( rdPtr->ini->GetItem(grpName , p2s , "" ).c_str() );
		}
		usedGroups++;

		if ( _stricmp(p1,"max") == 0 || _stricmp(p1,"/") == 0)
		{
			if ( itemTotal > currentBest || grp == 1 )
			{
				currentBest = itemTotal;
				currentRet = grpName;

			}
		}
		else
		{
			if ( itemTotal < currentBest || grp == 1 )
			{
				currentBest = itemTotal;
				currentRet = grpName;

			}
		}

		BottomOfLoop:;		//I know, I know.
	}
	rdPtr->rHo.hoFlags |= HOF_STRING;
	RETURN_STDSTRING( currentRet );
}

long WINAPI DLLExport expressionNthSortedName(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	long p2=CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_INT);

	ADJUST_FOR_INDEX(p2);

	ItemPairHolder sorted = SortGroup(rdPtr,p1,0,2);
	std::string out = "";
	if ( p2 <= sorted.size() && p2 >= 1)
		out = sorted[p2-1].itemName;
	rdPtr->rHo.hoFlags |= HOF_STRING;
	RETURN_STDSTRING( out );

}
long WINAPI DLLExport expressionNthSortedValue(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	long p2=CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_INT);

	ADJUST_FOR_INDEX(p2);

	ItemPairHolder sorted = SortGroup(rdPtr,p1,0,2);
	float out = 0;
	if ( p2 <= sorted.size() && p2 >= 1)
		out = StringToFloat( sorted[p2-1].itemValue.c_str() );
	long outRet;
	memcpy( (void*)&outRet,(void*)&out,sizeof(float));
	rdPtr->rHo.hoFlags |= HOF_FLOAT;
	return outRet;
}
long WINAPI DLLExport expressionGetCSV(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	char * p1=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	char * p2=(LPSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);

	std::string str = rdPtr->ini->WriteCSV(p1,p2);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	RETURN_STDSTRING( str );
}

// ----------------------------------------------------------
// Condition / Action / Expression jump table
// ----------------------------------------------------------
// Contains the address inside the extension of the different
// routines that handle the action, conditions and expressions.
// Located at the end of the source for convinience
// Must finish with a 0
//


long WINAPI DLLExport conditionEvent(LPRDATA rdPtr, long param1, long param2)
{
	return TRUE;
}

short WINAPI DLLExport actionExportCVS(LPRDATA rdPtr, long param1, long param2){
	//TOOD: CSV Array
	GLOBAL_SETTINGS_INTO_MEMORY();

	int mode = CNC_GetParameter(rdPtr);
	char* p1=(LPSTR)CNC_GetParameter(rdPtr); //String is path of file
	char* p2=(LPSTR)CNC_GetParameter(rdPtr); //Group
	char* p3=(LPSTR)CNC_GetParameter(rdPtr); //seperator.

	if ( mode == 0 )
	{
		MODIFIES_DATA();
		rdPtr->ini->ImportCSV(FormatPathReadOnly(p1),p2,p3);
	}
	if ( mode == 1 )
	{
		rdPtr->ini->ExportAsCSV(FormatPath(p1),p2,p3);
	}
	return 0;
}

short WINAPI DLLExport actionImportCVS(LPRDATA rdPtr, long param1, long param2){
	GLOBAL_SETTINGS_INTO_MEMORY();
	int mode = CNC_GetParameter(rdPtr);
	char* p1=(LPSTR)CNC_GetParameter(rdPtr); //String is path of file
	char* p3=(LPSTR)CNC_GetParameter(rdPtr); //seperator.
	if ( mode == 0 )
	{
		MODIFIES_DATA();
		rdPtr->ini->ImportCSVArray(FormatPathReadOnly(p1),p3);
	}
	if ( mode == 1 )
	{
		rdPtr->ini->ExportCSVArray(FormatPath(p1),p3);
	}
	return 0;
}


short WINAPI DLLExport actionSetItemArray(LPRDATA rdPtr, long param1, long param2)
{
	MODIFIES_DATA();

	char* grp = (LPSTR)CNC_GetParameter(rdPtr);
	char* itm=(LPSTR)CNC_GetParameter(rdPtr);
	long part =CNC_GetIntParameter(rdPtr);
	char* value =(LPSTR)CNC_GetParameter(rdPtr); //seperator.

	ADJUST_FOR_INDEX(part);

	rdPtr->ini->SetItemArray(grp,itm,part-1,value);
	return 0;
}

short WINAPI DLLExport actionLoadFromArray(LPRDATA rdPtr, long param1, long param2)
{
	MODIFIES_DATA();

	paramExt* p1=(paramExt*)CNC_GetParameter(rdPtr);
	LPSTR p2 = (LPSTR)CNC_GetStringParameter(rdPtr);

	MMFArray* arr = FindArrayObject( p1->pextData , rdPtr );

	if ( arr != 0 )
	{
		for ( int x = 0 ; x < arr->GetW() ; x++ )
			for ( int y = 0 ; y < arr->GetH() ; y++ )
				for ( int z = 0 ; z < arr->GetD() ; z++ )
				{				
					if ( arr->Has(x,y,z) )
						rdPtr->ini->SetItem(p2,arr->CoordinatesToText(x,y,z) , arr->Get(x,y,z)  );			
				}
	}
	return 0;
}

bool IsNumberString(std::string& x )
{
	if ( x == "" ) return false;
	for ( int n = 0 ; n < x.length() ; n++ )
	{
		if ( !isdigit(x[n]) )
			return false;

	}
	return true;
}

short WINAPI DLLExport actionSaveToArray(LPRDATA rdPtr, long param1, long param2)
{
	MODIFIES_DATA();

	paramExt* p1=(paramExt*)CNC_GetParameter(rdPtr);
	LPSTR p2 = (LPSTR)CNC_GetStringParameter(rdPtr);

	MMFArray* mmf_arr = FindArrayObject( p1->pextData , rdPtr );
	
	if ( mmf_arr != 0 )
	{
		if ( mmf_arr->IsText() )
		{
			#if !defined(RUN_ONLY)
				MessageBox( NULL , "Error: INI++ cannot put data into a string array for technical reasons. It can, however, read from string arrays and write to numerical arrays.\n\nThis error message will not be shown when the program is built." , "INI++ Error" , 0 );
			#endif // !defined(RUN_ONLY)
		}
		else
		{
			for ( int n = 1 ; n <= rdPtr->ini->ItemCount(p2) ; n++ )
			{
				std::string itemName = rdPtr->ini->GetNthItemName(p2,n,"");
				DynamicArray arr = SplitByComma( itemName );
				if ( arr.size() != 3 || !IsNumberString(arr[0]) || !IsNumberString(arr[1]) || !IsNumberString(arr[2]) )
					continue;
				int x = atoi(arr[0].c_str());
				int y = atoi(arr[1].c_str());
				int z = atoi(arr[2].c_str());
				mmf_arr->Set(x,y,z,rdPtr->ini->GetItem(p2,itemName,""));
			}
		}
	}
	return 0;
}






short WINAPI DLLExport actionEnableSubGroups(LPRDATA rdPtr, long param1, long param2){	
	MODIFIES_DATA();
	rdPtr->ini->SetSubGroups(param1,param2);
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}





short WINAPI DLLExport actionSwitchGlobalObject(LPRDATA rdPtr, long param1, long param2){

	GLOBAL_SETTINGS_INTO_MEMORY();

	char* p1=(LPSTR)CNC_GetParameter(rdPtr);

	if ( rdPtr->global == NULL )
	{
		rdPtr->global = new SuperINI_SettingsStruct();
		rdPtr->ini->ExportSettings(rdPtr->global);
		delete rdPtr->ini;
	}

	LPRH rhPtr;
	rhPtr = rdPtr->rHo.hoAdRunHeader;
	CRunApp* pApp = rhPtr->rhApp;
	
	//Get the parent app.
	while (pApp->m_pParentApp != NULL)
		pApp = pApp->m_pParentApp;

	mv* mV = rhPtr->rh4.rh4Mv;
	
	GlobalObjectManager* globalData = (GlobalObjectManager*)mV->mvGetExtUserData(pApp, hInstLib);
	if ( globalData == NULL )
	{
		globalData = new GlobalObjectManager();
		mV->mvSetExtUserData(pApp, hInstLib,(void*)globalData);

	}
	rdPtr->ini = globalData->get(p1);
	return 0;
}

short WINAPI DLLExport actionSSS(LPRDATA rdPtr, long param1, long param2){

	MODIFIES_DATA();

	paramExt* p1=(paramExt*)param1;
	char* p2 = (char*)param2;

	//param1: action
	//param2: group

	rdPtr->ini->PauseAutoSave(true);
	//actions:		SORT*,SHUFFLE*,SHIFT-UP,SHIFT-DOWN,CYCLE-UP,CYCLE-DOWN, REVERSE
	switch ( p1->pextData[0] )
	{
	case 0:	//Sort
		{
			ItemPairHolder sorted = SortGroup(rdPtr,p2,0,p1->pextData[1]);
			rdPtr->ini->DeleteGroup(p2);
			for ( int n = 0 ; n < sorted.size() ; n++ )
			{
				rdPtr->ini->SetItem( p2 , sorted[n].itemName , sorted[n].itemValue );
			}
			break;
		}
	case 9:	//Sort
		{
			ItemPairHolder sorted = SortGroup(rdPtr,p2,1,p1->pextData[1]);
			rdPtr->ini->DeleteGroup(p2);
			for ( int n = 0 ; n < sorted.size() ; n++ )
			{
				rdPtr->ini->SetItem( p2 , sorted[n].itemName , sorted[n].itemValue );
			}
			break;
		}

	case 10: //Sort by first item
		{
			ItemPairHolder sorted = SortGroup(rdPtr,p2,2,p1->pextData[1]);
			rdPtr->ini->DeleteGroup(p2);
			for ( int n = 0 ; n < sorted.size() ; n++ )
			{
				rdPtr->ini->SetItem( p2 , sorted[n].itemName , sorted[n].itemValue );
			}
			break;
		}

	case 1: //Shuffle
			srand( GetTickCount() );
			rdPtr->ini->GroupEffect(p2,1);
			break;
	case 2:	//Shift up (i.e. delete top item)
			rdPtr->ini->DeleteItem(p2,rdPtr->ini->GetNthItemName(p2,1,""));
			break;
	case 4:	//Shift down (i.e. delete the bottom item)
			rdPtr->ini->DeleteItem(p2,rdPtr->ini->GetNthItemName(p2,rdPtr->ini->ItemCount(p2),""));
			break;
	case 3: //Cycle up
			rdPtr->ini->GroupEffect(p2,2);
			break;
	case 5: //Cycle down
			rdPtr->ini->GroupEffect(p2,3);
			break;
	case 6:	//reverse
		rdPtr->ini->GroupEffect(p2,0);
		break;

	case 7: //swap
		{
			ItemPairHolder unsorted = GetGroup(rdPtr,p2);
			rdPtr->ini->DeleteGroup(p2);
			for ( int n = 0 ; n < unsorted.size() ; n++ )
					rdPtr->ini->SetItem( p2 , unsorted[n].itemValue , unsorted[n].itemName );

			break;
		}

	case 8:	//re-number
		{
			ItemPairHolder unsorted = GetGroup(rdPtr,p2);
			rdPtr->ini->DeleteGroup(p2);
			for ( int n = 0 ; n < unsorted.size() ; n++ )
					rdPtr->ini->SetItem( p2 , rdPtr->ini->toString(n+1) , unsorted[n].itemValue );
			break;
		}
	}
	rdPtr->ini->PauseAutoSave(false);
	rdPtr->ini->RequestAutoSave();

	return 0;
}

long WINAPI DLLExport expressionNthOverallItem(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	int p1=CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_LONG);
	ADJUST_FOR_INDEX(p1);	
	std::string str = rdPtr->ini->GetNthGeneralItemName(p1);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	RETURN_STDSTRING( str );
}

long WINAPI DLLExport expressionGetItemArray(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();


	char * grp=(LPSTR)CNC_GetFirstExpressionParameter(rdPtr, param1, TYPE_STRING);
	char * itm=(LPSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);
	int pos= CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_LONG);
	char * def=(LPSTR)CNC_GetNextExpressionParameter(rdPtr, param1, TYPE_STRING);

	ADJUST_FOR_INDEX(pos);	

	std::string str = rdPtr->ini->GetItemArray(grp,itm,pos-1,def);
	rdPtr->rHo.hoFlags |= HOF_STRING;
	RETURN_STDSTRING( str );
}










long WINAPI DLLExport expressionUniqueItemCount(LPRDATA rdPtr, long param1)
{
	return rdPtr->ini->GetNthGeneralItemNameCount();
}



#include "ChartObject.h"


DynamicArray SplitByComma( std::string what );
//Chart object action
short WINAPI DLLExport actionToChart(LPRDATA rdPtr, long param1, long param2){
	GLOBAL_SETTINGS_INTO_MEMORY();

	paramExt* p1=(paramExt*)CNC_GetParameter(rdPtr); //Object name
	char* p2=(LPSTR)CNC_GetParameter(rdPtr); //Group

	//These are not used yet.
	char* p3=(LPSTR)CNC_GetParameter(rdPtr); //item.
	char* p4=(LPSTR)CNC_GetParameter(rdPtr); //value.

	//This object doesn't actually need to be an array
	ChartObject::CHART_RUNDATA*  obj = ChartObject::FindChartObject( p1->pextData , rdPtr );
	if (obj != NULL )
	{


		int max = rdPtr->ini->ItemCount(p2);
		for ( int n = 1 ; n <= max ; n++ )
		{
			std::string name = rdPtr->ini->GetNthItemName(p2,n,"");
			std::string result = rdPtr->ini->GetNthItemValue(p2,n,"");
		
			float fname = StringToFloat(name.c_str());		
			//Split result up into parts
			DynamicArray parts = SplitByComma( result );
			float fresult = StringToFloat(parts[0].c_str());
			int col1,col2;
			col1 = 0x00ff;
			if ( parts.size() >= 2 )
				col1 = atoi(parts[1].c_str());
			col2 = col1;
			if ( parts.size() >= 3 )
				col2 = atoi(parts[2].c_str());

			obj->setPoint( (headerObject*)obj , *((int*)(&fname)) , *((int*)(&fresult))  , col1 , col2 , name.c_str() , 1  );	//Last parameter: 1 = At end.
		}
	}
	return 0;
}

short WINAPI DLLExport actionFromChart(LPRDATA rdPtr, long param1, long param2){
	MODIFIES_DATA();
	GLOBAL_SETTINGS_INTO_MEMORY();

	paramExt* p1=(paramExt*)CNC_GetParameter(rdPtr); //Object name
	char* p2=(LPSTR)CNC_GetParameter(rdPtr); //Group

	ChartObject::CHART_RUNDATA*  obj = ChartObject::FindChartObject( p1->pextData , rdPtr );
	if (obj != NULL )
	{
		ChartObject::ItemInfo structure;
		int i = 0;
		while ( obj->getPoint( (headerObject*)obj , i++ , &structure ) )
		{
			//Save what we have in structure
			char val[100];
			FloatToString( val , structure.y );
			std::string sval = val;

			//If there are colours we save those too.
			if ( (structure.flags & 2) != 0 )
				sval += "," + rdPtr->ini->toString(structure.col1);
			if ( (structure.flags & 4) != 0 )
				sval += "," + rdPtr->ini->toString(structure.col2);

			//If it has a string
			if ( (structure.flags & 1) != 0 )
				rdPtr->ini->SetItem( p2 , structure.label , sval );
			else
			{
				char name[100];
				FloatToString( name , structure.x );
				rdPtr->ini->SetItem( p2 , name , sval );
			}	
		}
	}
	return 0;
}




short WINAPI DLLExport actionLoadChartSettings(LPRDATA rdPtr, long param1, long param2)
{
	GLOBAL_SETTINGS_INTO_MEMORY();

	paramExt* p1=(paramExt*)CNC_GetParameter(rdPtr); //Object name
	char* p2=(LPSTR)CNC_GetParameter(rdPtr); //Group


	ChartObject::CHART_RUNDATA*  obj = ChartObject::FindChartObject( p1->pextData , rdPtr );
	if (obj != NULL )
	{
		ChartObject::ChartProperties p;
	
		obj->getSetProperties( (headerObject*)obj , false , &p );

		#define GRP(v)	rdPtr->ini->HasItem(p2,#v)
		#define	FLOAT(cond,v) if(GRP(v) && cond){ p.##v = StringToFloat(rdPtr->ini->GetItem( p2 , #v , "" ).c_str()); }
		#define	INT2(cond,v,cast) if(GRP(v) && cond){ p.##v = (cast)atoi(rdPtr->ini->GetItem( p2 , #v , "" ).c_str()); }
		#define	INT(cond,v) INT2(cond,v,int)
		#define	STRING(cond,v) if(GRP(v) && cond){ strncpy(p.##v,rdPtr->ini->GetItem( p2 , #v , p.##v ).c_str(),128);p.##v[127]=0;}
		#define BOOL(cond,v) if(GRP(v) && cond){ p.##v = !!atoi(rdPtr->ini->GetItem( p2 , #v , "" ).c_str()); }
		#define FONT(cond,v) {}

			#include "LoadSaveChartProperties.h"
		
		#undef	FLOAT
		#undef	INT
		#undef	INT2
		#undef	STRING
		#undef	BOOL
		#undef	FONT
		#undef  GRP

		obj->getSetProperties( (headerObject*)obj , true , &p );

	}
	return 0;
}



short WINAPI DLLExport actionSaveChartSettings(LPRDATA rdPtr, long param1, long param2)
{
	MODIFIES_DATA();
	GLOBAL_SETTINGS_INTO_MEMORY();

	paramExt* p1=(paramExt*)CNC_GetParameter(rdPtr); //Object name
	char* p2=(LPSTR)CNC_GetParameter(rdPtr); //Group


	ChartObject::CHART_RUNDATA*  obj = ChartObject::FindChartObject( p1->pextData , rdPtr );
	if (obj != NULL )
	{
		ChartObject::ChartProperties p;
	
		obj->getSetProperties( (headerObject*)obj , false , &p );

		#define	FLOAT(cond,v) if(cond){ char tmp[100];FloatToString( tmp , p.## v );rdPtr->ini->SetItem( p2 , #v , tmp ); }
		#define	INT(cond,v) if(cond){ char tmp[100];_itoa( p.## v,tmp,10 );rdPtr->ini->SetItem( p2 , #v , tmp ); }
		#define INT2(cond,v,cast) INT(cond,v)
		#define	STRING(cond,v) if(cond){ rdPtr->ini->SetItem( p2 , #v , p.##v ); }
		#define BOOL(a,b) INT(a,b)
		#define FONT(cond,v) {}
		/*if(cond){ char tmp[500];LogFontToString(tmp,p.##v);rdPtr->ini->SetItem( p2 , #v , tmp );}*/

			#include "LoadSaveChartProperties.h"
		
		#undef	FLOAT
		#undef	INT
		#undef	INT2
		#undef	STRING
		#undef	BOOL
		#undef	FONT
	}
	return 0;
}






short WINAPI DLLExport actionLoadChangeFile(LPRDATA rdPtr, long param1, long param2){
	MODIFIES_DATA();
	GLOBAL_SETTINGS_INTO_MEMORY();

	char* fname=(LPSTR)CNC_GetParameter(rdPtr); //Group
	paramExt* settings=(paramExt*)CNC_GetParameter(rdPtr); //Object name

	enum	LoadType	{	LT_LOAD=0,		LT_NOTHING						};
	enum	FNameType	{	FN_CHANGE=0,	FN_NOTHING ,	FN_CHANGE_IF_OK	};
	enum	DeleteType	{	DEL_CLEAR = 0 , DEL_NOTHING ,	DEL_CLEAR_IF_OK	};

	LoadType	loadType	  = (LoadType)	settings->pextData[0];
	FNameType	fnameType	  = (FNameType)	settings->pextData[1];
	DeleteType	delType		  = (DeleteType)settings->pextData[2];

	bool		saveRightAway =	settings->pextData[3] & 1;
	bool		readOnly	  =	settings->pextData[3] & 2;


	//Check if the file exists
	bool fileExists = true;
	if ( (delType == DEL_CLEAR_IF_OK) || (delType == FN_CHANGE_IF_OK) )
	{
			FileClass f(FormatPathReadOnly(fname),FileClass_READ);
			if ( f.is_open() )
			{
				f.close();
			}
			else fileExists = false;
	}

	rdPtr->ini->PauseAutoSave(true);

	//If we should clear all the data
	if ( (delType == DEL_CLEAR) || (delType == DEL_CLEAR_IF_OK && fileExists) )
		rdPtr->ini->ClearAll();

	//We try to load first
	bool loadedOK = false;
	if ( loadType == LT_LOAD )
	{
		if ( readOnly )
		{
			rdPtr->ini->SetReadOnly(true);
			loadedOK = rdPtr->ini->Load( FormatPathReadOnly(fname) , false );
		}
		else
		{
			rdPtr->ini->SetReadOnly(false);
			loadedOK = rdPtr->ini->Load( FormatPath(fname) , false );
		}
	}
	//Now we consider the file name
	if ( (fnameType == FN_CHANGE) || ((fnameType == FN_CHANGE_IF_OK) && loadedOK) )
		rdPtr->ini->SetAutoSavePath(fname);

	//Now we see if we should save
	if ( saveRightAway )
		rdPtr->ini->RequestAutoSave();

	rdPtr->ini->PauseAutoSave(false);
	GLOBAL_SETTINGS_CHANGE();
	return 0;
}


long WINAPI DLLExport expressionCurGroup(LPRDATA rdPtr, long param1)
{
	rdPtr->rHo.hoFlags |= HOF_STRING;
	std::string val = rdPtr->currentGroup;
	RETURN_STDSTRING( val );
}

long WINAPI DLLExport expressionCurGroupString(LPRDATA rdPtr, long param1)
{
	GLOBAL_SETTINGS_INTO_MEMORY();
	SuperINI ini2;
	SuperINI_SettingsStruct s;
	rdPtr->ini->ExportSettings(&s);
	ini2.ImportSettings(&s);
	ini2.SetAutoSavePath("");
	ini2.SetAutoSave(false);
	int total = rdPtr->ini->ItemCount(rdPtr->currentGroup);
	for ( int n = 1 ; n <= total  ; n++ )
		ini2.SetItem(rdPtr->currentGroup,rdPtr->ini->GetNthItemName(rdPtr->currentGroup,n,"") , rdPtr->ini->GetNthItemValue(rdPtr->currentGroup,n,"") ); 
	rdPtr->rHo.hoFlags |= HOF_STRING;
	std::string val = ini2.WriteStream();
	RETURN_STDSTRING( val );
}

long WINAPI DLLExport expressionFname(LPRDATA rdPtr, long param1)
{
	rdPtr->rHo.hoFlags |= HOF_STRING;
	std::string val = rdPtr->ini->GetFileName();
	RETURN_STDSTRING( val );
}

short WINAPI DLLExport actionUndo(LPRDATA rdPtr, long param1, long param2){
	GLOBAL_SETTINGS_INTO_MEMORY();
	rdPtr->ini->undo();
	return 0;
}

short WINAPI DLLExport actionRedo(LPRDATA rdPtr, long param1, long param2){
	GLOBAL_SETTINGS_INTO_MEMORY();
	rdPtr->ini->redo();
	return 0;
}

short WINAPI DLLExport actionClearUndoStack(LPRDATA rdPtr, long param1, long param2){
	GLOBAL_SETTINGS_INTO_MEMORY();
	int mode = CNC_GetParameter(rdPtr);
	rdPtr->ini->ClearStacks( mode & 1 , mode & 2 );
	return 0;
}

short WINAPI DLLExport actionAddNewUndoBlock(LPRDATA rdPtr, long param1, long param2){
	GLOBAL_SETTINGS_INTO_MEMORY();
	rdPtr->ini->forceNewUndoBlock();
	return 0;
}

short WINAPI DLLExport actionSetManualMode(LPRDATA rdPtr, long param1, long param2){
	GLOBAL_SETTINGS_INTO_MEMORY();
	int mode = CNC_GetParameter(rdPtr);
	rdPtr->ini->setManualMode( mode == 2 );
	return 0;
}


long (WINAPI * ConditionJumps[])(LPRDATA rdPtr, long param1, long param2) =
	{
	conditionHasGroupG,
	conditionHasItemG,
	conditionCompareMD5G,
	conditionHasGroup,
	conditionHasItem,
	conditionCompareMD5,
	conditionWildcatMatch,
	conditionEvent,
	conditionEvent,
	conditionEvent,
	conditionEvent,
	conditionEvent,
	conditionEvent,
	conditionEvent,
	conditionEvent,
	conditionEvent,
	conditionEvent,
	conditionEvent,
	conditionHasUndo,
	conditionHasRedo,
	0
	};

short (WINAPI * ActionJumps[])(LPRDATA rdPtr, long param1, long param2) =
	{
	actionSetCurrentGroup,
	actionSetValueG,
	actionSetStringG,
	actionSetStringMD5G,
	actionSaveObjectG,
	actionLoadObjectG,
	actionSavePositionG,
	actionSaveGlobalValuesG,
	actionLoadGlobalValuesG,
	actionDeleteGroupG,
	actionDeleteItemG,
	actionRenameGroupG,
	actionRenameItemG,
	actionMoveItemToGroupG,
	actionSetValue,
	actionSetString,
	actionSetStringMD5,
	actionSaveObject,
	actionLoadObject,
	actionSavePosition,
	actionSaveAltValues,
	actionLoadAltValues,
	actionRenameGroup,
	actionRenameItem,
	actionMoveItem,
	actionMoveItemToGroup,
	actionCopyGroup,
	actionCopyItem,
	actionDeleteGroup,
	actionDeleteItem,
	actionDeleteItemInAllGroups,
	actionDeleteFromPattern,
	actionClearAll,
	actionPerformSearch,
	actionPerformMultiplesSearch,
	actionClearResults,
	actionCreateSubINI,
	actionCreateSubINIObject,
	actionMergeFile,
	actionMergeGroupFile,
	actionMerge,
	actionMergeGroupObject,
	actionNew,
	actionLoad,
	actionSave,
	actionSaveAs,
	actionBackupTo,
	actionClose,
	actionLoadFromString,
	actionSetAutoSave,
	actionSetCompresson,
	actionSetEncryption,
	actionSetReadOnly,
	actionSetCaseSensitive,
	actionSetEscapeChars,
	actionNeverQuoteStrings,
	actionSetRepeatModes,
	actionSetNewLineChar,
	actionSetDefaultDirectory,
	actionCompressFile,
	actionDeCompressFile,
	actionOpenDialog,
	actionAddRepeatedItem,
	actionCloseDialog,
	actionRefreshDialog,
	actionExportCVS,
	actionImportCVS,
	actionToChart,
	actionFindSubGroups,
	actionEnableSubGroups,
	actionSwitchGlobalObject,
	actionSSS,
	actionSetItemArray,
	actionLoadFromArray,
	actionSaveToArray,
	actionFromChart,
	actionSaveChartSettings,
	actionLoadChartSettings,
	actionLoadChangeFile,
	actionUndo,
	actionRedo,
	actionClearUndoStack,
	actionAddNewUndoBlock,
	actionSetManualMode,
	0
	};

long (WINAPI * ExpressionJumps[])(LPRDATA rdPtr, long param) =
	{
	expressionGetItemValueG,
	expressionGetItemStringG,
	expressionGetXG,
	expressionGetYG,
	expressionNthItemG,
	expressionNthItemValueG,
	expressionNthItemStringG,
	expressionItemCountG,
	expressionGetItemValue,
	expressionGetItemString,
	expressionGetX,
	expressionGetY,
	expressionNthGroup,
	expressionNthItem,
	expressionNthItemString,
	expressionNthItemValue,
	expressionGroupCount,
	expressionItemCount,
	expressionTotalItems,
	expressionSearchResultCounts,
	expressionGetNthSearchResultGroup,
	expressionGetNthSearchResultItem,
	expressionGetNthSearchResultItemValue,
	expressionGetNthSearchResultItemString,
	expressionGetNthSearchResultPath,
	expressionWriteStream,
	expressionHashString,
	expressionEncryptString,
	expressionEscapeString,
	expressionUnEscapeString,
	expressionInnerProduct,
	expressionInnerProductStr,
	expressionNthSortedName,
	expressionNthSortedValue,
	expressionGetCSV,
	expressionNthOverallItem,
	expressionUniqueItemCount,
	expressionGetItemArray,
	expressionCurGroup,
	expressionCurGroupString,
	expressionFname,
	0
	};


 
