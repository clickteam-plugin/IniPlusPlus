/* Expressions.cpp
 * This is where you should define the
 * behavior of your expressions. Make sure
 * the parameters and return types match those
 * in the JSON exactly! Double check Extension.h
 * as well.
 */

#include "Common.h"

float Extension::expressionGetItemValueG(TCHAR const *item, float def) //TODO
{
	if(hasItem(data->currentGroup, item))
	{
		std::basic_istringstream<TCHAR> iss{valueByName(data->currentGroup, item)};
		float f = 0.0f;
		iss >> f;
		return f;
	}
	return def;
	return 0.0f;
}

TCHAR const *Extension::expressionGetItemStringG(TCHAR const *item, TCHAR const *def)
{
	if(hasItem(data->currentGroup, item))
	{
		return Runtime.CopyString(valueByName(data->currentGroup, item).c_str());
	}
	return Runtime.CopyString(def);
}

int Extension::expressionGetXG(TCHAR const *item)
{
	//
	return 0;
}

int Extension::expressionGetYG(TCHAR const *item)
{
	//
	return 0;
}

TCHAR const *Extension::expressionNthItemG(int n)
{
	//
	return _T("");
}

float Extension::expressionNthItemValueG(int n) //TODO
{
	//
	return 0.0f;
}

TCHAR const *Extension::expressionNthItemStringG(int n)
{
	//
	return _T("");
}

int Extension::expressionItemCountG()
{
	//
	return 0;
}

float Extension::expressionGetItemValue(TCHAR const *group, TCHAR const *item, float def) //TODO
{
	if(hasItem(group, item))
	{
		std::basic_istringstream<TCHAR> iss {valueByName(group, item)};
		float f = 0.0f;
		iss >> f;
		return f;
	}
	return def;
}

TCHAR const *Extension::expressionGetItemString(TCHAR const *group, TCHAR const *item, TCHAR const *def)
{
	if(hasItem(group, item))
	{
		return Runtime.CopyString(valueByName(group, item).c_str());
	}
	return Runtime.CopyString(def);
}

int Extension::expressionGetX(TCHAR const *group, TCHAR const *item)
{
	//
	return 0;
}

int Extension::expressionGetY(TCHAR const *group, TCHAR const *item)
{
	//
	return 0;
}

TCHAR const *Extension::expressionNthGroup(int n)
{
	if(n >= 0 && n < data->ini.size())
	{
		auto it = data->ini.begin();
		std::advance(it, n);
		return Runtime.CopyString(it->first.c_str());
	}
	return _T("");
}

TCHAR const *Extension::expressionNthItem(TCHAR const *group, int n)
{
	if(hasGroup(group))
	{
		auto g = groupByName(group);
		if(n >= 0 && n < g.size())
		{
			auto it = g.begin();
			std::advance(it, n);
			return Runtime.CopyString(it->first.c_str());
		}
	}
	return _T("");
}

TCHAR const *Extension::expressionNthItemString(TCHAR const *group, int n)
{
	if(hasGroup(group))
	{
		auto g = groupByName(group);
		if(n >= 0 && n < g.size())
		{
			auto it = g.begin();
			std::advance(it, n);
			return Runtime.CopyString(it->second.c_str());
		}
	}
	return _T("");
}

float Extension::expressionNthItemValue(TCHAR const *group, int n) //TODO
{
	if(hasGroup(group))
	{
		auto g = groupByName(group);
		if(n >= 0 && n < g.size())
		{
			auto it = g.begin();
			std::advance(it, n);
			return expressionGetItemValue(group, it->first.c_str(), 0.0f);
		}
	}
	return 0.0f;
}

int Extension::expressionGroupCount()
{
	return data->ini.size();
}

int Extension::expressionItemCount(TCHAR const *group)
{
	if(hasGroup(group))
	{
		return groupByName(group).size();
	}
	return 0;
}

int Extension::expressionTotalItems()
{
	std::size_t n = 0;
	for(auto const &group : data->ini)
	{
		n += group.second.size();
	}
	return n;
}

int Extension::expressionSearchResultCounts()
{
	//
	return 0;
}

TCHAR const *Extension::expressionGetNthSearchResultGroup(int n)
{
	//
	return _T("");
}

TCHAR const *Extension::expressionGetNthSearchResultItem(int n)
{
	//
	return _T("");
}

float Extension::expressionGetNthSearchResultItemValue(int n) //TODO
{
	//
	return 0.0f;
}

TCHAR const *Extension::expressionGetNthSearchResultItemString(int n)
{
	//
	return _T("");
}

TCHAR const *Extension::expressionGetNthSearchResultPath(int n, TCHAR const *separator)
{
	//
	return _T("");
}

TCHAR const *Extension::expressionWriteStream()
{
	std::basic_ostringstream<TCHAR> oss;
	saveIni(oss);
	return Runtime.CopyString(oss.str().c_str());
}

TCHAR const *Extension::expressionHashString(TCHAR const *str)
{
	//
	return _T("");
}

TCHAR const *Extension::expressionEncryptString(TCHAR const *, TCHAR const *) //TODO
{
	//
	return _T("");
}

TCHAR const *Extension::expressionEscapeString(TCHAR const *str)
{
	//
	return _T("");
}

TCHAR const *Extension::expressionUnEscapeString(TCHAR const *str)
{
	//
	return _T("");
}

int Extension::expressionInnerProduct(TCHAR const *, TCHAR const *) //TODO
{
	//
	return 0;
}

TCHAR const *Extension::expressionInnerProductStr(TCHAR const *, TCHAR const *) //TODO
{
	//
	return _T("");
}

TCHAR const *Extension::expressionNthSortedName(TCHAR const *group, int n)
{
	//
	return _T("");
}

float Extension::expressionNthSortedValue(TCHAR const *group, int n) //TODO
{
	//
	return 0.0f;
}

TCHAR const *Extension::expressionGetCSV(TCHAR const *, TCHAR const *) //TODO
{
	//
	return _T("");
}

TCHAR const *Extension::expressionNthOverallItem(int n)
{
	//
	return _T("");
}

int Extension::expressionUniqueItemCount()
{
	//
	return 0;
}

TCHAR const *Extension::expressionGetItemArray(TCHAR const *group, TCHAR const *item, int index, TCHAR const *def)
{
	//
	return _T("");
}

TCHAR const *Extension::expressionCurGroup()
{
	//
	return _T("");
}

TCHAR const *Extension::expressionCurGroupString()
{
	//
	return _T("");
}

TCHAR const *Extension::expressionFname()
{
	//
	return _T("");
}

TCHAR const *Extension::expressionErrorMsg()
{
	return Runtime.CopyString(error_msg.c_str());
}
