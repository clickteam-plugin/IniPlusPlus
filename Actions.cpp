/* Actions.cpp
 * This is where you should define the
 * behavior of your actions. Make sure
 * the parameters match those in the JSON
 * exactly! Double check Extension.h
 * as well.
 */

#include "Common.h"

void Extension::actionSetCurrentGroup(TCHAR const *group, int flag)
{
	if(flag == 0 || !hasGroup(group))
	{
		data->currentGroup = group;
	}
	else
	{
		stdtstring g = group;
		g += '.';
		for(unsigned i = 1; hasGroup(g + std::to_wstring(i)); ++i)
		{
			data->currentGroup = g + std::to_wstring(i);
		}
	}
}

void Extension::actionSetValueG(TCHAR const *item, int flag, float v) //TODO
{
	return actionSetValue(data->currentGroup.c_str(), item, flag, v);
}

void Extension::actionSetStringG(TCHAR const *item, TCHAR const *v)
{
	return actionSetString(data->currentGroup.c_str(), item, v);
}

void Extension::actionSetStringMD5G(TCHAR const *item, TCHAR const *v)
{
	//
}

void Extension::actionSaveObjectG(RunObject *o, int flag_pos, int flag_alt)
{
	//
}

void Extension::actionLoadObjectG(RunObject *o)
{
	//
}

void Extension::actionSavePositionG(TCHAR const *item, unsigned position)
{
	//
}

void Extension::actionSaveGlobalValuesG()
{
	//
}

void Extension::actionLoadGlobalValuesG()
{
	//
}

void Extension::actionDeleteGroupG()
{
	//
}

void Extension::actionDeleteItemG(TCHAR const *item)
{
	return actionDeleteItem(data->currentGroup.c_str(), item);
}

void Extension::actionRenameGroupG(TCHAR const *newname, int mode)
{
	//
}

void Extension::actionRenameItemG(TCHAR const *item, TCHAR const *newname, int flag)
{
	//
}

void Extension::actionMoveItemToGroupG(TCHAR const *item, TCHAR const *group, int flag)
{
	//
}

void Extension::actionSetValue(TCHAR const *group, TCHAR const *item, int flag, float v) //TODO
{
	stdtstring value = ((flag == 0)? std::to_wstring(int(v)) : std::to_wstring(v));
	return doDoer<ValueDoer>(*this, group, item, value);
}

void Extension::actionSetString(TCHAR const *group, TCHAR const *item, TCHAR const *v)
{
	return doDoer<ValueDoer>(*this, group, item, v);
}

void Extension::actionSetStringMD5(TCHAR const *group, TCHAR const *item, TCHAR const *v)
{
	//
}

void Extension::actionSaveObject(RunObject *o, TCHAR const *group, int flag_pos, int flag_alt)
{
	//
}

void Extension::actionLoadObject(RunObject *o, TCHAR const *group)
{
	//
}

void Extension::actionSavePosition(TCHAR const *group, TCHAR const *item, unsigned position)
{
	//
}

void Extension::actionSaveAltValues(TCHAR const *group)
{
	//
}

void Extension::actionLoadAltValues(TCHAR const *group)
{
	//
}

void Extension::actionRenameGroup(TCHAR const *group, TCHAR const *newname, int mode)
{
	//
}

void Extension::actionRenameItem(TCHAR const *group, TCHAR const *item, TCHAR const *newname, int flag)
{
	//
}

void Extension::actionMoveItem(TCHAR const *group, TCHAR const *item, TCHAR const *newgroup, TCHAR const *newitem, int flag)
{
	//
}

void Extension::actionMoveItemToGroup(TCHAR const *group, TCHAR const *item, TCHAR const *newgroup, int flag)
{
	//
}

void Extension::actionCopyGroup(TCHAR const *group, TCHAR const *newname, int mode)
{
	//
}

void Extension::actionCopyItem(TCHAR const *group, TCHAR const *item, TCHAR const *newgroup, TCHAR const *newitem)
{
	//
}

void Extension::actionDeleteGroup(TCHAR const *group)
{
	//
}

void Extension::actionDeleteItem(TCHAR const *group, TCHAR const *item)
{
	return doDoer<DeleteItemDoer>(*this, group, item);
}

void Extension::actionDeleteItemInAllGroups(TCHAR const *item)
{
	//
}

void Extension::actionDeleteFromPattern(TCHAR const *gpatern, TCHAR const *ipattern, TCHAR const *vpattern, int flag)
{
	//
}

void Extension::actionClearAll()
{
	//
}

void Extension::actionPerformSearch(TCHAR const *gcond, TCHAR const *icond, TCHAR const *vcond, void *settings) //TODO
{
	//
}

void Extension::actionPerformMultiplesSearch(TCHAR const *group, TCHAR const *item)
{
	//
}

void Extension::actionClearResults()
{
	//
}

void Extension::actionCreateSubINI(TCHAR const *file, TCHAR const *gcond, TCHAR const *icond, TCHAR const *vcond, int flag)
{
	//
}

void Extension::actionCreateSubINIObject(void *object_name, TCHAR const *gcond, TCHAR const *icond, TCHAR const *vcond, int flag) //TODO
{
	//
}

void Extension::actionMergeFile(TCHAR const *file, int flag)
{
	//
}

void Extension::actionMergeGroupFile(TCHAR const *file, TCHAR const *filegroup, TCHAR const *group, int flag)
{
	//
}

void Extension::actionMerge(void *objectname, int flag) //TODO
{
	//
}

void Extension::actionMergeGroupObject(void *objectname, TCHAR const *objectgroup, TCHAR const *group, int flag) //TODO
{
	//
}

void Extension::actionNew(TCHAR const *file, int flag)
{
	if(flag == 0)
	{
		actionClose();
	}
	data->autoSavePath = file;
	if(flag != 0 && data->bAutoSave)
	{
		actionSave();
	}
}

void Extension::actionLoad(TCHAR const *file, int flag)
{
	data->ReadOnly = (flag? true : false);
	std::ifstream ifs (file);
	if(ifs)
	{
		//load entire file and convert it to unicode
		std::basic_istringstream<TCHAR> iss {utf16from8(std::string(std::istreambuf_iterator<char>(ifs), {}))};
		return loadIni(iss);
	}
	else
	{
		return doError(stdtstring{_T("Unable to read file ")}+file);
	}
}

void Extension::actionSave()
{
	return actionSaveAs(data->autoSavePath.c_str());
}

void Extension::actionSaveAs(TCHAR const *file)
{
	//TODO: optionally create folders if they don't exist
	std::basic_ostringstream<TCHAR> oss;
	saveIni(oss);
	auto str = utf8from16(oss.str());
	std::ofstream{file} << str;
}

void Extension::actionBackupTo(TCHAR const *file, int flag, TCHAR const *key)
{
	//
}

void Extension::actionClose()
{
	data->autoSavePath.clear();
	data->ini.clear();
	data->ReadOnly = false;
}

void Extension::actionLoadFromString(TCHAR const *inistr, int mode)
{
	if(mode == 0)
	{
		data->ini.clear();
	}
	std::basic_istringstream<TCHAR> iss {inistr};
	return loadIni(iss);
}

void Extension::actionSetAutoSave(int flag_save, int flag_load)
{
	data->bAutoSave = (flag_save? true : false);
	data->autoLoad = (flag_load? true : false);
}

void Extension::actionSetCompresson(int flag)
{
	//
}

void Extension::actionSetEncryption(TCHAR const *key)
{
	//
}

void Extension::actionSetReadOnly(int flag)
{
	//
}

void Extension::actionSetCaseSensitive(int flag)
{
	//
}

void Extension::actionSetEscapeChars(int flag_groups, int flag_items, int flag_values)
{
	//
}

void Extension::actionNeverQuoteStrings(int flag)
{
	//
}

void Extension::actionSetRepeatModes(int flag_groups, int flag_items, int flag_repeats)
{
	//
}

void Extension::actionSetNewLineChar(TCHAR const *str)
{
	//
}

void Extension::actionSetDefaultDirectory(TCHAR const *directory)
{
	//
}

void Extension::actionCompressFile(TCHAR const *file_source, TCHAR const *file_dest)
{
	//
}

void Extension::actionDeCompressFile(TCHAR const *file_source, TCHAR const *file_dest)
{
	//
}

void Extension::actionOpenDialog(TCHAR const *title, void *settings) //TODO
{
	//
}

void Extension::actionAddRepeatedItem(TCHAR const *group, TCHAR const *item, TCHAR const *str, int mode)
{
	//
}

void Extension::actionCloseDialog()
{
	//
}

void Extension::actionRefreshDialog(int mode)
{
	//
}

void Extension::actionExportCVS(int io, TCHAR const *file, TCHAR const *group, TCHAR const *separator)
{
	//
}

void Extension::actionImportCVS(int io, TCHAR const *file, TCHAR const *separator)
{
	//
}

void Extension::actionToChart(void *objectname, TCHAR const *group) //TODO
{
	//
}

void Extension::actionFindSubGroups(TCHAR const *group, int depth)
{
	//
}

void Extension::actionEnableSubGroups(int flag_subgroups, int flag_empties)
{
	//
}

void Extension::actionSwitchGlobalObject(TCHAR const *dataslot)
{
	if(gdata.find(dataslot) == std::end(gdata))
	{
		//the new data slot should be independent from the old
		gdata.emplace(stdtstring{dataslot}, data = std::make_shared<Data>(*data));
	}
	else
	{
		data = gdata.at(dataslot).lock();
	}
}

void Extension::actionSSS(void *settings, TCHAR const *group)
{
	//
}

void Extension::actionSetItemArray(TCHAR const *group, TCHAR const *item, int part, TCHAR const *v)
{
	//
}

void Extension::actionLoadFromArray(void *objectname, TCHAR const *group) //TODO
{
	//
}

void Extension::actionSaveToArray(void *objectname, TCHAR const *group) //TODO
{
	//
}

void Extension::actionFromChart(void *objectname, TCHAR const *group) //TODO
{
	//
}

void Extension::actionSaveChartSettings(void *objectname, TCHAR const *group) //TODO
{
	//
}

void Extension::actionLoadChartSettings(void *objectname, TCHAR const *group) //TODO
{
	//
}

void Extension::actionLoadChangeFile(TCHAR const *file, paramExt *settings) //TODO: bugproof
{
	enum struct LoadType
	{
		Load,
		Nothing
	};
	enum struct FNameType
	{
		Change,
		Nothing,
		ChangeIfOk
	};
	enum struct DeleteType
	{
		Clear,
		Nothing,
		ClearIfOk
	};
	LoadType   lt = LoadType  (settings->pextData[0]);
	FNameType  ft = FNameType (settings->pextData[1]);
	DeleteType dt = DeleteType(settings->pextData[2]);
	bool saveNow  = (((settings->pextData[3]) & 1)? true : false);
	bool readOnly = (((settings->pextData[3]) & 2)? true : false);

	bool exists = false;
	if(std::ifstream ifs {file})
	{
		exists = true;
	}

	if(dt == DeleteType::Clear || (dt == DeleteType::ClearIfOk && exists))
	{
		data->ini.clear();
	}

	if(lt == LoadType::Load)
	{
		actionLoad(file, readOnly);
	}
	if(ft == FNameType::Change || (ft == FNameType::ChangeIfOk && exists))
	{
		data->autoSavePath = file;
	}
	if(saveNow)
	{
		actionSave();
	}
}

void Extension::actionUndo()
{
	undo();
}

void Extension::actionRedo()
{
	redo();
}

void Extension::actionClearUndoStack(int mode)
{
	//
}

void Extension::actionAddNewUndoBlock()
{
	//
}

void Extension::actionSetManualMode(int mode)
{
	//
}
