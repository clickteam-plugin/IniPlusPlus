/* Properties.cpp
 * In this file you will define the edittime
 * properties for your extension. By default
 * only a version number is displayed.
 * Functions defined here:
 * GetProperties
 * ReleaseProperties
 * GetPropCreateParam
 * ReleasePropCreateParam
 * GetPropValue
 * SetPropValue
 * GetPropCheck
 * SetPropCheck
 * EditProp
 * IsPropEnabled
 */

#include "Common.h"

#ifndef RUN_ONLY

/* Prop
 * This is the namespace the property identifiers
 * are stored in. It keeps them out of the global
 * namespace and it helps with intellisense.
 */
namespace Prop
{
	/* <enum>
	 * The property identitifers.
	 */
	enum
	{
		zNOT_USED = PROPID_EXTITEM_CUSTOM_FIRST,
		Version,
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

LPCTSTR DefaultFolderList[] =
{0,
	_T("Windows Folder"),
	_T("(Initial) Current Folder"),
	_T("Program Folder"),
	_T("Application Data (User)"),
	_T("Application Data (Common)"),
	NULL
};

LPCTSTR GroupRepeatList[] =
{0,
	_T("First Only"),
	_T("Last Only"),
	_T("Rename"),
	_T("Merge"),
	NULL
};
LPCTSTR ItemRepeatList[] =
{0,
	_T("First Only"),
	_T("Last Only"),
	_T("Rename"),
	NULL
};

int UndoRange[] = {-1, 127};

FilenameCreateParam fcp =
{
	_T("INI Files|*.ini|All Files|*.*|"),
	OFN_PATHMUSTEXIST
};

#define _P(s) ((UINT_PTR)_T(s))

PropData Properties[] =
{
	PropData_StaticString    (Prop::Version       , _P("Version #")                                     , _P("This is the current version of the Ini++ object.")),
	PropData_CheckBox        (Prop::GlobalData    , _P("Global Data?")                                  , _P("If this is checked, then the data is shared with all Ini++ objects with this property, even across frames. Important: Only one object may have this property per frame.")),
	PropData_EditString      (Prop::GlobalDataName, _P("Global data key")                               , _P("A key can be selected. All global Ini++ objects with the same key share the same data.")),
	PropData_Group           (Prop::PathTitle     , _P("File and Path Settings")                        , _P("File and Path Settings")),
	PropData_Filename_Check  (Prop::DefPath       , _P("Default File")                                  , _P("The file to be initially loaded. Relative paths may be used. If the field is left blank or the check-box unchecked no file will be loaded."), &fcp),
	PropData_CheckBox        (Prop::ReadOnly      , _P("Read Only?")                                    , _P("If selected, the file specified above will be opened in Read Only mode. This does not apply if another file is opened or created.")),
	PropData_ComboBox        (Prop::DefFolder     , _P("Base Folder")                                   , _P("The folder that all relative paths are relative to."), DefaultFolderList),
	PropData_EditMultiLine   (Prop::DefaultText   , _P("Initial Data")                                  , _P("Data loaded into the object after the default file is loaded. Object settings are used except encryption and compression.")),
	PropData_CheckBox        (Prop::AutoLoad      , _P("Enable 'Auto Load'")                            , _P("Before any condition, expression or action, reload the file. Avoid this opition if possible.")),
	PropData_CheckBox        (Prop::AutoSave      , _P("Enable 'Auto Save'")                            , _P("When the object is written to when a file is open, automatically resave the file?")),
	PropData_CheckBox        (Prop::CreateFolders , _P("Create folders for files if they do not exist?"), _P("Allow the Save functions to create folders if they do not exist. If not selected, folders (but not files) must exist.")),
	PropData_CheckBox        (Prop::Index         , _P("1-based index")                                 , _P("If selected, any specified index will start at 1. Otherwise it will start at 0.")),
	PropData_Group           (Prop::CompEncYTitle , _P("Compression & Encyption")                       , _P("Compression & Encyption")),
	PropData_CheckBox        (Prop::Compress      , _P("Use Compression?")                              , _P("If selected, the Ini file will be decompressed when loaded and compressed when saved.")),
	PropData_EditString_Check(Prop::Encrypt       , _P("Encryption")                                    , _P("If the box is checked then the Ini file will be decrypted when loaded and encrypted when saved, with the key specified.")),
	PropData_Group           (Prop::UndoTitle     , _P("Undo/Redo Buffer Size")                         , _P("Undo/Redo Buffer Size")),
	PropData_SpinEdit        (Prop::Undo          , _P("Undo:")                                         , _P("Number of actions that can be undone. -1 for infinite."), &UndoRange),
	PropData_SpinEdit        (Prop::Redo          , _P("Redo:")                                         , _P("Number of 'undo's that can be redone. -1 for infinite."), &UndoRange),
	PropData_End()
};

#endif

/* GetProperties
 * Here, you choose what proeprties to insert
 * into which tabs. Lots of dynamic things can
 * be done in here, such as dynamically
 * generating the properties based on the
 * number of stored values in the editdata.
 */
BOOL MMF2Func GetProperties(mv *mV, SerializedED *SED, BOOL MasterItem)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	mvInsertProps(mV, SED, Properties, PROPID_TAB_GENERAL, TRUE);
	//if you changed ed:
	//ed.Serialize(mV, SED);
	return TRUE;
#endif
	return FALSE;
}

/* ReleaseProperties
 * If you dynamically allocated memory for
 * property arrays in GetProperties, release
 * that memory in this function.
 */
void MMF2Func ReleaseProperties(mv *mV, SerializedED *SED, BOOL MasterItem)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	//
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
}

/* GetPropCreateParam
 * A convenience function if you use the
 * hard-coded Properties array above. Allows
 * you to dynamically create e.g. combo
 * lists and other things that have
 * additional parameters for their
 * properties. However if you dynamically
 * allocated everything in GetProperties,
 * this function is useless.
 */
LPARAM MMF2Func GetPropCreateParam(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	//return (LPARAM)TheParameter;
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return 0;
}

/* ReleasePropCreateParam
 * If you found a useful way to use the
 * above function and you dynamically
 * allocated memory, release it here.
 */
void MMF2Func ReleasePropCreateParam(mv *mV, SerializedED *SED, UINT PropID, LPARAM lParam)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	//MyAwesomeMemoryFreeingFunction(lParam);
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
}

/* GetPropValue
 * This is where you return the display
 * values for the properties you defined.
 * Refer to the MMF2SDK Help file for
 * information on what to return for
 * each kind of property.
 */
void *MMF2Func GetPropValue(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	switch(PropID)
	{
	case Prop::Version:
		{
			return new CPropStringValue(_T("v1.6 July 2015"));
		}
	//case Prop::MyString:
	//	{
	//		return new CPropStringValue(ed.MyString.c_str());
	//	}
	//case Prop::MyInt:
	//	{
	//		return new CPropDWordValue(ed.MyInt);
	//	}
	}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return NULL;
}

/* SetPropValue
 * The user has just finished entering their
 * 500 page essay into your MyString field.
 * MMF2 isn't so kind as to save that for them,
 * so it entrusts it with you. Store it!
 */
void MMF2Func SetPropValue(mv *mV, SerializedED *SED, UINT PropID, CPropValue *PropVal)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//switch(PropID)
	//{
	//case Prop::MyString:
	//	{
	//		ed.MyString = ((CPropStringValue*)PropVal)->GetString();
	//		break;
	//	}
	//case Prop::MyInt:
	//	{
	//		ed.MyInt = (CPropDWordValue*)PropVal)->m_dwValue;
	//		break;
	//	}
	//}
	//since you changed ed:
	//ed.Serialize(mV, SED);

	//You may want to have your object redrawn in the
	//frame editor after the modifications; in this
	//case, just call this function:
	//mvInvalidateObject(mV, SED);
#endif
}

/* GetPropCheck
 * There are checkbox properties, and
 * there are properties that have their
 * own checkbox as an option. This is
 * where you tell MMF2 whether those
 * boxes are ticked or not.
 */
BOOL MMF2Func GetPropCheck(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//switch(PropID)
	//{
	//case Prop::MyCheckBoxPropertyOrPropertyThatHasTheCheckboxOptionSet:
	//	{
	//		return ed.WhetherOrNotThatPropertyOfMineIsTicked ? TRUE : FALSE;
	//	}
	//}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return FALSE;
}

/* SetPropCheck
 * The user has painstakingly moved the
 * mouse cursor over the checkbox and
 * expended the immense effort required
 * to click the mouse and toggle the
 * state of the tickbox. Don't let their
 * effort be all for naught!
 */
void MMF2Func SetPropCheck(mv *mV, SerializedED *SED, UINT PropID, BOOL Ticked)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//switch(PropID)
	//{
	//case Prop::MyCheckBoxPropertyOrPropertyThatHasTheCheckboxOptionSet:
	//	{
	//		ed.WhetherOrNotThatPropertyOfMineIsTicked = Ticked != FALSE ? true : false;
	//	}
	//}
	//since you changed ed:
	//ed.Serialize(mV, SED);
#endif
}

/* EditProp
 * If you use a button proeprty or a
 * property that has a button, then
 * you would be impolite to ignore
 * the user when they click on that
 * button. Here is where you respond
 * to that button press e.g. with
 * a dialog.
 */
BOOL MMF2Func EditProp(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//switch(PropID)
	//{
	//case Prop::MyButtonPropertyOrPropertyThatHasAButtonWithIt:
	//	{
	//		//
	//		break;
	//	}
	//}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return FALSE;
}

/* IsPropEnabled
 * Let's say you're tired of having to deal
 * with the user's various 500 page essays,
 * tickbox toggles, and button clicks. Here,
 * you can simply disable a property by
 * returning FALSE for that property.
 */
BOOL MMF2Func IsPropEnabled(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	switch(PropID)
	{
	case Prop::Version:
		{
			return FALSE; //Makes the version proeprty greyed out
		}
	//case Prop::MyString:	//intentional\\
	//case Prop::MyInt:		//fallthrough\\
	//	{
	//		return TRUE; //allows the user to interact with these proeprties
	//	}
	}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return FALSE;
}
