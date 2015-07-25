/* Extension.cpp
 * This file contains the definitions for
 * your extension's general runtime functions,
 * such as the constructor and destructor,
 * handling routines, etc.
 * Functions defined here:
 * Extension::Extension <constructor>
 * Extension::~Extension <destructor>
 * Extension::Handle
 * Extension::Display
 * Extension::Pause
 * Extension::Continue
 * Extension::Save
 * Extension::Load
 * Extension::Action		<--|
 * Extension::Condition		<--|- not what you think!
 * Extension::Expression	<--|
 */

#include "Common.h"

std::map<stdtstring, std::weak_ptr<Extension::Data>> Extension::gdata;

/* <constructor>
 * This is your extension's constructor, which
 * is the replacement for the old CreateRunObject
 * function. You don't need to manually call
 * constructors or pointlessly initialize
 * pointers with dynamic memory. Just link
 * your A/C/Es, perform initialization steps, and
 * you're good to go.
 */
Extension::Extension(RD *rd, SerializedED *SED, createObjectInfo *COB)
: rd(rd)
, rh(rd->rHo.hoAdRunHeader)
, Runtime(rd)
{
	//Link all your action/condition/expression functions
	//to their IDs to match the IDs in the JSON here.
	LinkAction(0, actionSetCurrentGroup);
	LinkAction(1, actionSetValueG);
	LinkAction(2, actionSetStringG);
	LinkAction(3, actionSetStringMD5G);
	LinkAction(4, actionSaveObjectG);
	LinkAction(5, actionLoadObjectG);
	LinkAction(6, actionSavePositionG);
	LinkAction(7, actionSaveGlobalValuesG);
	LinkAction(8, actionLoadGlobalValuesG);
	LinkAction(9, actionDeleteGroupG);
	LinkAction(10, actionDeleteItemG);
	LinkAction(11, actionRenameGroupG);
	LinkAction(12, actionRenameItemG);
	LinkAction(13, actionMoveItemToGroupG);
	LinkAction(14, actionSetValue);
	LinkAction(15, actionSetString);
	LinkAction(16, actionSetStringMD5);
	LinkAction(17, actionSaveObject);
	LinkAction(18, actionLoadObject);
	LinkAction(19, actionSavePosition);
	LinkAction(20, actionSaveAltValues);
	LinkAction(21, actionLoadAltValues);
	LinkAction(22, actionRenameGroup);
	LinkAction(23, actionRenameItem);
	LinkAction(24, actionMoveItem);
	LinkAction(25, actionMoveItemToGroup);
	LinkAction(26, actionCopyGroup);
	LinkAction(27, actionCopyItem);
	LinkAction(28, actionDeleteGroup);
	LinkAction(29, actionDeleteItem);
	LinkAction(30, actionDeleteItemInAllGroups);
	LinkAction(31, actionDeleteFromPattern);
	LinkAction(32, actionClearAll);
	LinkAction(33, actionPerformSearch);
	LinkAction(34, actionPerformMultiplesSearch);
	LinkAction(35, actionClearResults);
	LinkAction(36, actionCreateSubINI);
	LinkAction(37, actionCreateSubINIObject);
	LinkAction(38, actionMergeFile);
	LinkAction(39, actionMergeGroupFile);
	LinkAction(40, actionMerge);
	LinkAction(41, actionMergeGroupObject);
	LinkAction(42, actionNew);
	LinkAction(43, actionLoad);
	LinkAction(44, actionSave);
	LinkAction(45, actionSaveAs);
	LinkAction(46, actionBackupTo);
	LinkAction(47, actionClose);
	LinkAction(48, actionLoadFromString);
	LinkAction(49, actionSetAutoSave);
	LinkAction(50, actionSetCompresson);
	LinkAction(51, actionSetEncryption);
	LinkAction(52, actionSetReadOnly);
	LinkAction(53, actionSetCaseSensitive);
	LinkAction(54, actionSetEscapeChars);
	LinkAction(55, actionNeverQuoteStrings);
	LinkAction(56, actionSetRepeatModes);
	LinkAction(57, actionSetNewLineChar);
	LinkAction(58, actionSetDefaultDirectory);
	LinkAction(59, actionCompressFile);
	LinkAction(60, actionDeCompressFile);
	LinkAction(61, actionOpenDialog);
	LinkAction(62, actionAddRepeatedItem);
	LinkAction(63, actionCloseDialog);
	LinkAction(64, actionRefreshDialog);
	LinkAction(65, actionExportCVS);
	LinkAction(66, actionImportCVS);
	LinkAction(67, actionToChart);
	LinkAction(68, actionFindSubGroups);
	LinkAction(69, actionEnableSubGroups);
	LinkAction(70, actionSwitchGlobalObject);
	LinkAction(71, actionSSS);
	LinkAction(72, actionSetItemArray);
	LinkAction(73, actionLoadFromArray);
	LinkAction(74, actionSaveToArray);
	LinkAction(75, actionFromChart);
	LinkAction(76, actionSaveChartSettings);
	LinkAction(77, actionLoadChartSettings);
	LinkAction(78, actionLoadChangeFile);
	LinkAction(79, actionUndo);
	LinkAction(80, actionRedo);
	LinkAction(81, actionClearUndoStack);
	LinkAction(82, actionAddNewUndoBlock);
	LinkAction(83, actionSetManualMode);

	LinkCondition(0, conditionHasGroupG);
	LinkCondition(1, conditionHasItemG);
	LinkCondition(2, conditionCompareMD5G);
	LinkCondition(3, conditionHasGroup);
	LinkCondition(4, conditionHasItem);
	LinkCondition(5, conditionCompareMD5);
	LinkCondition(6, conditionWildcatMatch);
	LinkCondition(7, conditionEvent);
	LinkCondition(8, conditionEvent);
	LinkCondition(9, conditionEvent);
	LinkCondition(10, conditionEvent);
	LinkCondition(11, conditionEvent);
	LinkCondition(12, conditionEvent);
	LinkCondition(13, conditionEvent);
	LinkCondition(14, conditionEvent);
	LinkCondition(15, conditionEvent);
	LinkCondition(16, conditionEvent);
	LinkCondition(17, conditionEvent);
	LinkCondition(18, conditionHasUndo);
	LinkCondition(19, conditionHasRedo);

	LinkExpression(0, expressionGetItemValueG);
	LinkExpression(1, expressionGetItemStringG);
	LinkExpression(2, expressionGetXG);
	LinkExpression(3, expressionGetYG);
	LinkExpression(4, expressionNthItemG);
	LinkExpression(5, expressionNthItemValueG);
	LinkExpression(6, expressionNthItemStringG);
	LinkExpression(7, expressionItemCountG);
	LinkExpression(8, expressionGetItemValue);
	LinkExpression(9, expressionGetItemString);
	LinkExpression(10, expressionGetX);
	LinkExpression(11, expressionGetY);
	LinkExpression(12, expressionNthGroup);
	LinkExpression(13, expressionNthItem);
	LinkExpression(14, expressionNthItemString);
	LinkExpression(15, expressionNthItemValue);
	LinkExpression(16, expressionGroupCount);
	LinkExpression(17, expressionItemCount);
	LinkExpression(18, expressionTotalItems);
	LinkExpression(19, expressionSearchResultCounts);
	LinkExpression(20, expressionGetNthSearchResultGroup);
	LinkExpression(21, expressionGetNthSearchResultItem);
	LinkExpression(22, expressionGetNthSearchResultItemValue);
	LinkExpression(23, expressionGetNthSearchResultItemString);
	LinkExpression(24, expressionGetNthSearchResultPath);
	LinkExpression(25, expressionWriteStream);
	LinkExpression(26, expressionHashString);
	LinkExpression(27, expressionEncryptString);
	LinkExpression(28, expressionEscapeString);
	LinkExpression(29, expressionUnEscapeString);
	LinkExpression(30, expressionInnerProduct);
	LinkExpression(31, expressionInnerProductStr);
	LinkExpression(32, expressionNthSortedName);
	LinkExpression(33, expressionNthSortedValue);
	LinkExpression(34, expressionGetCSV);
	LinkExpression(35, expressionNthOverallItem);
	LinkExpression(36, expressionUniqueItemCount);
	LinkExpression(37, expressionGetItemArray);
	LinkExpression(38, expressionCurGroup);
	LinkExpression(39, expressionCurGroupString);
	LinkExpression(40, expressionFname);


	//Load settings from editdata and link to global data
	EditData ed (SED);
	auto s = std::make_shared<Data>(ed);
	if(ed.globalObject)
	{
		if(gdata.find(ed.globalKey) == std::end(gdata) || gdata.at(ed.globalKey).expired())
		{
			gdata.emplace(ed.globalKey, s);
		}
		s = gdata.at(ed.globalKey).lock();
	}
	data = std::move(s);
}

/* <destructor>
 * This is your extension's destructor, the
 * replacement for DestroyRunObject. No calling
 * destructors manually or deallocating pointless
 * dynamic memory - in most cases this function
 * won't need any code written.
 */
Extension::~Extension()
{
}

/* Handle
 * MMF2 calls this function to let your extension
 * "live" - if you want, you can have MMF2 call this
 * every frame. This is where you'd, for instance,
 * simulate physics or move an object. This is
 * the analagous function to the old HandleRunObject.
 */
short Extension::Handle()
{
	/*
	If your extension will draw to the MMF window you should first 
	check if anything about its display has changed :

		if (rd->roc.rcChanged) return REFLAG_DISPLAY;
		else return 0;

	You will also need to make sure you change this flag yourself 
	to 1 whenever you want to redraw your object

	If your extension won't draw to the window, but it still needs 
	to do something every MMF2 loop use:

		return 0;

	If you don't need to do something every loop, use :

		return REFLAG_ONESHOT;

	This doesn't mean this function can never run again. If you want MMF2
	to handle your object again (causing this code to run) use this function:

		Runtime.Rehandle();

	At the end of the event loop this code will run.
	*/

	//Will not be called next loop	
	return REFLAG_ONESHOT;
}

/* Display
 * This is the analagous function to
 * DisplayRunObject. If you return
 * REFLAG_DISPLAY in Handle() this
 * routine will run. If you want MMF2
 * to apply ink effects for you, then
 * implement GetRunObjectSurface in
 * Runtime.cpp instead.
 */
short Extension::Display()
{
	return 0;
}

/* Pause
 * If your extension plays sound, for
 * example, then MMF2 calls this to
 * let you know to pause the music,
 * usually by another extension's request
 * or by the player pausing the applcation.
 */
short Extension::Pause()
{
	return 0;
}

/* Continue
 * Opposite to the above, MMF2 lets
 * you know that the silence is over;
 * your extension may live again.
 */
short Extension::Continue()
{
	return 0;
}

/* Save
 * When the user uses the Save
 * Frame Position action, you need
 * so serialize your runtime data to
 * the File given. It is a Windows
 * file handle, but you can use some
 * of MMF2's built-in functions for
 * writing files. Check the MMF2SDK
 * Help file for more information.
 */
bool Extension::Save(HANDLE File)
{
	return true;
}

/* Load
 * As opposed to above, here you need to
 * restore your extension's runtime state
 * from the given file. Only read what you
 * wrote!
 */
bool Extension::Load(HANDLE File)
{
	return true;
}


/* Action, Condition, Expression
 * These are called if there's no function linked
 * to an ID. You may want to put MessageBox calls
 * to let you know that the ID is unlinked, or you
 * may just want to use unlinked A/C/Es as a feature.
 */
void Extension::Action(int ID, RD *rd, long param1, long param2)
{
}

long Extension::Condition(int ID, RD *rd, long param1, long param2)
{
	return false; //hopefully StringComparison (PARAM_CMPSTRING) is not used, or this may crash
}

long Extension::Expression(int ID, RD *rd, long param)
{
	return long(_T("")); //so that unlinked expressions that return strings won't crash
}
