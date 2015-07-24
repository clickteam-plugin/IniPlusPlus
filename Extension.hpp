/* Extension.h
 * This is where you define your extension
 * class. Runtime variables and function
 * declarations go in here; A/C/E function
 * definitions go in their respective .cpp
 * files, other function definitions go in
 * Extension.cpp. This header is included
 * at the bottom of Common.h, so you have
 * access to everything included by it.
 */

class Extension
{
public:
	/* rd
	 * This is a pointer to the classic
	 * RunData structure. Some things still
	 * have to be stored there for MMF2's
	 * sake, so you need to have acces to
	 * that data. You should not modify
	 * the rundata structure, however,
	 * as it is global to all extensions.
	 */
	RD *rd;

	/* rh
	 * This is a pointer to the RunHeader
	 * structure, which contains some
	 * application information.
	 */
	RunHeader *rh;

	/* Runtime
	 * This is EDIF's compendium of useful
	 * functions for every day life as an
	 * extension. You can trigger immediate
	 * events, read and write global data,
	 * etc. See the EDIF wiki for more
	 * information.
	 */
	Edif::Runtime Runtime;

	/* MinimumBuild
	 * Always set this to the latest build
	 * of MMF2 that has been released, unless
	 * you need to set it higher for a beta
	 * version of MMF2 to get access to some
	 * new SDK feature.
	 */
	static const int MinimumBuild = 284;

	/* Version
	 * This is the version of your extension
	 * that is stored in the editdata. You should
	 * only change this when your editdata structure
	 * changes, not just when you have a new version
	 * of your extension with new features.
	 */
	static const int Version = 2;

	/* OEFLAGS, OEPREFS
	 * These are settings for your extension
	 * that tell MMF2 how to treat your extension
	 * and what behaviors and responsibilities
	 * to add. Check the MMF2SDK Help file for
	 * more detailed information on the flags you
	 * can set and what they implicate.
	 */
	static const int OEFLAGS = OEFLAG_VALUES|OEFLAG_RUNBEFOREFADEIN|OEFLAG_NEVERKILL|OEFLAG_MOVEMENTS;
	static const int OEPREFS = 0;

	/* WindowProcPriority
	 * If your extension creates a Window
	 * and implements the WindowProc function,
	 * this is the priority for that stuff.
	 */
	static const int WindowProcPriority = 100;

	Extension(RD *rd, SerializedED *SED, createObjectInfo *COB); //defined & documented in Extension.cpp
	~Extension();                                                //defined & documented in Extension.cpp


	/* Add any data you want to store in your
	 * extension to this class (eg. what you'd
	 * normally store in the rundata). Unlike
	 * other SDKs, you can store real C++
	 * objects with constructors and destructors,
	 * without having to call them manually or
	 * store a pointer.
	 */

	stdtstring const subgroup_separator = _T("/");

	using Value = stdtstring;
	using Group = std::multimap<stdtstring /*item name*/, Value>;
	using Ini = std::multimap<stdtstring /*group name*/, Group>;

	using SearchResults = std::multiset<std::tuple<stdtstring /*group name*/, stdtstring /*item name*/, Value>>;

	enum struct RepeatMode
	{
		TakeFirst,
		TakeLast,
		Rename,
		GroupMerge,
	};

	struct Data //global data
	{
		Ini ini;
		SearchResults results;
		stdtstring defaultDirectory;
		stdtstring currentGroup;

		bool bAutoSave;
		bool CaseSensitive;
		bool EscapeCharsInValue;
		bool EscapeCharsInItem;
		bool EscapeCharsInGroups;
		bool NeverQuoteStrings;
		bool ReadOnly;
		bool StandardNewLines;
		stdtstring NewLineChar;
		stdtstring autoSavePath;
		bool AutoSaveEncrypted;
		stdtstring AutoSaveKey;
		bool AutoSaveCompressed;
		RepeatMode groupRepeatSetting;
		RepeatMode itemRepeatSetting;
		bool saveRepeats;
		bool subGroups;
		bool allowEmptyGroup;

		bool bool_CanCreateFolders;
		bool index;
		bool autoLoad;

		Data(EditData const &ed)
		{
			//TODO
		}
	};

	enum struct CallbackPhase
	{
		No,
	#ifndef RUN_ONLY
		YesNoisy, //show a warning dialog for bad code
	#endif
		YesSilent, //the warning dialog was already shown
	};

	//data that doesn't need to be global
	short dialogSettings;
	char dialogDisplay; //TODO
	stdtstring dialogTitle;
	HWND dialogCurrent;
	HWND dialogChild;
	HIMAGELIST icons; //TODO
	CallbackPhase duringCallback;

	//global data stuff
	static std::map<stdtstring /*global key*/, std::weak_ptr<Data>> gdata;
	std::shared_ptr<Data> data;


	/* Add your actions, conditions, and expressions
	 * as real class member functions here. The arguments
	 * (and return types for expressions/conditions)
	 * must match EXACTLY what you defined in the JSON.
	 * As a handy tip, all parameters will be either
	 * a pointer, an int, or a float (each only 4 bytes)
	 * and the same applies to return types. Remember to
	 * link the actions, conditions and expressions to their
	 * numeric IDs in the class constructor in Extension.cpp.
	 */

	//Actions - Defined in Actions.cpp
	void actionSetCurrentGroup(TCHAR const *group, int flag);
	void actionSetValueG(TCHAR const *item, int flag, float v); //TODO
	void actionSetStringG(TCHAR const *item, TCHAR const *v);
	void actionSetStringMD5G(TCHAR const *item, TCHAR const *v);
	void actionSaveObjectG(RunObject *o, int flag_pos, int flag_alt);
	void actionLoadObjectG(RunObject *o);
	void actionSavePositionG(TCHAR const *item, unsigned position);
	void actionSaveGlobalValuesG();
	void actionLoadGlobalValuesG();
	void actionDeleteGroupG();
	void actionDeleteItemG(TCHAR const *item);
	void actionRenameGroupG(TCHAR const *newname, int mode);
	void actionRenameItemG(TCHAR const *item, TCHAR const *newname, int flag);
	void actionMoveItemToGroupG(TCHAR const *item, TCHAR const *group, int flag);
	void actionSetValue(TCHAR const *group, TCHAR const *item, int flag, float v); //TODO
	void actionSetString(TCHAR const *group, TCHAR const *item, TCHAR const *v);
	void actionSetStringMD5(TCHAR const *group, TCHAR const *item, TCHAR const *v);
	void actionSaveObject(RunObject *o, TCHAR const *group, int flag_pos, int flag_alt);
	void actionLoadObject(RunObject *o, TCHAR const *group);
	void actionSavePosition(TCHAR const *group, TCHAR const *item, unsigned position);
	void actionSaveAltValues(TCHAR const *group);
	void actionLoadAltValues(TCHAR const *group);
	void actionRenameGroup(TCHAR const *group, TCHAR const *newname, int mode);
	void actionRenameItem(TCHAR const *group, TCHAR const *item, TCHAR const *newname, int flag);
	void actionMoveItem(TCHAR const *group, TCHAR const *item, TCHAR const *newgroup, TCHAR const *newitem, int flag);
	void actionMoveItemToGroup(TCHAR const *group, TCHAR const *item, TCHAR const *newgroup, int flag);
	void actionCopyGroup(TCHAR const *group, TCHAR const *newname, int mode);
	void actionCopyItem(TCHAR const *group, TCHAR const *item, TCHAR const *newgroup, TCHAR const *newitem);
	void actionDeleteGroup(TCHAR const *group);
	void actionDeleteItem(TCHAR const *group, TCHAR const *item);
	void actionDeleteItemInAllGroups(TCHAR const *item);
	void actionDeleteFromPattern(TCHAR const *gpatern, TCHAR const *ipattern, TCHAR const *vpattern, int flag);
	void actionClearAll();
	void actionPerformSearch(TCHAR const *gcond, TCHAR const *icond, TCHAR const *vcond, void *settings); //TODO
	void actionPerformMultiplesSearch(TCHAR const *group, TCHAR const *item);
	void actionClearResults();
	void actionCreateSubINI(TCHAR const *file, TCHAR const *gcond, TCHAR const *icond, TCHAR const *vcond, int flag);
	void actionCreateSubINIObject(void *object_name, TCHAR const *gcond, TCHAR const *icond, TCHAR const *vcond, int flag); //TODO
	void actionMergeFile(TCHAR const *file, int flag);
	void actionMergeGroupFile(TCHAR const *file, TCHAR const *filegroup, TCHAR const *group, int flag);
	void actionMerge(void *objectname, int flag); //TODO
	void actionMergeGroupObject(void *objectname, TCHAR const *objectgroup, TCHAR const *group, int flag); //TODO
	void actionNew(TCHAR const *file, int flag);
	void actionLoad(TCHAR const *file, int flag);
	void actionSave();
	void actionSaveAs(TCHAR const *file);
	void actionBackupTo(TCHAR const *file, int flag, TCHAR const *key);
	void actionClose();
	void actionLoadFromString(TCHAR const *inistr, int mode);
	void actionSetAutoSave(int flag_save, int flag_load);
	void actionSetCompresson(int flag);
	void actionSetEncryption(TCHAR const *key);
	void actionSetReadOnly(int flag);
	void actionSetCaseSensitive(int flag);
	void actionSetEscapeChars(int flag_groups, int flag_items, int flag_values);
	void actionNeverQuoteStrings(int flag);
	void actionSetRepeatModes(int flag_groups, int flag_items, int flag_repeats);
	void actionSetNewLineChar(TCHAR const *str);
	void actionSetDefaultDirectory(TCHAR const *directory);
	void actionCompressFile(TCHAR const *file_source, TCHAR const *file_dest);
	void actionDeCompressFile(TCHAR const *file_source, TCHAR const *file_dest);
	void actionOpenDialog(TCHAR const *title, void *settings); //TODO
	void actionAddRepeatedItem(TCHAR const *group, TCHAR const *item, TCHAR const *str, int mode);
	void actionCloseDialog();
	void actionRefreshDialog(int mode);
	void actionExportCVS(int io, TCHAR const *file, TCHAR const *group, TCHAR const *separator);
	void actionImportCVS(int io, TCHAR const *file, TCHAR const *separator);
	void actionToChart(void *objectname, TCHAR const *group); //TODO
	void actionFindSubGroups(TCHAR const *group, int depth);
	void actionEnableSubGroups(int flag_subgroups, int flag_empties);
	void actionSwitchGlobalObject(TCHAR const *dataslot);
	void actionSSS(void *settings, TCHAR const *group);
	void actionSetItemArray(TCHAR const *group, TCHAR const *item, int part, TCHAR const *v);
	void actionLoadFromArray(void *objectname, TCHAR const *group); //TODO
	void actionSaveToArray(void *objectname, TCHAR const *group); //TODO
	void actionFromChart(void *objectname, TCHAR const *group); //TODO
	void actionSaveChartSettings(void *objectname, TCHAR const *group); //TODO
	void actionLoadChartSettings(void *objectname, TCHAR const *group); //TODO
	void actionLoadChangeFile(TCHAR const *file, void *settings);
	void actionUndo();
	void actionRedo();
	void actionClearUndoStack(int mode);
	void actionAddNewUndoBlock();
	void actionSetManualMode(int mode);

	//Conditions - Defined in Conditions.cpp
	bool conditionHasGroupG();
	bool conditionHasItemG(TCHAR const *item);
	bool conditionCompareMD5G(TCHAR const *item, TCHAR const *def, TCHAR const *comp);
	bool conditionHasGroup(TCHAR const *group);
	bool conditionHasItem(TCHAR const *group, TCHAR const *item);
	bool conditionCompareMD5(TCHAR const *group, TCHAR const *item, TCHAR const *def, TCHAR const *comp);
	bool conditionWildcatMatch(TCHAR const *pattern, TCHAR const *str, int flag);
	bool conditionEvent(){ return true; }
	bool conditionHasUndo();
	bool conditionHasRedo();

	//Expressions - Defined in Expressions.cpp
	float expressionGetItemValueG(TCHAR const *item, float def); //TODO
	TCHAR const *expressionGetItemStringG(TCHAR const *item, TCHAR const *def);
	int expressionGetXG(TCHAR const *item);
	int expressionGetYG(TCHAR const *item);
	TCHAR const *expressionNthItemG(int n);
	float expressionNthItemValueG(int n); //TODO
	TCHAR const *expressionNthItemStringG(int n);
	int expressionItemCountG();
	float expressionGetItemValue(TCHAR const *group, TCHAR const *item, float def); //TODO
	TCHAR const *expressionGetItemString(TCHAR const *group, TCHAR const *item, TCHAR const *def);
	int expressionGetX(TCHAR const *group, TCHAR const *item);
	int expressionGetY(TCHAR const *group, TCHAR const *item);
	TCHAR const *expressionNthGroup(int n);
	TCHAR const *expressionNthItem(TCHAR const *group, int n);
	TCHAR const *expressionNthItemString(TCHAR const *group, int n);
	float expressionNthItemValue(TCHAR const *group, int n); //TODO
	int expressionGroupCount();
	int expressionItemCount(TCHAR const *group);
	int expressionTotalItems();
	int expressionSearchResultCounts();
	TCHAR const *expressionGetNthSearchResultGroup(int n);
	TCHAR const *expressionGetNthSearchResultItem(int n);
	float expressionGetNthSearchResultItemValue(int n); //TODO
	TCHAR const *expressionGetNthSearchResultItemString(int n);
	TCHAR const *expressionGetNthSearchResultPath(int n, TCHAR const *separator);
	TCHAR const *expressionWriteStream();
	TCHAR const *expressionHashString(TCHAR const *str);
	TCHAR const *expressionEncryptString(TCHAR const *, TCHAR const *); //TODO
	TCHAR const *expressionEscapeString(TCHAR const *str);
	TCHAR const *expressionUnEscapeString(TCHAR const *str);
	int expressionInnerProduct(TCHAR const *, TCHAR const *); //TODO
	TCHAR const *expressionInnerProductStr(TCHAR const *, TCHAR const *); //TODO
	TCHAR const *expressionNthSortedName(TCHAR const *group, int n);
	float expressionNthSortedValue(TCHAR const *group, int n); //TODO
	TCHAR const *expressionGetCSV(TCHAR const *, TCHAR const *); //TODO
	TCHAR const *expressionNthOverallItem(int n);
	int expressionUniqueItemCount();
	TCHAR const *expressionGetItemArray(TCHAR const *group, TCHAR const *item, int index, TCHAR const *def);
	TCHAR const *expressionCurGroup();
	TCHAR const *expressionCurGroupString();
	TCHAR const *expressionFname();


	short Handle();         //defined & documented in Extension.cpp
	short Display();        //defined & documented in Extension.cpp

	short Pause();          //defined & documented in Extension.cpp
	short Continue();       //defined & documented in Extension.cpp

	bool Save(HANDLE File); //defined & documented in Extension.cpp
	bool Load(HANDLE File); //defined & documented in Extension.cpp

	//defined & documented in Extension.cpp
	void Action(int ID, RD *rd, long param1, long param2);
	long Condition(int ID, RD *rd, long param1, long param2);
	long Expression(int ID, RD *rd, long param);
};
