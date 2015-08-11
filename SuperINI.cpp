
#include "common.h"
#include "SuperINI.h"
#include "Alphanum.hpp"

//==============================
// Used in the searching code
//==============================
bool __inline IsNumber( const std::string& x )
{
	for ( int n = 0 ; n < x.length() ; n++ )
		if (!isdigit(x[n]) && !(n==0 && x[0]=='-'))
			return false;
	return true;
}

std::string lcretALWAYS(std::string x )
{
	for ( int n = 0 ; n < x.length() ; n++ )
	{
		x[n] = tolower(x[n]);
	}
	return x;
}


//mode:
//		0		Usual (non-case sensitive)
//		1		Usual (case sensitive)
//		2		Alphanum
int Ini_Item_Compare( const std::string& a , const std::string & b , int mode )
{
	if ( mode == 0 )
	{
		int first = _stricmp( a.c_str() , b.c_str() );
		if ( first != 0 ) return first;
		return strcmp( a.c_str() , b.c_str() );
	}
	if ( mode == 1 )
	{
		return strcmp( a.c_str() , b.c_str() );
	}
	if ( mode == 2 )
	{
		return doj::alphanum_comp(lcretALWAYS(a).c_str(),lcretALWAYS(b).c_str());
	}
	return 0;
}


//Never case sensitive!
bool superini_wildcmp(const char *wild, const char *string, bool caseSensitive) {

	if ( strcmp(wild,"*") == 0 ) return true;

  // Written by Jack Handy - jakkhandy@hotmail.com
  const char *cp = NULL, *mp = NULL;

	bool notMode = false;
	if ( *wild == '!' )
	{
		wild++;
		notMode = true;
	}

  while ((*string) && (*wild != '*')) {

	if ( caseSensitive )
	{
	  if (( (*wild) != (*string)) && (*wild != '.'))
	  {
	    return notMode;
	  }
	}
	else
	{
	   if ((tolower(*wild) != tolower(*string)) && (*wild != '.'))
	   {
			return notMode;
	   }
	}
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return !notMode;
      }
      mp = wild;
      cp = string+1;
    } 
	else 
		if	(
			(*wild == '.') || ( caseSensitive && (*wild == *string) )
			|| ( !caseSensitive && ( tolower(*wild) == tolower(*string) ) )
			)
		{
			wild++;
			string++;
		} 
		else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild ^ notMode;
}


#define DEBUGOUTSTD(str) {unsigned long disregard; HANDLE la; la = GetStdHandle(STD_OUTPUT_HANDLE); WriteConsole( la , ((std::string)(str)).c_str() , ((std::string)(str)).length() , &disregard , 0 );}

void SuperINI_Crypt(char *inp, unsigned long inplen, const char* key = "", unsigned long keylen = 0)
{
	if ( key[0] == 0 )
		return;
	
    //we will consider size of sbox 256 bytes
    //(extra byte are only to prevent any mishep just in case)
    char Sbox[257], Sbox2[257];
    unsigned long i, j, t, x;

    //this unsecured key is to be used only when there is no input key from user
 
    char temp , k;
    i = j = k = t =  x = 0;
    temp = 0;

    //initialize sbox i
    for(i = 0; i < 256U; i++)
    {
        Sbox[i] = (char)i;
		Sbox2[i] = 0;
    }

    j = 0;
    //whether user has sent any inpur key
    if(keylen)
    {
        //initialize the sbox2 with user key
        for(i = 0; i < 256U ; i++)
        {
            if(j == keylen)
            {
                j = 0;
            }
            Sbox2[i] = key[j++];
        }    
    }
 
    j = 0 ; //Initialize j
    //scramble sbox1 with sbox2
    for(i = 0; i < 256; i++)
    {
        j = (j + (unsigned long) Sbox[i] + (unsigned long) Sbox2[i]) % 256U ;
        temp =  Sbox[i];                    
        Sbox[i] = Sbox[j];
        Sbox[j] =  temp;
    }

    i = j = 0;
    for(x = 0; x < inplen; x++)
    {
        //increment i
        i = (i + 1U) % 256U;
        //increment j
        j = (j + (unsigned long) Sbox[i]) % 256U;

        //Scramble SBox #1 further so encryption routine will
        //will repeat itself at great interval
        temp = Sbox[i];
        Sbox[i] = Sbox[j] ;
        Sbox[j] = temp;

        //Get ready to create pseudo random  byte for encryption key
        t = ((unsigned long) Sbox[i] + (unsigned long) Sbox[j]) %  256U ;

        //get the random byte
        k = Sbox[t];

        //xor with the data and done
		char pre = inp[x];

        inp[x] = (inp[x] ^ k);


		if ( x > inplen - 20 ) {
		std::string str2("A -> B");
		str2[0] = pre;
		str2[5] = inp[x];

		char num[30]; _itoa(x,num,10);

		str2 = "Line " + std::string(num) +" = " + str2 + "\n";
		DEBUGOUTSTD( str2 );
		}

    } 
	
}





DynamicArray SplitByComma( std::string what )
{
	DynamicArray out;
	int n = 0;
	std::string cur = "";

	while ( n < what.length() )
	{
		if ( what[n] != ',' )
			cur += what[n];
		else
		{
			out.push_back(cur);
			cur = "";		
		}

		n++;
	}
	out.push_back(cur);
	return out;
}

SuperINI::SuperINI()
{
	//MMF ONLY:
	cb = 0;
	inRedo = false;
	useRedoStack = false;
	undoLimit = redoLimit = 0;
	ResetSettings();

	return;
}


void SuperINI::ResetSettings()
{
	rdPtrFavorite = 0;

	bAutoSave = true;
	bAutoSavePaused = false;
	CaseSensitive = false;
	ReadOnly = false;

	EscapeCharsInValue = false;	//Non-standard but no side effects
	EscapeCharsInItem = false;	//Non-standard, but better to have
	EscapeCharsInGroups = false;	//Non-standard, but you may as well!
	NeverQuoteStrings = false;
	StandardNewLines = true;
	SetStandardNewLines();

	groupRepeatSetting = 3; //Merge
	itemRepeatSetting = 1; //Rename
	saveRepeats = false;
	subGroups = false;

	AutoSaveCompressed = false;
	AutoSaveEncrypted = false;
	AutoSaveKey = "";
	autoSavePath = "";
	allowEmptyGroup = true;
	autoAddUndoStage = true;
}

//Internal functions to do with saving
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool SuperINI::PauseAutoSave( bool val )
{
	bAutoSavePaused = val;
	return true;
}
bool SuperINI::RequestAutoSave()
{

	if ( cb != NULL && !bAutoSavePaused)
	{
		for ( int n = 0 ; n != rdPtr.size() ; n++ )
			cb( rdPtr[n] , 1 );
	}
	if ( bAutoSave && !bAutoSavePaused && !ReadOnly )
	{
		return Save();
	}
	return true;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void SuperINI::lc( std::string& x)
{
	if ( !CaseSensitive )
		for ( int n = 0 ; n < x.length() ; n++ )
		{
			x[n] = tolower(x[n]);
		}
}
std::string SuperINI::lcret( std::string x )
{
	if ( !CaseSensitive )
		return lcretALWAYS(x);
	else return x;
}
/*
std::string SuperINI::lcretALWAYS(std::string x )
{
	for ( int n = 0 ; n < x.length() ; n++ )
	{
		x[n] = tolower(x[n]);
	}
	return x;
}
*/


std::string SuperINI::toString( int x )
{
	char buff[20];
	_itoa( x , buff , 10 );
	return std::string(buff);
}

//Search class
/*struct SearchResults
{
	vector< Triple >	results;
}*/

//mode:		0		Copy (No check)
//			1		Or (Copy only if not exist)
void SearchResults::AddItem( std::string a, std::string b, std::string c , int mode )
{
	if ( (mode == 0) || (results.find( Triple(a,b,c) ) == -1) )
		results.push_back( Triple(a,b,c) );
	return;
}
int SearchResults::CountItems(){
	return results.size();
}
void SearchResults::ClearAll(){
	results.clear();
	return;
}
// ~~ which ~~
//	0	group
//	1	item
//	2	value
std::string SearchResults::GetItem( int n , int which )
{
	if ( n < 0 || n >= CountItems() )
		return "";
	if ( which == 0 )
		return results[n].grp;
	if ( which == 1 )
		return results[n].itm;
	if ( which == 2 )
		return results[n].val;
	return "";
}
//mode	2	And
//		3	Xor
bool SearchResults::OperatorWith(SearchResults* what, int mode)
{
	//AND
	if ( mode == 2 )
	{
		for ( int n = 0 ; n < CountItems() ; n++ )
		{
				if (! what->HasItem(results[n]) )
				{
					results.removeAt(n);
					n--;
				}
		}
	}

	if ( mode != 2 ) //Thus is 3, and XOR.
	{
		int n;
		DynamicArrayGeneral< Triple > rejected;
		for ( n = 0 ; n < CountItems() ; n++ )
		{
			if ( what->HasItem(results[n]) )
			{
				rejected.push_back( results[n] );
				results.removeAt(n);
				n--;
			}
		}
	
		for ( n = 0 ; n < what->CountItems() ; n++ )
		{
			Triple d = what->GetItemDirect(n);
			if ( rejected.find(d) == -1 )
				AddItem( d );
		}
	}
	
	return true;
}


//================================================
//		Basic Access
//================================================

void SuperINI::ForceGroupExist( std::string grp )
{
	if ( HasGroup( grp ) )
		return;
	pushUndo( new UndoGroupMake(grp) );
	std::string groupKey = lcret(grp);
	data.insert( INIMap::value_type( groupKey , SMapContainer(grp) ) );
	ordering.push_back(groupKey);
}


void SuperINI::pushUndo( UndoElement* element )
{
	
	if ( !useRedoStack ) {
		if ( undoLimit != 0 ) {
			undoInformation.back()->add( element );
			return;
		}
	}
	else
		if ( redoLimit != 0 ) {
			redoInformation.back()->add( element );
			return;
		}

	delete element;
}
void SuperINI::newUndoBlock()
{

	if ( !useRedoStack ) {
		if ( !inRedo )
			ClearStacks(false,true);
		if ( undoLimit != 0 && (undoInformation.empty() || (autoAddUndoStage && !undoInformation.back()->empty()))  )
			undoInformation.push_back( new UndoCompound() );
	} else {
		if ( redoLimit != 0 && (redoInformation.empty() || !redoInformation.back()->empty())  )
			redoInformation.push_back( new UndoCompound() );
	}

	while ( undoLimit > 0 && undoInformation.size() > undoLimit ) {
		delete undoInformation.front();
		undoInformation.pop_front();
	}
	while ( redoLimit > 0 && redoInformation.size() > redoLimit ) {
		delete redoInformation.front();
		redoInformation.pop_front();
	}
}

void SuperINI::forceNewUndoBlock() {
	undoInformation.push_back( new UndoCompound() );
}

void SuperINI::ClearStacks( bool undo , bool redo ) {
	
	if ( undo ) {
		while ( !undoInformation.empty() ) {
			delete undoInformation.front();
			undoInformation.pop_front();
		}
	}
	if ( redo ) {
		while (  !redoInformation.empty() ) {
			delete redoInformation.front();
			redoInformation.pop_front();
		}
	}

}

//Return value: Previously exist?
bool SuperINI::SetItem( std::string group , std::string item , std::string value )
{
	std::string groupKey = lcret(group);
	std::string itemKey = lcret(item);

	ForceGroupExist(group);
	INIMap::iterator groupIter;

	groupIter = data.find( groupKey );
	groupIter->second.name = group;
	SMap* groupMap = &((groupIter->second).map);
	//Does the item exist?


	if ( !HasItem(group,item) )
	{
		pushUndo( static_cast<UndoElement*>(new UndoCreateItem(group,item)) );

		groupMap->insert( SMap::value_type( itemKey , SuperItem(item,value) ) );
		
		(groupIter->second).ordering->push_back(itemKey);
		//MMF ONLY
		if ( cb != NULL )
		{
			for ( int n = 0 ; n != rdPtr.size() ; n++ )
				cb( rdPtr[n] , 4 );
		}
	}
	else
	{
		//if ( !bAutoSavePaused )
		pushUndo( ((UndoElement*)new UndoSetItem(group,item,groupMap->find(itemKey)->second.val)) );
		groupMap->find( itemKey)->second.name = item;
		groupMap->find( itemKey)->second.val = value;
	}

	RequestAutoSave();
	return true;

}

bool SuperINI::SetItemArray(std::string grp , std::string itm , int part , std::string val )
{
	if ( part < 0 ) return false;
	std::string str = GetItem(grp,itm,"");

	DynamicArray s = SplitByComma(str);
	s.EnforceSize(part);
	s.set(part , val);
	
	std::string x = "";
	for ( int n = 0 ; n < s.size()-1 ; n++ )
		x += s[n] + ",";
	x += s[s.size()-1];
	SetItem(grp,itm,x);
	return true;
}
std::string SuperINI::GetItemArray(std::string grp , std::string itm , int part , std::string def )
{
	if ( part < 0 || !HasItem(grp,itm) )
		return def;
	std::string str = GetItem(grp,itm,"");
	DynamicArray s = SplitByComma(str);
	if ( part < s.size() )
		return s[part];
	return def;
}










std::string SuperINI::GetItem( std::string group , std::string item , std::string def )
{
	lc(group);lc(item);

	INIMap::iterator groupIter;

	//If the group does not exist, just return def now.
	if ( !HasGroup(group) )
		return def;

	groupIter = data.find( group );

	SMap* groupMap = &((groupIter->second).map);

	//Does the item exist?
	if ( !HasItem(group,item) )
		return def;
	else
		return groupMap->find(item)->second.val;

}


//================================================
//		Existance Functions
//================================================

//Checks if a given item exists.
bool SuperINI::HasItem( std::string group , std::string item )
{
	lc(group);lc(item);
	if ( !HasGroup(group) )
		return false;

	//The group does exist.
	INIMap::iterator groupIter = data.find( group );
	//Update: Maybe it is okay now.
	SMap* GroupMap = &((groupIter->second).map);

	SMap::iterator iter = GroupMap->find( item );
	return ( iter != GroupMap->end() );
}

bool SuperINI::HasGroup( std::string group )
{
	lc(group);
	INIMap::iterator iter = data.find( group );
	if (iter == data.end())
		return false;
	return true;
}


//================================================
//		Copying
//================================================

//bool Merge
//		true		Copy keeping the old values if they are not overwritten
//		false		Replace the old group, deleting the old group.
bool SuperINI::CopyGroup( std::string source , std::string dest , bool merge )
{
	std::string destKey = lcret(dest);
	lc(source);

	if ( !HasGroup( source ) )
		return false;

	INIMap::iterator sourceIter = data.find( source );

	//If we are not merging.
	if ( !merge )
	{
		SMap GroupCopy = sourceIter->second.map;

		//Make the new group if it doesn't exist
		ForceGroupExist(dest);

		INIMap::iterator destIter = data.find( destKey );

		pushUndo( new UndoByRestoreGroup( destIter , dest ) );

		destIter->second.map = GroupCopy;
		destIter->second.ordering = (sourceIter->second).ordering->Copy();
		RequestAutoSave();
		return true;
	}

	//If we are
	else
	{
		SMap::iterator iter;
		PauseAutoSave(true);
		for ( iter = (sourceIter->second).map.begin() ; iter != (sourceIter->second).map.end() ; iter++ )
		{
			SetItem( dest , iter->second.name , iter->second.val );
		}
		PauseAutoSave(false);
		RequestAutoSave();
		return true;
	}
}

//This function fails if the source does not exist.
bool SuperINI::CopyItem( std::string gSource , std::string iSource , std::string gDest , std::string iDest )
{
	if ( !HasItem( gSource , iSource ) )
		return false;
	return SetItem( gDest , iDest , GetItem( gSource , iSource , "" ) );
}


//================================================
//		Deleting
//================================================

bool SuperINI::DeleteGroup( std::string group )
{
	std::string originalCase = group;
	lc(group);
	if ( HasGroup(group) )
	{
		pushUndo( new UndoByRestoreGroup( data.find(group) , originalCase ) );
		data.erase(group);
		ordering.remove(group);
		RequestAutoSave();
		return true;
	}
	return false;
}

bool SuperINI::DeleteItem( std::string group , std::string item )
{
	std::string grpC = group, itmC = item;
	lc(group);lc(item);
	if ( HasItem( group , item ) )
	{
		pushUndo( new UndoSetItem(grpC,itmC,GetItem(grpC,itmC,"")) );
		INIMap::iterator iter = data.find( group );
		iter->second.map.erase(item);
		
		//Remove from the ordering
		iter->second.ordering->remove(item);

		//Don't let empty groups live on.
		if ( !allowEmptyGroup )
			if ( iter->second.map.empty() )
				DeleteGroup(group);

		RequestAutoSave();

		return true;
	}
	return false;
}

bool SuperINI::DeleteItemFromAllGroups( std::string item )
{
	lc(item);
	PauseAutoSave(true);
	for ( int n = 1 ; n <= GroupCount() ; n++ )
	{
		DeleteItem( GetNthGroupName(n,"") , item );
	}
	PauseAutoSave(false);
	RequestAutoSave();
	return true;
}
bool SuperINI::ClearAll()
{
	PauseAutoSave(true);
	while ( GroupCount() > 0 )
		DeleteGroup( GetNthGroupNameInternal(1,"") );
	PauseAutoSave(false);
	RequestAutoSave();

	//data.clear();
	//ordering.clear();
//	RequestAutoSave();
	return true;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~
//Delete By Pattern
//~~~~~~~~~~~~~~~~~~~~~~~~~~
bool SuperINI::DeleteByPattern( std::string grpCondition , std::string itemCondition , std::string valueCondition , bool caseSensitive )
{
	PauseAutoSave(true);
	SearchResults p;
	PerformSearch( &p , grpCondition , itemCondition , valueCondition ,  caseSensitive*8 , 0 );
	for ( int n = 0 ; n < p.CountItems() ; n++ )
	{
		DeleteItem( p.GetItem(n,0) , p.GetItem(n,1) );
	}
	PauseAutoSave(false);
	RequestAutoSave();
	return true;
}



//================================================
//		Listing
//================================================
std::string SuperINI::GetNthGroupNameInternal( int n ,std::string def )
{
	if ( n > GroupCount() || n <= 0 )
		return def;

	INIMap::iterator iter = data.begin();
	for ( int p = n ; n != 1 ; n-- )
		iter++;
	return iter->first;
}
std::string SuperINI::GetNthItemNameInternal( std::string group , int n , std::string def )
{
	lc(group);
	if ( n > ItemCount(group) || n <= 0 )
		return def;

	SMap::iterator iter = data.find(group)->second.map.begin();
	for ( int p = n ; n != 1 ; n-- )
		iter++;
	return iter->first;
}

std::string SuperINI::GetNthGroupName( int n ,std::string def )
{
	if ( n > GroupCount() || n <= 0 )
		return def;

	std::string grpName = ordering.get(n-1);
	INIMap::iterator groupIter = data.find( grpName );
	if ( groupIter == data.end() )
		return def;
	return groupIter->second.name;
}

//Now this returns the *ordered* nth item.
std::string SuperINI::GetNthItemName( std::string group , int n , std::string def )
{
	lc(group);
	if ( n > ItemCount(group) || n <= 0 )
		return def;

	SMapContainer* smc = &data.find(group)->second;
	return smc->map.find(smc->ordering->get(n-1))->second.name;

}
std::string SuperINI::GetNthItemValue( std::string group , int n , std::string def )
{
	lc(group);
	if ( n > ItemCount(group) || n <= 0 )
		return def;

	return GetItem(group,GetNthItemName(group,n,""),def);
}

//Now updated! =)
//mode:		0	Copy
//			1	Or
//			2	And
//			3	Xor
//bitmask:
//			bit		meaning			mask
//			-------------------------------
//			1		NOT				1
//			2		Group mode		2
//			3		Formula OR		4
//			4		Case sensitive	8
bool SuperINI::PerformSearch( SearchResults* out , std::string grpCondition , std::string itemCondition , std::string valueCondition , int bitMask , int mode )
{
	bool LocalCaseSensitive = bitMask & 8;
	//bool FormulaNOT = !(bitMask & 1);	//NO LONGER USED.
	bool FormulaOR = bitMask & 4;
	bool groupMode = bitMask & 2;

	if ( out == NULL) return false;
	if ( mode == 0 ) out->ClearAll();
	SearchResults* org = 0;
	if ( mode >= 2 )
	{
		org = out;
		out = new SearchResults();
	}
	//Format the patterns to lower case if need be.
	lc(grpCondition);lc(itemCondition);

	//Now search through!
	int matched = 0;
	for ( int grp = 1 ; grp <= GroupCount() ; grp++ )
	{
		matched = 0;
		std::string grpName = GetNthGroupName(grp,"");
		bool a = superini_wildcmp( grpCondition.c_str() , grpName.c_str() , CaseSensitive && LocalCaseSensitive);
		matched += a;
		if ( a  || FormulaOR )
		{
			if ( !FormulaOR ) matched = 0;
			for ( int itm = 1 ; itm <= ItemCount(grpName) ; itm++ )
			{
				int preMatched = matched;
				std::string itmName = GetNthItemName( grpName , itm , "" );
				bool b = superini_wildcmp( itemCondition.c_str() , itmName.c_str()  , CaseSensitive && LocalCaseSensitive);
				matched += b;
				if ( b || FormulaOR )
				{
					if ( !FormulaOR ) matched = 0;
					std::string itmValue = GetItem( grpName , itmName , "" );
					bool c =  superini_wildcmp( valueCondition.c_str() , itmValue.c_str()  , LocalCaseSensitive);
					matched += c;
					if ( c || (FormulaOR && matched != 0) )
					{
						if ( !groupMode )
							out->AddItem( grpName , itmName , itmValue , (mode==1) );
						else
						{
							out->AddItem( grpName , "","" , (mode==1) );
							goto nextGroup;
						}
					}
				}
				matched = preMatched;
				
			}
		}
		nextGroup:; //Shut up. It makes faster code.
	}
	if ( mode >= 2 )
	{
		org->OperatorWith( out , mode );
		delete out;
	}
	return true;
}



int __inline CountSlashes(std::string& x )
{
	int out = 0;
	for ( int n = 0 ; n < x.length() ; n++ )
		if ( x[n] == '/' )
			out++;
	return out;
}

bool SuperINI::FindSubgroups( SearchResults* out , std::string grpN , int levels )
{
	if ( out == NULL) return false;
	out->ClearAll();
	//Format the patterns to lower case if the need be.
	lc(grpN);
	//Now search through!
	for ( int grp = 1 ; grp <= GroupCount() ; grp++ )
	{
		std::string grpName = GetNthGroupName(grp,"");
		std::string grpNameCase = lcret(grpName);
		if ( grpNameCase.substr( 0 , grpN.length()+1 ) == (grpN+"/") )
		{
			std::string cut = grpNameCase.substr(grpN.length());
			int cnt = CountSlashes(cut);
			if ( cnt != 0 && (cnt <= levels || -cnt == levels || levels == 0) )
				out->AddItem( grpName , "" , "" , 0 );
		}
	}
	return true;
}








bool SuperINI::CreateSubINI( SuperINI* out , std::string grpCondition , std::string itemCondition , std::string valueCondition , bool caseSensitive )
{
	SearchResults p;
	PerformSearch( &p , grpCondition , itemCondition , valueCondition ,  caseSensitive*8 , 0 );
	out->PauseAutoSave(true);
	for ( int n = 0 ; n < p.CountItems() ; n++ )
	{
		out->SetItem( p.GetItem(n,0) , p.GetItem(n,1) , p.GetItem(n,2) );
	}
	out->PauseAutoSave(false);
	out->RequestAutoSave();

	return true;
}

bool SuperINI::PerformMultiplesSearch( SearchResults* out , std::string grp , std::string item )
{
	out->ClearAll();
	if ( HasItem( grp , item ) )
		out->AddItem( grp , item , GetItem( grp , item , "") , 0 );
	PerformSearch(out ,  grp , item + ".*" , "*" , 8 , 0 );
	return true;
}



//================================================
//		Information
//================================================
int SuperINI::GroupCount()
{
	return data.size();
}
int SuperINI::ItemCount( std::string group )
{
	lc(group);
	if ( !HasGroup(group) )
		return 0;
	INIMap::iterator iter = data.find( group );
	return iter->second.map.size();

}

int SuperINI::TotalItemCount( )
{
	int cnt = 0;
	for ( int n = 1 ; n <= GroupCount() ; n++ )
	{
		cnt += ItemCount(GetNthGroupName( n , "" ));
	}
	return cnt;
}


//================================================
//		Renaming
//================================================

//Mode:
//			0			Overwrite
//			1			Merge (with dominance)
//			2			Do not overwrite
bool SuperINI::RenameGroup( std::string old , std::string nu , int mode )
{
	//lc(old);
	//lc(nu);
	if ( mode == 2 && HasGroup(nu) )
		return false;
	if ( lcret(old) == lcret(nu) )
		return false;
	
	PauseAutoSave(true);
	CopyGroup( old , nu , !!mode );
	ordering.swap(lcret(old),lcret(nu));
	DeleteGroup( old );
	PauseAutoSave(false);
	RequestAutoSave();
	return true;
}

bool SuperINI::RenameItem( std::string grp , std::string oldItem , std::string newItem , bool allowOverwrite )
{
	if ( lcret(oldItem) == lcret(newItem) )
		return false;
	if ( !HasItem( grp , oldItem ))
		return false;

	bool hasDest = HasItem( grp , newItem );
	if ( !allowOverwrite && hasDest )
		return false;
	PauseAutoSave(true);

	//1. Add it
	SetItem( grp , newItem , GetItem( grp , oldItem , "" ) );

	//2. Swap the two positions in the look-up table
	data.find(grp)->second.ordering->swap( oldItem , newItem );

	//3. Delete the source
	DeleteItem( grp , oldItem );
	PauseAutoSave(false);
	RequestAutoSave();
	return true;
}

bool SuperINI::MoveItem( std::string grpOld , std::string grpNew , std::string oldItem , std::string newItem , bool allowOverwrite )
{
	if ( !HasItem( grpOld , oldItem ))
		return false;
	if ( !allowOverwrite && HasItem( grpNew , newItem ) )
		return false;
	PauseAutoSave(true);
	SetItem( grpNew , newItem , GetItem( grpOld , oldItem , "" ) );
	DeleteItem( grpOld , oldItem );
	PauseAutoSave(false);
	RequestAutoSave();
	return true;
}

bool SuperINI::MoveItemToGroup( std::string grpOld , std::string grpNew , std::string Item , bool allowOverwrite )
{
	return MoveItem( grpOld , grpNew , Item , Item , allowOverwrite );
}


//================================================
//		Merge
//================================================
bool SuperINI::Merge( SuperINI* x , bool allowOverwrite )
{
	if ( x == NULL) return false;
	//Look through each of its groups and copy each group in.
	for ( int n = 1 ; n <= x->GroupCount() ; n++ )
	{
		std::string grpName = x->GetNthGroupName(n,"");
		MergeGroup( x , grpName , grpName , allowOverwrite );
	}
	return true;
}

//mode:
//			0		Overwrite in external objects favour
//			1		Overwrite in _this_'s favour
bool SuperINI::MergeGroup( SuperINI* x , std::string grpSource , std::string grpDest , bool allowOverwrite )
{
	if ( x == NULL ) return false;
	//Merges a group from an external source.
	for ( int n = 1 ; n <= x->ItemCount(grpSource) ; n++ )
	{
		std::string itemName = x->GetNthItemName( grpSource , n , "" );
		if (! ( !allowOverwrite && HasItem( grpDest , itemName ) ))
			SetItem( grpDest , itemName , x->GetItem( grpSource , itemName , "" ) );
	}
	return true;
}

//================================================
//		Case Sensativity
//================================================


bool SuperINI::UnCollapseToPerfered( std::string grpName )
{
	if ( !HasGroup( grpName ) )
		return false;

	INIMap::iterator sourceIter = data.find( grpName );

	SMap GroupCopy = (sourceIter->second).map;
	std::string perferedName = sourceIter->second.name;

	for ( SMap::iterator x = GroupCopy.begin() ; x != GroupCopy.end() ; x++ )
	{
		//x->first = lcretALWAYS(x->first);
		if ( x->second.name != x->first )
		{
			GroupCopy.insert( SMap::value_type(  x->second.name  ,  SuperItem(x->second.name,x->second.val) ));
			GroupCopy.erase( x );
			x = GroupCopy.begin();
		}
	}

	//Make the new group if it doesn't exist
	if ( grpName != perferedName )
	{
		if ( !HasGroup( perferedName ) )
			data.insert( INIMap::value_type( perferedName , SMapContainer(perferedName) ) );

		INIMap::iterator destIter = data.find( perferedName );
		destIter->second.map = GroupCopy;
		data.erase(sourceIter);
		return true;
	}
	else sourceIter->second.map = GroupCopy;

	return false;
}

bool SuperINI::CollapseToLowercaseKey( std::string grpName )
{
	if ( !HasGroup( grpName ) )
		return false;

	INIMap::iterator sourceIter = data.find( grpName );

	SMap GroupCopy = (sourceIter->second).map;

	for ( SMap::iterator x = GroupCopy.begin() ; x != GroupCopy.end() ; x++ )
	{
		//x->first = lcretALWAYS(x->first);
		if ( lcretALWAYS(x->first) != x->first )
		{
			GroupCopy.insert( SMap::value_type(  lcretALWAYS(x->first)  ,  SuperItem(x->second.name,x->second.val) ));
			GroupCopy.erase( x );
			x = GroupCopy.begin();
		}
	}

	//Make the new group if it doesn't exist
	if ( lcretALWAYS(grpName) != grpName )
	{
		if ( !HasGroup( lcretALWAYS(grpName) ) )
			data.insert( INIMap::value_type( lcretALWAYS(grpName) , SMapContainer(grpName) ) );

		INIMap::iterator destIter = data.find( lcretALWAYS(grpName) );
		destIter->second.map = GroupCopy;
		data.erase(sourceIter);
		return true;
	}
	else	sourceIter->second.map = GroupCopy;

	return false;
}

bool SuperINI::SetCaseSensitivity( bool _CaseSensitive )
{
	if ( _CaseSensitive == CaseSensitive )
		return false;

	//If we are becoming case sensitive, that is if:
	if ( _CaseSensitive )
	{
		CaseSensitive = true;
		PauseAutoSave(true);
		//Go through everyone and rename them to their display title.
		for ( int n = 1 ; n <= GroupCount() ; n++ )
			UnCollapseToPerfered( GetNthGroupNameInternal(n,"") );

		PauseAutoSave(false);
		RequestAutoSave();
		return true;
	}

	//Go through everyone and rename them to their display title.
	else if ( ! _CaseSensitive )
	{
		//CaseSensitive = false;
		PauseAutoSave(true);
		//Go through everyone and rename them to their display title.
		for ( int n = 1 ; n <= GroupCount() ; n++ )
		{
			if ( CollapseToLowercaseKey( GetNthGroupNameInternal(n,"") ) )
				n = 1;
		}
		CaseSensitive = false;
		PauseAutoSave(false);
		RequestAutoSave();
		return true;
	}
	CaseSensitive = _CaseSensitive;
	return true;
}


bool SuperINI::SetItemMD5( std::string grp , std::string item , std::string val )
{
	md5.setPlainText( val.c_str() );
	return SetItem( grp , item , md5.getMD5Digest() );
}
bool SuperINI::CompareItemMD5( std::string grp , std::string item , std::string def , std::string compareMit )
{
	md5.setPlainText( compareMit.c_str() );
	std::string a = md5.getMD5Digest();
	
	md5.setPlainText( def.c_str() );
	def = md5.getMD5Digest();

	std::string b = GetItem( grp ,item , def );
	return (a.compare(b) == 0 );
}

//================================================
//		Functions which do things only as a bonus
//================================================
std::string SuperINI::HashMD5( std::string what )
{
	md5.setPlainText( what.c_str() );
	return md5.getMD5Digest();	
}

bool SuperINI::WildcatCompare( std::string pattern , std::string str , bool caseSensative )
{
	return !!superini_wildcmp(pattern.c_str() , str.c_str() , caseSensative );
}

std::string SuperINI::ToggleEncryption( std::string data , std::string key )
{
	char* buf = new char[ data.length()+1];
	strcpy( buf , data.c_str() );
	
	SuperINI_Crypt( buf , data.length() , key.c_str() , key.length() );

	std::string out = std::string(buf);
	
	delete [] buf;
	return out;
}


bool SuperINI::CompressTo( std::string source , std::string dest )
{
	FileClass f(source.c_str(),FileClass_READ);
	if ( f.is_open() )
	{
		int size = f.size();
		char* data = f.GetAllDataC();
		f.close();

		unsigned char* outpnt;
		int outlen;
		CompressHuffman( (unsigned char*)data , size ,  outpnt , outlen );
		delete [] data;
	
		FileClass f2(dest.c_str() , FileClass_WRITE);
		if ( f2.is_open() )
		{
			f2.write( outpnt , outlen );
			f2.close();
		}

		delete [] outpnt;
		return true;
	}
	return false;

}
bool SuperINI::UncompressTo( std::string source , std::string dest )
{
	FileClass f( source.c_str() , FileClass_READ );
	if ( f.is_open() )
	{
		char* data = f.GetAllDataC();
		int inlen = f.size();
		f.close();

		unsigned char* outpnt;
		int outlen;
		bool sucsess = DecompressHuffman( (unsigned char*)data , inlen  ,  outpnt , outlen );
		delete [] data;
	
		if ( !sucsess )
			return false;

		FileClass f2(dest.c_str() , FileClass_WRITE );
		if ( f2.is_open() )
		{
			f2.write( outpnt , outlen );
			f2.close();
		}

		delete [] outpnt;
		return true;
	}
	return false;
}

//================================================
//		LoadStream
//================================================
//Notes:
//
//	(1) If an item comes before any group header, it is assumed to be in the group
//		"".
//
//	(2)	Groups should not start or end with spaces. We allow it.
//


bool SuperINI::SplitSubjectToEscapes( std::string what , std::string& one , std::string& two )
{
	if ( ! EscapeCharsInItem )
	{
		if ( what.find('=') == -1 )
			return false;
		else
		{
			one = what.substr(0 , what.find('=') );
			two = what.substr( what.find('=')+1 );
			return true;
		}
	}

	int n = 0;
	//state:
	//	0		idle
	//	1		in slash
	int state = 0;
	std::string item = "";
	while ( n < what.length() )
	{
		if ( state == 0 && what[n] == '\\' )
			state = 1;
		else if ( state == 0 && what[n] == '=' )
		{
			//we are done
			one = item;
			two = what.substr( n+1 );
			return true;
		}
		else if ( state == 1 )
		{
			switch (what[n])
			{
			case '=':
				item += '=';
				break;
			case 'n':
				item += GetNewLine();
				break;
			case 't':
				item += '\t';
				break;
			case '_':
				item += ' ';
				break;
			case '\\':
				item += '\\';
				break;
			case ';':
				item += ';';
				break;
			case '[':
				item += '[';
				break;
			}
			state = 0;
		}
		else
			item += what[n];

		n++;
	}
	return false;
}


std::string SuperINI::ValueEscapeCharacters( std::string what ,  int force )
{
	if ( (!force && !EscapeCharsInValue) || force == -1 )
		return what;

	int n = 0;
	//state:
	//	0		idle
	//	1		in slash
	int state = 0;
	std::string item = "";
	while ( n < what.length() )
	{
		if ( state == 0 && what[n] == '\\' )
			state = 1;
		else if ( state == 1 )
		{
			switch (what[n])
			{
			case 'n':
				item += GetNewLine();
				break;
			case 't':
				item += '\t';
				break;
			case '_':
				item += ' ';
				break;
			case '\\':
				item += '\\';
				break;
			case 'q':
			case '"':
				item += '"';
				break;
			}
			state = 0;
		}
		else
			item += what[n];

		n++;
	}
	return item;
}



void SuperINI::Strip( std::string& str )
{
	while ( str[0] == ' ' || str[0] == '\t' )
		str = str.substr(1);
	while ( str[str.length()-1] == ' ' || str[str.length()-1] == '\t' )
		str = str.substr(0,str.length()-1);
	return;
}


std::string GetPathFromStack( DynamicArray& stack )
{
	std::string out = "";
	for ( int n = 0; n != stack.size() ; n++ )
		out += "/" + stack[n];
	if ( out != "" )
		return out.substr(1);
	return out;
}





//Update should be 'true' if autosave should be applied after.
bool SuperINI::LoadStream( std::string data , bool update , bool clear )
{
	PauseAutoSave(true);

	if ( clear )
		ClearAll();

	std::string currentGroup = "";
	std::string nonStripedLine = "";
	DynamicArray currentGroupStack;// = DynamicArray();		TODO
	bool ignoreForABit = false;
	int lastDepth = 0;
	int nxt = 0;

	while ( nxt != -1 )
	{
		//Grab the next line
		
		//If in standard mode, find
		//next 10 or 13 and skip next 
		//char if it is 10 and you found 13.
		std::string line = "";
		if ( !StandardNewLines )
		{
			nxt = data.find(NewLineChar);
			line = data.substr(0,nxt);

			if ( nxt + (NewLineChar.length()) < data.length() )
				data = data.substr( nxt + (NewLineChar.length()) );
			else
				nxt = -1;
		}
		else
		{
			nxt = data.find_first_of( STANDARD_NEW_LINE );
			if ( nxt != -1 )
			{
				line = data.substr( 0 , nxt );
				if ( data[nxt+1] == '\n' )
					nxt++;
				data = data.substr( nxt + 1 );
			}
			else line = data;
		}
		int depth = 0;
		if ( subGroups )
		{
					for ( int n = 0 ; n < line.length() ; n++ )
					{
						if ( line[n] == '\t' )
							depth++;
						else break;
					}
					if ( depth < lastDepth )
					{
						for ( int n = 0 ; n < lastDepth-depth; n++ )
							if ( !currentGroupStack.empty() )
								currentGroupStack.pop_back();
						currentGroup = GetPathFromStack( currentGroupStack );
					}
		}
		Strip(line);

		if		( line == "" );
		else if ( line[0] == ';' );
		else if ( line[0] == '[' )
		{
			ignoreForABit = false;
			line = ValueEscapeCharacters( line , ((EscapeCharsInGroups)?1:-1) );
			if ( line[line.length()-1] == ']' )
			{
				line = line.substr(1,line.length()-2);
				if ( subGroups )
				{
					if ( depth == lastDepth )
					{
						if ( !currentGroupStack.empty() )
							currentGroupStack.pop_back();
					}
						
					lastDepth = depth;
					if ( depth == 0 )
						currentGroupStack.clear();

					currentGroupStack.push_back(line);
					line = GetPathFromStack( currentGroupStack );
				}


				if ( HasGroup(line) && groupRepeatSetting != 3 )
					//If it is 3 we don't even care if it exists.
				{
						//		0		Take first value	{mmf default}
						//		1		Take last value
						//		2		Rename as [NAME].# , where # is a number
						//		3		Merge (For groups only) [Sorted]
						if ( groupRepeatSetting == 1 )
						{
							DeleteGroup(line);
							currentGroup = line;
						}
						if ( groupRepeatSetting == 0 )
						{
							ignoreForABit = true;
						}
						if ( groupRepeatSetting == 2 )
						{
							int n = 1;
							while ( HasGroup( line + "." + toString(n)) )
								n++;
							currentGroup = line + "." + toString(n);
						}
					
				}
				else currentGroup = line;
			}
			if ( allowEmptyGroup )
					AddGroupWithNoItems(currentGroup);
		}
		else if ( !ignoreForABit )
		{
			std::string item;
			std::string value;
			if ( SplitSubjectToEscapes( line , item , value ))
			{
				Strip( item );
				Strip( value );
				value = ValueEscapeCharacters( value ,  false );
				if ( !NeverQuoteStrings )
				{
					if ( value[0] == '"' && value[value.length()-1] == '"' )
						value = value.substr(1,value.length()-2);
				}
				bool exists = HasItem( currentGroup , item );
				if ( exists && itemRepeatSetting == 2 )
				{
					//Rename mode
					int n = 1;
					while ( HasItem( currentGroup , item + "." + toString(n)) )
						n++;
					
					SetItem( currentGroup , item + "." + toString(n) , value );
				}
				else if ( ! ( exists && itemRepeatSetting == 0 ) )
				{
					SetItem( currentGroup , item , value );
				}
			}
		}
	}
	RemoveEmptyGroups();
	PauseAutoSave(false);
	if (update)
		RequestAutoSave();

	return true;
}

bool SuperINI::Load( std::string path , bool changeAutosavePath )
{
	PauseAutoSave(true);
	if ( changeAutosavePath )
	{
		ClearAll();
		autoSavePath = path;
	}
	bool ok = LoadFromFile( path , AutoSaveCompressed , AutoSaveEncrypted , AutoSaveKey );
	PauseAutoSave(false);
	return ok;
}

bool SuperINI::LoadInAddition( std::string path )
{
	autoSavePath = path;
	LoadFromFile( path , AutoSaveCompressed , AutoSaveEncrypted , AutoSaveKey );
	return true;
}


bool SuperINI::RequestAutoLoad( )
{
	if ( autoSavePath != "" )
	{
		return Load( autoSavePath);
	}
	return false;
}


bool SuperINI::Save( )
{
	if ( autoSavePath != "" && !ReadOnly )
	{
		return SaveToFile( autoSavePath , AutoSaveCompressed , AutoSaveEncrypted , AutoSaveKey );
	}
	return false;
}

bool SuperINI::SaveAs( std::string path )
{
	if ( SaveToFile( path , AutoSaveCompressed , AutoSaveEncrypted , AutoSaveKey ) )
	{
		autoSavePath = path;
		SetReadOnly(false);
		return true;
	}
	return false;
}

std::string SuperINI::WriteCVSLine( std::string grp , std::string seperator )
{
	std::string out = "";
	for ( int n = 1 ; n <= ItemCount(grp) ; n++ )
	{
		out += GetNthItemName(grp,n,"");
		out += seperator;
		out += GetNthItemValue(grp,n,"");
		out += seperator;
	}
	out = out.substr(0,out.length()-1);
	return out;
}

std::string SuperINI::WriteCSV( std::string grp , std::string seperator )
{
	std::string out = "";
	if ( grp == "" )
	{
		for ( int n = 1 ; n <= GroupCount() ; n++ )
		{
			std::string grpName = GetNthGroupName(n,"");
			out += grpName;
			out += seperator;
			out += WriteCVSLine(grpName,seperator);
			out += STANDARD_NEW_LINE;
		}
		out = out.substr(0,out.length()-2);
	}
	else
	{
		out = WriteCVSLine(grp,seperator);
	}
	return out;
}



bool SuperINI::ExportAsCSV( std::string path , std::string grp , std::string seperator )
{
	std::string txt = WriteCSV(grp,seperator);
	FileClass f(path.c_str(),FileClass_WRITE);
	f.write( (unsigned char*)txt.c_str() , txt.length()+1 );
	f.close();
	return true;
}

std::string GetNextPart(std::string& line, std::string seperator )
{
	if ( line.find(seperator) == -1 )
	{
		std::string out = line;
		line = "";
		return out;
	}
	std::string out = line.substr(0,line.find(seperator));
	if( line.length() >= line.find(seperator)+seperator.length() )
		line = line.substr(line.find(seperator)+seperator.length());
	else
		line = "";
	return out;
}

bool SuperINI::ImportCSVLine( std::string line , std::string grp , std::string seperator )
{
	while( line != "" )
	{
		std::string itm = GetNextPart(line,seperator);
		std::string val = GetNextPart(line,seperator);
		SetItem(grp,itm,val);
	}
	return false;
}
bool SuperINI::ImportCSV( std::string path , std::string grp , std::string seperator )
{
	FileClass f(path.c_str(),FileClass_READ);
	if ( f.is_open() )
	{
		while( !f.eof() )
		{
			std::string line = f.getline();
			std::string actualGrp = grp;
			if ( actualGrp == "" )
				actualGrp = GetNextPart(line,seperator);
			ImportCSVLine(line,actualGrp,seperator);
		}
	}
	f.close();
	return true;
}

bool SuperINI::Close()
{
	autoSavePath = "";
	PauseAutoSave(true);
	ClearAll();
	PauseAutoSave(false);
	SetReadOnly(false);
	return true;
}

bool SuperINI::New( std::string path , bool keepOldData )
{
	if ( !keepOldData )
		Close();
	autoSavePath = path;
	if ( keepOldData )
		RequestAutoSave();

	return true;
}

//NOTE: This _does not_ clear the old file first!
bool SuperINI::LoadFromFile( std::string path , bool compressed , bool encrypted , std::string key )
{

	FileClass f(path.c_str(),FileClass_READ);
	if ( f.is_open() )
	{
		char* data = f.GetAllDataC();
		int size = f.size();
		f.close();

		if ( compressed )
		{
			unsigned char* decompressed;
			int len;
			if ( DecompressHuffman( (unsigned char*)data , size , decompressed , len ) )
			{
				delete [] data;
				data = (char*)decompressed;
				size = len;	
			}
		}
		if ( encrypted )
		{
			SuperINI_Crypt( data , size , key.c_str() , key.length() );
		}
		//data[size] = 0;	//This is allowed as GetAllDataC gives us a buffer one byte too large.

		LoadStream( std::string(data) , false , false );
		delete [] data;
		return true;
	}
	else return false;

}

bool SuperINI::StringReplace( std::string &str , std::string pat , std::string with )
{
	int pos = 0;
	while ( (pos = str.find( pat , pos )) != -1 )
	{
		str.replace( pos , pat.length() , with );
		pos+= with.length();
	}
	return true;
}

std::string SuperINI::ApplyEscapes( std::string what , bool forReal , bool spaces , bool equals )
{
	if ( !forReal )
		return what;

	StringReplace( what , "\\" , "\\\\" );
	StringReplace( what , "\"" , "\\q" );
	StringReplace( what , "\t" , "\\t" );
	StringReplace( what , GetNewLine()  , "\\n" );
	if ( spaces )
	{
		if (what[0] == ' ')
			what = "\\_" + what.substr(1);
		if ( what[what.length()-1] == ' ' )
			what = what.substr(0,what.length()-1) + "\\_";
	}
	if ( equals )
		StringReplace( what , "=" , "\\=" );

	return what;
}

#define STR_BEFOREDOT(str)		str.substr(0,str.find_last_of('.'))

std::string SuperINI::WriteStream( )
{
	std::string out = "";
	//Now search through!
	Set mySet;
	for ( int grp = 1 ; grp <= GroupCount() ; grp++ )
	{
		std::string grpName = GetNthGroupName(grp,"");
		std::string grpNameUnProcessed = grpName;
		std::string eachLine = "";
		grpName = ApplyEscapes( grpName , EscapeCharsInGroups , false , false );
		if ( subGroups )
		{
			std::string pathSoFar = "";
			while ( grpName.find('/') != -1 )
			{
				std::string front = grpName.substr(0,grpName.find('/'));
				pathSoFar += "/" + front;
				if ( !HasGroup(pathSoFar.substr(1)) && !mySet.HasItem(pathSoFar.substr(1)))
				{
					mySet.Insert( pathSoFar.substr(1) );
					out += eachLine + "[" + front + "]"+ GetNewLine();//pathSoFar --> front.
				}
				grpName = grpName.substr(grpName.find('/')+1);
				eachLine += '\t';
			}
		}
		out += eachLine;
		if (! saveRepeats )
			out += "[" + grpName + "]" + GetNewLine();
		else
			out += "[" + STR_BEFOREDOT(grpName) + "]" + GetNewLine();
			for ( int itm = 1 ; itm <= ItemCount(grpNameUnProcessed) ; itm++ )
			{
				out += eachLine;
				std::string itmName = GetNthItemName( grpNameUnProcessed , itm , "" );
				std::string itmNameUnProcessed = itmName;
				itmName = ApplyEscapes( itmName , EscapeCharsInItem , true , true );
				if ( EscapeCharsInItem && ( itmName[0] == ';' || itmName[0] == '[' ) )
					itmName = "\\" + itmName;
				std::string itmValue = GetItem( grpNameUnProcessed , itmNameUnProcessed , "" );
				itmValue = ApplyEscapes( itmValue , EscapeCharsInValue , NeverQuoteStrings , false );
				if ( ! NeverQuoteStrings && (itmValue[0] == ' ' || itmValue[itmValue.length()-1] == ' ') )
					itmValue = "\"" + itmValue + "\"";
				if (! saveRepeats )
					out += itmName + "=" + itmValue + GetNewLine();
				else
					out += STR_BEFOREDOT(itmName) + "=" + itmValue + GetNewLine();
			}
	}
	out = out.substr( 0 , out.length() - GetNewLine().length() );
	return out;
}

bool SuperINI::SaveToFile( std::string path , bool compressed , bool encrypted , std::string key )
{
	if ( cb != NULL )
		cb( rdPtrFavorite , 2 );

	std::string strData = WriteStream();
	//while ( compressed && strData.length() < 8 )
	//	strData += GetNewLine();

	int outlen = strData.length();
	unsigned char* data = new unsigned char[outlen+1];

	strcpy( (char*)data , strData.c_str() );
	if ( encrypted )
	{
		SuperINI_Crypt( (char*)data , strData.length() , key.c_str() , key.length() );
	}
	if ( compressed )
	{
		unsigned char* outpnt;
		CompressHuffman( (unsigned char*)data , strData.length() ,  outpnt , outlen );
		delete [] data;
		data = (unsigned char*)outpnt; 
	}
	FileClass f(path.c_str() , FileClass_WRITE);
	if ( f.is_open())
	{
		f.write( data , outlen );
		f.close();
	}
	delete [] data;
	return true;
}

//Export/Import settings functions (made for the MMF versions global object feature)
void SuperINI::ExportSettings( SuperINI_SettingsStruct* settings )
{
	settings->AutoSaveCompressed = AutoSaveCompressed;
	settings->AutoSaveEncrypted = AutoSaveEncrypted;
	settings->AutoSaveKey = AutoSaveKey;
	settings->autoSavePath = autoSavePath;
	settings->bAutoSave = bAutoSave;
	settings->CaseSensitive = CaseSensitive;
	settings->EscapeCharsInGroups = EscapeCharsInGroups;
	settings->EscapeCharsInItem = EscapeCharsInItem;
	settings->EscapeCharsInValue = EscapeCharsInValue;
	settings->groupRepeatSetting = groupRepeatSetting;
	settings->itemRepeatSetting = itemRepeatSetting;
	settings->NeverQuoteStrings = NeverQuoteStrings;
	settings->NewLineChar = NewLineChar;
	settings->ReadOnly = ReadOnly;
	settings->saveRepeats = saveRepeats;
	settings->StandardNewLines = StandardNewLines;
	settings->rdPtrFavorite = rdPtrFavorite;
	settings->subGroups = subGroups;
	settings->allowEmptyGroup = allowEmptyGroup;
}
void SuperINI::ImportSettings( SuperINI_SettingsStruct* settings )
{
//	MessageBox(NULL,"Import settings (checking)","K",0);
	if ( rdPtrFavorite == settings->rdPtrFavorite )
		return;

	AutoSaveCompressed = settings->AutoSaveCompressed;
	AutoSaveEncrypted = settings->AutoSaveEncrypted;
	AutoSaveKey = settings->AutoSaveKey;
	autoSavePath = settings->autoSavePath;
	bAutoSave = settings->bAutoSave;
	EscapeCharsInGroups = settings->EscapeCharsInGroups;
	EscapeCharsInItem = settings->EscapeCharsInItem;
	EscapeCharsInValue = settings->EscapeCharsInValue;
	groupRepeatSetting = settings->groupRepeatSetting;
	itemRepeatSetting = settings->itemRepeatSetting;
	NeverQuoteStrings = settings->NeverQuoteStrings;
	NewLineChar = settings->NewLineChar;
	ReadOnly = settings->ReadOnly;
	saveRepeats = settings->saveRepeats;
	StandardNewLines = settings->StandardNewLines;
	rdPtrFavorite = settings->rdPtrFavorite;

	SetSubGroups( settings->subGroups , settings->allowEmptyGroup );
	SetCaseSensitivity( settings->CaseSensitive );	//For this can actually change the data.
}


bool SuperINI::RemoveEmptyGroups()
{
	if ( allowEmptyGroup )
		return false;

	PauseAutoSave(true);
	for ( int n = 1 ; n <= GroupCount(); n++ )
	{
		if ( ItemCount(GetNthGroupName(n,"")) == 0 )
		{
			DeleteGroup( GetNthGroupName(n,"") );
			n--;
		}
	}
	PauseAutoSave(false);
	return true;
}

bool SuperINI::AddGroupWithNoItems( std::string name )
{
	ForceGroupExist(name);
	return true;
}


std::string SuperINI::WriteCSVArray( std::string seperator )
{
	std::string out = "";
	DynamicArray currentItemNames;
	int n;
	for ( n = 1 ; n <= GroupCount(); n++ )
	{
		DynamicArray thisGroup;
		std::string grp = GetNthGroupName(n,"");
		for ( int m = 1 ; m <= ItemCount(grp) ; m++ )
		{
			std::string itm = GetNthItemName(grp,m,"");
			lc(itm);
			int n = currentItemNames.find(itm);
			if ( n == -1 )
			{
				currentItemNames.push_back(itm);
				n = currentItemNames.size()-1;
			}
			thisGroup.set( n , GetNthItemValue(grp,m,"") ); 
		}
		std::string thatGroup = grp;
		for ( int n = 0 ; n < thisGroup.size() ; n++ )
		{
			thatGroup += seperator + thisGroup[n];
		}
		out += thatGroup + STANDARD_NEW_LINE;
	}
	std::string head = "Group";
	for ( n = 0 ; n < currentItemNames.size() ; n++ )
	{
		head += seperator + currentItemNames[n];		
	}
	out = head + STANDARD_NEW_LINE + out;
	return out;
}

bool SuperINI::ExportCSVArray( std::string path , std::string seperator )
{
	std::string txt = WriteCSVArray(seperator);
	FileClass f(path.c_str(),FileClass_WRITE);
	f.write( (unsigned char*)txt.c_str() , txt.length()+1 );
	f.close();
	return true;
}

bool SuperINI::ImportCSVArray( std::string path , std::string seperator )
{
	FileClass f(path.c_str(),FileClass_READ);
	if ( f.is_open() )
	{
		DynamicArray	itemNames;
		std::string line = f.getline();
		std::string part = "";
		while ( line != "" )
		{
			part = GetNextPart(line,seperator);
			itemNames.push_back( part );
		}
		while( !f.eof() )
		{
			std::string line = f.getline();
			std::string actualGrp = GetNextPart(line,seperator);
			int iteration = 0;
			while ( line != "" && iteration++ < itemNames.size() )
			{
				part = GetNextPart(line,seperator);
				if ( part != "" )
					SetItem( actualGrp , itemNames[iteration] , part );
			}
		}
	}
	f.close();
	return true;
}

std::string SuperINI::GetNthGeneralItemName( int pos )
{
	DynamicArray currentItemNames;

	for ( int n = 1 ; n <= GroupCount(); n++ )
	{
		std::string grp = GetNthGroupName(n,"");
		for ( int m = 1 ; m <= ItemCount(grp) ; m++ )
		{
			std::string itm = GetNthItemName(grp,m,"");
			lc(itm); //Best not?
			if ( currentItemNames.find(itm) == -1 )
				currentItemNames.push_back(itm);
			if ( currentItemNames.size() == pos )
				return GetNthItemName(grp,m,"");
		}
	}
	//return currentItemNames[pos-1];
	return "";
}

int SuperINI::GetNthGeneralItemNameCount( )
{
	DynamicArray currentItemNames;

	for ( int n = 1 ; n <= GroupCount(); n++ )
	{
		std::string grp = GetNthGroupName(n,"");
		for ( int m = 1 ; m <= ItemCount(grp) ; m++ )
		{
			std::string itm = GetNthItemName(grp,m,"");
			lc(itm);
			if ( currentItemNames.find(itm) == -1 )
				currentItemNames.push_back(itm);
		}
	}
	return currentItemNames.size();
}


void SuperINI::GroupEffect( std::string grp , int eff )
{
	INIMap::iterator groupIter;

	//If the group does not exist, just return def now.
	if ( !HasGroup(grp) )
		return;

	groupIter = data.find( grp );

	switch ( eff )
	{
	case 0:	//Reverse

		groupIter->second.ordering->reverse();
		break;


	case 1:	//Shuffle
		{
			DynamicArray* cpy = groupIter->second.ordering;

			int n = 0;
			while ( !cpy->empty() )
			{
				int num = rand()%cpy->size();
				groupIter->second.ordering->set(n,cpy->get(num));
				cpy->removeAt(num);
				n++;
			}
			break;
		}

	case 2:	//Cycle up
		{
			DynamicArray* arr = groupIter->second.ordering;
			std::string tmp = arr->get(0);
			arr->removeAt(0);
			arr->push_back(tmp);
		}
		break;
	case 3:	//Cycle down
		{
			DynamicArray* arr = groupIter->second.ordering;
			std::string tmp = arr->get(arr->size()-1);
			for ( int n = arr->size()-1; n > 0 ; n-- )
				arr->set(n , arr->get(n-1));
			arr->set(0,tmp);
		}
		break;




	}


}

void SuperINI::AddDirectlyForUndo( std::string group , SMap map, DynamicArray* ordering ) {
	std::string groupKey = lcret(group);
	ForceGroupExist(group);

	INIMap::iterator groupIter;
	groupIter = data.find( groupKey );

	pushUndo( new UndoByRestoreGroup(groupIter,group));

	groupIter->second.map = map;
	groupIter->second.ordering = ordering->Copy();

	RequestAutoSave();
}


void SuperINI::undo()
{
	if ( undoInformation.empty() )
		return;
	UndoElement* top = undoInformation.back();
	if ( top == 0 ) return;
	useRedoStack = true;
	PauseAutoSave(true);
	newUndoBlock();
	top->perform(this);
	PauseAutoSave(false);
	RequestAutoSave();
	delete top;
	undoInformation.pop_back();
	useRedoStack = false;

	//if ( !autoAddUndoStage )
	//	forceNewUndoBlock();
}

void SuperINI::redo()
{
	if ( redoInformation.empty() )
		return;

//	if ( !autoAddUndoStage )
//		forceNewUndoBlock();

	UndoElement* top = redoInformation.back();
	if ( top == 0 ) return;
	useRedoStack = false;
	inRedo = true;
	PauseAutoSave(true);
	newUndoBlock();
	//forceNewUndoBlock();
	top->perform(this);
	PauseAutoSave(false);
	inRedo = false;
	RequestAutoSave();
	delete top;
	redoInformation.pop_back();

	// Add a new bit if we have to...
	//if ( !autoAddUndoStage && (undoLimit != 0) && (undoInformation.empty() || !undoInformation.back()->empty()) )
	//	forceNewUndoBlock();
}


