/* Conditions.cpp
 * This is where you should define the
 * behavior of your conditions. Make sure
 * the parameters and return types match those
 * in the JSON exactly! Double check Extension.h
 * as well.
 */

#include "Common.h"

bool Extension::conditionHasGroupG()
{
	return data->ini.find(data->currentGroup) != std::end(data->ini);
}

bool Extension::conditionHasItemG(TCHAR const *item)
{
	return conditionHasGroupG() && currentGroup().find(item) != std::end(currentGroup());
}

bool Extension::conditionCompareMD5G(TCHAR const *item, TCHAR const *def, TCHAR const *comp)
{
	//
	return false;
}

bool Extension::conditionHasGroup(TCHAR const *group)
{
	return data->ini.find(group) != std::end(data->ini);
}

bool Extension::conditionHasItem(TCHAR const *group, TCHAR const *item)
{
	return conditionHasGroup(group) && groupByName(group).find(item) != std::end(groupByName(group));
}

bool Extension::conditionCompareMD5(TCHAR const *group, TCHAR const *item, TCHAR const *def, TCHAR const *comp)
{
	//
	return false;
}

bool Extension::conditionWildcatMatch(TCHAR const *pattern, TCHAR const *str, int flag)
{
	//
	return false;
}

bool Extension::conditionHasUndo()
{
	return !undos.empty();
}

bool Extension::conditionHasRedo()
{
	return !redos.empty();
}
