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
	static const int MinimumBuild = 258;

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

	//stdtstring MyString;
	//int MyInt;
	//std::vector<float> MyArray;


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
	void actionSetValueG(/**/);
	void actionSetStringG(/**/);
	void actionSetStringMD5G(/**/);
	void actionSaveObjectG(/**/);
	void actionLoadObjectG(/**/);
	void actionSavePositionG(/**/);
	void actionSaveGlobalValuesG(/**/);
	void actionLoadGlobalValuesG(/**/);
	void actionDeleteGroupG(/**/);
	void actionDeleteItemG(/**/);
	void actionRenameGroupG(/**/);
	void actionRenameItemG(/**/);
	void actionMoveItemToGroupG(/**/);
	void actionSetValue(/**/);
	void actionSetString(/**/);
	void actionSetStringMD5(/**/);
	void actionSaveObject(/**/);
	void actionLoadObject(/**/);
	void actionSavePosition(/**/);
	void actionSaveAltValues(/**/);
	void actionLoadAltValues(/**/);
	void actionRenameGroup(/**/);
	void actionRenameItem(/**/);
	void actionMoveItem(/**/);
	void actionMoveItemToGroup(/**/);
	void actionCopyGroup(/**/);
	void actionCopyItem(/**/);
	void actionDeleteGroup(/**/);
	void actionDeleteItem(/**/);
	void actionDeleteItemInAllGroups(/**/);
	void actionDeleteFromPattern(/**/);
	void actionClearAll(/**/);
	void actionPerformSearch(/**/);
	void actionPerformMultiplesSearch(/**/);
	void actionClearResults(/**/);
	void actionCreateSubINI(/**/);
	void actionCreateSubINIObject(/**/);
	void actionMergeFile(/**/);
	void actionMergeGroupFile(/**/);
	void actionMerge(/**/);
	void actionMergeGroupObject(/**/);
	void actionNew(/**/);
	void actionLoad(/**/);
	void actionSave(/**/);
	void actionSaveAs(/**/);
	void actionBackupTo(/**/);
	void actionClose(/**/);
	void actionLoadFromString(/**/);
	void actionSetAutoSave(/**/);
	void actionSetCompresson(/**/);
	void actionSetEncryption(/**/);
	void actionSetReadOnly(/**/);
	void actionSetCaseSensitive(/**/);
	void actionSetEscapeChars(/**/);
	void actionNeverQuoteStrings(/**/);
	void actionSetRepeatModes(/**/);
	void actionSetNewLineChar(/**/);
	void actionSetDefaultDirectory(/**/);
	void actionCompressFile(/**/);
	void actionDeCompressFile(/**/);
	void actionOpenDialog(/**/);
	void actionAddRepeatedItem(/**/);
	void actionCloseDialog(/**/);
	void actionRefreshDialog(/**/);
	void actionExportCVS(/**/);
	void actionImportCVS(/**/);
	void actionToChart(/**/);
	void actionFindSubGroups(/**/);
	void actionEnableSubGroups(/**/);
	void actionSwitchGlobalObject(/**/);
	void actionSSS(/**/);
	void actionSetItemArray(/**/);
	void actionLoadFromArray(/**/);
	void actionSaveToArray(/**/);
	void actionFromChart(/**/);
	void actionSaveChartSettings(/**/);
	void actionLoadChartSettings(/**/);
	void actionLoadChangeFile(/**/);
	void actionUndo(/**/);
	void actionRedo(/**/);
	void actionClearUndoStack(/**/);
	void actionAddNewUndoBlock(/**/);
	void actionSetManualMode(/**/);

	//Conditions - Defined in Conditions.cpp
	bool conditionHasGroupG(/**/);
	bool conditionHasItemG(/**/);
	bool conditionCompareMD5G(/**/);
	bool conditionHasGroup(/**/);
	bool conditionHasItem(/**/);
	bool conditionCompareMD5(/**/);
	bool conditionWildcatMatch(/**/);
	bool conditionEvent(){ return true; }
	bool conditionHasUndo(/**/);
	bool conditionHasRedo(/**/);

	//Expressions - Defined in Expressions.cpp
	void expressionGetItemValueG(/**/);
	void expressionGetItemStringG(/**/);
	void expressionGetXG(/**/);
	void expressionGetYG(/**/);
	void expressionNthItemG(/**/);
	void expressionNthItemValueG(/**/);
	void expressionNthItemStringG(/**/);
	void expressionItemCountG(/**/);
	void expressionGetItemValue(/**/);
	void expressionGetItemString(/**/);
	void expressionGetX(/**/);
	void expressionGetY(/**/);
	void expressionNthGroup(/**/);
	void expressionNthItem(/**/);
	void expressionNthItemString(/**/);
	void expressionNthItemValue(/**/);
	void expressionGroupCount(/**/);
	void expressionItemCount(/**/);
	void expressionTotalItems(/**/);
	void expressionSearchResultCounts(/**/);
	void expressionGetNthSearchResultGroup(/**/);
	void expressionGetNthSearchResultItem(/**/);
	void expressionGetNthSearchResultItemValue(/**/);
	void expressionGetNthSearchResultItemString(/**/);
	void expressionGetNthSearchResultPath(/**/);
	void expressionWriteStream(/**/);
	void expressionHashString(/**/);
	void expressionEncryptString(/**/);
	void expressionEscapeString(/**/);
	void expressionUnEscapeString(/**/);
	void expressionInnerProduct(/**/);
	void expressionInnerProductStr(/**/);
	void expressionNthSortedName(/**/);
	void expressionNthSortedValue(/**/);
	void expressionGetCSV(/**/);
	void expressionNthOverallItem(/**/);
	void expressionUniqueItemCount(/**/);
	void expressionGetItemArray(/**/);
	void expressionCurGroup(/**/);
	void expressionCurGroupString(/**/);
	void expressionFname(/**/);


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
