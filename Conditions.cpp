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
	return hasGroup(data->currentGroup);
}

bool Extension::conditionHasItemG(TCHAR const *item)
{
	return hasItem(data->currentGroup, item);
}

bool Extension::conditionCompareMD5G(TCHAR const *item, TCHAR const *def, TCHAR const *comp)
{
	//
	return false;
}

bool Extension::conditionHasGroup(TCHAR const *group)
{
	return hasGroup(group);
}

bool Extension::conditionHasItem(TCHAR const *group, TCHAR const *item)
{
	return hasItem(group, item);
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
	return !data->undos.empty();
}

bool Extension::conditionHasRedo()
{
	return !data->redos.empty();
}
