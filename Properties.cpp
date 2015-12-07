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
		zLast
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

PropData GeneralProps[] =
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
PropData AdvancedProps[] =
{
	PropData_Group               (Prop::IniTitle        , _P("Format Settings")                  , _P("Format Settings")),
	PropData_CheckBox            (Prop::StandardQM      , _P("Use Standard Settings?")           , _P("Use standard Ini file settings with no encryption or compression. Disables 'special' features.")),
	PropData_Group               (Prop::TreeTitle       , _P("Advanced Settings")                , _P("Custom settings can only be used if \"Standard Ini\" mode is disabled.")),
		PropData_EditString_Check(Prop::NewLine         , _P("New line")                         , _P("The string which represents new lines. If the box is unchecked then the standard Windows new line is used.")),
		PropData_CheckBox        (Prop::QuoteStrings    , _P("Always quote strings?")            , _P("If this is selected then string values are always quoted. Otherwise, they will be only if they have leading or trailing white space.")),
		PropData_CheckBox        (Prop::CaseSensitive   , _P("Case sensitive?")                  , _P("If selected, group and item names are case sensitive.")),
		PropData_CheckBox        (Prop::AllowEmptyGroups, _P("Allow empty groups")               , _P("With this selected, empty groups will still be said to exist.")),
		PropData_CheckBox        (Prop::SubGroups       , _P("Load and Save Subgroups")          , _P("When selected, indented groups will be considered 'subgroups', and they will be renamed to the 'path' of the group.")),
		PropData_ComboBox        (Prop::GroupRepeat     , _P("Repeated Groups")                  , _P("See documentation."), GroupRepeatList),
		PropData_ComboBox        (Prop::ItemRepeat      , _P("Repeated Items")                   , _P("See documentation."), ItemRepeatList),
		PropData_CheckBox        (Prop::RepeatSave      , _P("Save repeated items as original?") , _P("If selected, repeated items will be saved without all the text before the last dot. This may result in multiple items having the same name.")),
		PropData_CheckBox        (Prop::EscapeChar_Group, _P("Escape characters in group names?"), _P("Escape characters in group names?")),
		PropData_CheckBox        (Prop::EscapeChar_Item , _P("Escape characters in item names?") , _P("Escape characters in item names?")),
		PropData_CheckBox        (Prop::EscapeChar_Value, _P("Escape characters in item values?"), _P("Escape characters in item values?")),
//	PropData_Folder_End          (),
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
	mvInsertProps(mV, SED, GeneralProps , PROPID_TAB_GENERAL, TRUE);
	mvInsertProps(mV, SED, AdvancedProps, PROPID_TAB_CUSTOM1, TRUE);
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
	EditData ed (mV, SED);
	switch(PropID)
	{
	case Prop::Version:
		{
			#ifdef UNICODE
			return new CPropStringValue(_T("Unicode v1.6 December 2015 (v0.2.1-chrilley)"));
			#else
			return new CPropStringValue(_T("ANSI v1.6 December 2015 (v0.2.1-chrilley)"));
			#endif
		}
	case Prop::DefPath:
		{
			return new CPropStringValue(ed.defaultFile.c_str());
		}
	case Prop::DefFolder:
		{
			return new CPropDWordValue(ed.defaultFolder);
		}
	case Prop::DefaultText:
		{
			return new CPropStringValue(ed.defaultText.c_str());
		}
	case Prop::Encrypt:
		{
			return new CPropStringValue(ed.encrypt_key.c_str());
		}
	case Prop::NewLine:
		{
			return new CPropStringValue(ed.newline.c_str());
		}
	case Prop::GroupRepeat:
		{
			return new CPropDWordValue(ed.repeatGroups);
		}
	case Prop::ItemRepeat:
		{
			return new CPropDWordValue(ed.repeatItems);
		}
	case Prop::GlobalDataName:
		{
			return new CPropStringValue(ed.globalKey.c_str());
		}
	case Prop::Undo:
		{
			return new CPropDWordValue(ed.undoCount);
		}
	case Prop::Redo:
		{
			return new CPropDWordValue(ed.redoCount);
		}
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
	EditData ed (mV, SED);
	switch(PropID)
	{
	case Prop::DefPath:
		{
			ed.defaultFile = ((CPropStringValue *)PropVal)->GetString();
		} break;
	case Prop::DefaultText:
		{
			ed.defaultText = ((CPropStringValue *)PropVal)->GetString();
		} break;
	case Prop::DefFolder:
		{
			ed.defaultFolder = ((CPropDWordValue *)PropVal)->m_dwValue;
		} break;
	case Prop::Encrypt:
		{
			ed.encrypt_key = ((CPropStringValue *)PropVal)->GetString();
		} break;
	case Prop::NewLine:
		{
			ed.newline = ((CPropStringValue *)PropVal)->GetString();
		} break;
	case Prop::GroupRepeat:
		{
			ed.repeatGroups = ((CPropDWordValue *)PropVal)->m_dwValue;
		} break;
	case Prop::ItemRepeat:
		{
			ed.repeatItems = (char)(((CPropDWordValue *)PropVal)->m_dwValue);
		} break;
	case Prop::GlobalDataName:
		{
			ed.globalKey = ((CPropStringValue *)PropVal)->GetString();
			ed.loadGlobal(mV);
			ed.Serialize(mV, SED);
			for(UINT id = PROPID_EXTITEM_CUSTOM_FIRST + 1; id != Prop::zLast; ++id)
			{
				mvRefreshProp(mV, SED, id, FALSE);
			}
		} break;
	case Prop::Undo:
		{
			ed.undoCount = (signed char)(((CPropDWordValue *)PropVal)->m_dwValue);
		} break;
	case Prop::Redo:
		{
			ed.redoCount = (signed char)(((CPropDWordValue *)PropVal)->m_dwValue);
		} break;
	default: return;
	}
	ed.Serialize(mV, SED);

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
	EditData ed (mV, SED);
	switch(PropID)
	{
	case Prop::DefPath:
		{
			return ed.b_defaultFile ? TRUE : FALSE;
		}
	case Prop::CreateFolders:
		{
			return ed.bool_CanCreateFolders ? TRUE : FALSE;
		}
	case Prop::AutoSave:
		{
			return ed.bool_AutoSave ? TRUE : FALSE;
		}
	case Prop::StandardQM:
		{
			return ed.bool_stdINI ? TRUE : FALSE;
		}
	case Prop::Compress:
		{
			return ed.bool_compress ? TRUE : FALSE;
		}
	case Prop::Encrypt:
		{
			return ed.bool_encrypt ? TRUE : FALSE;
		}
	case Prop::NewLine:
		{
			return ed.bool_newline ? TRUE : FALSE;
		}
	case Prop::QuoteStrings:
		{
			return ed.bool_QuoteStrings ? TRUE : FALSE;
		}
	case Prop::EscapeChar_Group:
		{
			return ed.bool_EscapeGroup ? TRUE : FALSE;
		}
	case Prop::EscapeChar_Item:
		{
			return ed.bool_EscapeItem ? TRUE : FALSE;
		}
	case Prop::EscapeChar_Value:
		{
			return ed.bool_EscapeValue ? TRUE : FALSE;
		}
	case Prop::CaseSensitive:
		{
			return ed.bool_CaseSensitive ? TRUE : FALSE;
		}
	case Prop::ReadOnly:
		{
			return ed.b_ReadOnly ? TRUE : FALSE;
		}
	case Prop::GlobalData:
		{
			return ed.globalObject ? TRUE : FALSE;
		}
	case Prop::RepeatSave:
		{
			return ed.saveRepeated ? TRUE : FALSE;
		}
	case Prop::Index:
		{
			return ed.index ? TRUE : FALSE;
		}
	case Prop::AutoLoad:
		{
			return ed.autoLoad ? TRUE : FALSE;
		}
	case Prop::SubGroups:
		{
			return ed.subGroups ? TRUE : FALSE;
		}
	case Prop::AllowEmptyGroups:
		{
			return ed.allowEmptyGroups ? TRUE : FALSE;
		}
	}
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
	bool ticked = ((Ticked != FALSE)? true : false);
	EditData ed (mV, SED);
	switch(PropID)
	{
	case Prop::DefPath:
		{
			ed.b_defaultFile = ticked;
			ed.Serialize(mV, SED);
			mvRefreshProp(mV, SED, Prop::ReadOnly, FALSE);
			return;
		} break;
	case Prop::CreateFolders:
		{
			ed.bool_CanCreateFolders = ticked;
		} break;
	case Prop::AutoSave:
		{
			ed.bool_AutoSave = ticked;
		} break;
	case Prop::StandardQM:
		{
			ed.bool_stdINI = ticked;
			ed.Serialize(mV, SED);
//			mvRefreshProp(mV, SED, Prop::Compress, FALSE);
//			mvRefreshProp(mV, SED, Prop::Encrypt, FALSE);
			mvRefreshProp(mV, SED, Prop::NewLine, FALSE);
			mvRefreshProp(mV, SED, Prop::QuoteStrings, FALSE);
			mvRefreshProp(mV, SED, Prop::GroupRepeat, FALSE);
			mvRefreshProp(mV, SED, Prop::ItemRepeat, FALSE);
			mvRefreshProp(mV, SED, Prop::EscapeChar_Group, FALSE);
			mvRefreshProp(mV, SED, Prop::EscapeChar_Item, FALSE);
			mvRefreshProp(mV, SED, Prop::EscapeChar_Value, FALSE);
			mvRefreshProp(mV, SED, Prop::CaseSensitive, FALSE);
			mvRefreshProp(mV, SED, Prop::RepeatSave, FALSE);
			mvRefreshProp(mV, SED, Prop::SubGroups, FALSE);
			mvRefreshProp(mV, SED, Prop::AllowEmptyGroups, FALSE);
			return;
		} break;
	case Prop::Compress:
		{
			ed.bool_compress = ticked;
		} break;
	case Prop::Encrypt:
		{
			ed.bool_encrypt = ticked;
		} break;
	case Prop::NewLine:
		{
			ed.bool_newline = ticked;
		} break;
	case Prop::QuoteStrings:
		{
			ed.bool_QuoteStrings = ticked;
		} break;
	case Prop::EscapeChar_Group:
		{
			ed.bool_EscapeGroup = ticked;
		} break;
	case Prop::EscapeChar_Item:
		{
			ed.bool_EscapeItem = ticked;
		} break;
	case Prop::EscapeChar_Value:
		{
			ed.bool_EscapeValue = ticked;
		} break;
	case Prop::CaseSensitive:
		{
			ed.bool_CaseSensitive = ticked;
		} break;
	case Prop::ReadOnly:
		{
			ed.b_ReadOnly = ticked;
		} break;
	case Prop::GlobalData:
		{
			ed.globalObject = ticked;
			ed.loadGlobal(mV);
			ed.Serialize(mV, SED);
			for(UINT id = PROPID_EXTITEM_CUSTOM_FIRST + 1; id != Prop::zLast; ++id)
			{
				mvRefreshProp(mV, SED, id, FALSE);
			}
			return;
		} break;
	case Prop::RepeatSave:
		{
			ed.saveRepeated = ticked;
		} break;
	case Prop::Index:
		{
			ed.index = ticked;
		} break;
	case Prop::AutoLoad:
		{
			ed.autoLoad = ticked;
		} break;
	case Prop::SubGroups:
		{
			ed.subGroups = ticked;
		} break;
	case Prop::AllowEmptyGroups:
		{
			ed.allowEmptyGroups = ticked;
		} break;
	default: return;
	}
	ed.Serialize(mV, SED);
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
	EditData ed (mV, SED);
	switch(PropID)
	{
	case Prop::Version:
		{
			return FALSE;
		}
//	case Prop::Compress:
//	case Prop::Encrypt:
	case Prop::NewLine:
	case Prop::QuoteStrings:
	case Prop::GroupRepeat:
	case Prop::ItemRepeat:
	case Prop::EscapeChar_Group:
	case Prop::EscapeChar_Item:
	case Prop::EscapeChar_Value:
	case Prop::CaseSensitive:
	case Prop::RepeatSave:
	case Prop::SubGroups:
	case Prop::AllowEmptyGroups:
		{
			return ed.bool_stdINI? FALSE : TRUE;
		}
	case Prop::ReadOnly:
		{
			return ed.b_defaultFile? TRUE : FALSE;
		}
	case Prop::GlobalDataName:
		{
			return ed.globalObject? TRUE : FALSE;
		}
	default: return TRUE;
	}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return FALSE;
}
