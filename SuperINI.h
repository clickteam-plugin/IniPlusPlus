#ifndef __SUPERINI
#define __SUPERINI

#pragma warning( disable : 4786 ) //C4786
#pragma warning( disable : 4503 )
#pragma warning( disable : 4800 )


#include <map>
#include <list>
#include "new_vector.h"
#include <string>
#include "md5class.h"

bool CompressHuffman( unsigned char* input , int inlen , unsigned char* & output , int& outlen );
bool DecompressHuffman( unsigned char* input , int inlen , unsigned char* & output , int& outlen );

#include "UndoInformation.h"

DynamicArray SplitByComma( std::string what );

int Ini_Item_Compare( const std::string& a , const std::string & b , int mode );


//numberSort makes numbers appear top of the list in numerical order.
struct numberSort : std::binary_function<std::string, std::string, bool> {
	bool operator()(const std::string& _X, const std::string& _Y) const
	{return _X.compare(_Y)<0;}
	};


struct SuperItem
{
	std::string		name;
	std::string		val;
	SuperItem(const std::string& _name, const std::string& _val )
	{ name = _name ; val = _val; }
};

typedef std::map<std::string, SuperItem ,  numberSort   > SMap;

//Represents a group
struct SMapContainer
{
	SMap			map;
	std::string		name;
	SMapContainer( const std::string & _name  )
	{
		name = _name;
		map = SMap();
		ordering = new DynamicArray();
	}
	SMapContainer( SMapContainer const & old )
	{
		name = old.name;
		map = old.map;
		ordering = old.ordering->Copy();
	}
	~SMapContainer()
	{
		delete ordering;
	}
	//This contains the ordering information
	DynamicArray* ordering;
};

typedef std::map<std::string,SMapContainer , numberSort> INIMap;




struct Triple
{
	std::string grp;
	std::string itm;
	std::string val;

	Triple(std::string a, std::string b, std::string c)
	{
		grp = a;
		itm = b;
		val = c;
	};
	Triple(){ Triple("","",""); }
	bool operator==(Triple a) const
	{
		return ( (a.grp) == (grp) && (a.itm) == (itm) );
	}
};
struct SearchResults
{
	DynamicArrayGeneral< Triple >	results;

	void AddItem( std::string , std::string , std::string , int );
	std::string GetItem( int , int );
	void ClearAll();
	int CountItems();
	bool OperatorWith(SearchResults* what, int mode);
	bool HasItem(Triple& what){ return results.find(what) != -1; }
	Triple GetItemDirect(int n) {return results[n];}
	void AddItem( Triple d ){ AddItem(d.grp,d.itm,d.val,1); }
};

typedef void (__stdcall* SUPERINICALLBACK)(void*, char);


struct SuperINI_SettingsStruct
{
	bool bAutoSave;
	bool CaseSensitive;
	bool EscapeCharsInValue;
	bool EscapeCharsInItem;
	bool EscapeCharsInGroups;
	bool NeverQuoteStrings;
	bool ReadOnly;
	bool StandardNewLines;
	std::string NewLineChar;
	std::string autoSavePath;
	bool AutoSaveEncrypted;
	std::string AutoSaveKey;
	bool AutoSaveCompressed;
	int		groupRepeatSetting;
	int		itemRepeatSetting;
	bool	saveRepeats;
	void* rdPtrFavorite;
	bool	subGroups;
	bool allowEmptyGroup;
};


class SuperINI
{
public:

	//EXTRA MEMBERS REQUIRED FOR THE MMF EXTENSION
	// (These are NOT used in the class)

	//In normal mode:
	//	rdPtr contains one item, the rdPtr.
	//	rdPtrFavorite also has this value.
	//In Global mode:
	//	rdPtr contains the rdPtr of *all* 
	DynamicArrayGeneral<void*> rdPtr;
	void*				rdPtrFavorite;
	SUPERINICALLBACK cb;


	SuperINI();
	~SuperINI()
	{	
		undoLimit = redoLimit = 0;
		Close();
	
		ClearStacks(true,true);
	}


	//File operations
	void undo();
	void redo();

	//Existance functions
	bool HasItem( std::string , std::string );
	bool HasGroup( std::string );

	// Read and write functions
	bool SetItem( std::string , std::string , std::string );
	std::string GetItem( std::string , std::string , std::string );
	bool SetItemMD5( std::string , std::string , std::string );
	bool CompareItemMD5( std::string , std::string , std::string , std::string );
	bool SetItemArray(std::string , std::string , int , std::string);
	std::string GetItemArray(std::string  , std::string  , int  , std::string );
	// Copying functions
	bool CopyGroup( std::string , std::string , bool );
	bool CopyItem( std::string , std::string , std::string , std::string );

	// Deleting functions
	bool DeleteGroup( std::string );
	bool DeleteItem( std::string , std::string );
	bool DeleteItemFromAllGroups( std::string );
	bool DeleteByPattern( std::string grpCondition , std::string itemCondition , std::string valueCondition , bool caseSensitive);
	bool ClearAll();

	// Renaming functions
	bool RenameGroup( std::string , std::string , int  );
	bool RenameItem( std::string , std::string , std::string , bool );
	bool MoveItem( std::string , std::string , std::string , std::string , bool );
	bool MoveItemToGroup( std::string , std::string , std::string , bool );

	// Listing functions
	std::string GetNthGroupName( int n , std::string );
	std::string GetNthItemName( std::string , int n  , std::string );
	std::string GetNthItemValue( std::string , int n  , std::string );
	std::string GetNthGeneralItemName( int n );
	
	//Searching functions
	bool PerformSearch( SearchResults* out , std::string grpCondition , std::string itemCondition , std::string valueCondition , int bitMask , int mode );
	bool CreateSubINI( SuperINI* out , std::string grpCondition , std::string itemCondition , std::string valueCondition , bool caseSensitive );
	bool PerformMultiplesSearch( SearchResults* out , std::string grp , std::string item );
	bool FindSubgroups( SearchResults* out , std::string grp , int levels );

	// Merging functions
	bool MergeGroup( SuperINI* x , std::string grpSource , std::string grpDest , bool allowOverwrite );
	bool Merge( SuperINI* , bool );

	// Information functions
	int GroupCount();
	int ItemCount( std::string );
	int TotalItemCount( );
	int GetNthGeneralItemNameCount( );

	//Settings functions
	bool SetCaseSensitivity( bool _CaseSensitive );
		
	// Disk Functions
	bool LoadStream( std::string data , bool update , bool clear );
	std::string WriteStream( );
	bool LoadFromFile( std::string path , bool compressed , bool encrypted , std::string key );
	bool SaveToFile( std::string path , bool compressed , bool encrypted , std::string key );
	

	std::string WriteCSVArray( std::string seperator );
	std::string WriteCSV( std::string grp , std::string seperator );

	bool ExportAsCSV( std::string path , std::string grp , std::string seperator );
	bool ExportCSVArray( std::string path , std::string seperator );

	bool ImportCSVLine( std::string line , std::string grp , std::string seperator );
	bool ImportCSV( std::string line , std::string grp , std::string seperator );
	bool ImportCSVArray( std::string path , std::string seperator );
	
	bool Load( std::string path , bool changeAutoSave = true );
	bool LoadInAddition( std::string path );
	bool SaveAs( std::string path );
	bool Save( );
	bool Close();
	bool New( std::string , bool );
	bool RequestAutoLoad( );
	void SetAutoSavePath( std::string x ){ autoSavePath = x; }

	void forceNewUndoBlock();

	std::string GetFileName()
	{
		return autoSavePath;
	}
	void setManualMode( bool b ) {
		autoAddUndoStage = !b;
		if ( !b )
			newUndoBlock();
	}
//=====================================================================

	//@return negative if l<r, 0 if l equals r, positive if l>r
	int WhichGroupIsFirst( std::string _a , std::string _b )
	{
		int a = ordering.find(_a);
		int b = ordering.find(_b);
		if ( a < b ) return -1;
		if ( a == b ) return 0;
		return 1;
	}
	int WhichItemIsFirst( std::string grp , std::string _a , std::string _b )
	{
		INIMap::iterator groupIter  = data.find( grp );
		if ( groupIter == data.end() ) return 0;
		DynamicArray* da = ((groupIter->second).ordering);
		int a = da->find(_a);
		int b = da->find(_b);
		if ( a < b ) return -1;
		if ( a == b ) return 0;
		return 1;
	}

	//Extra feature
	std::string HashMD5( std::string );
	bool WildcatCompare( std::string , std::string , bool );
	std::string ToggleEncryption( std::string data , std::string key );
	std::string EscapeCharacters( std::string what )
	{
		return ApplyEscapes( what , true , false , false );
	}
	std::string UnEscapeCharacters( std::string what )
	{
		return ValueEscapeCharacters(what,1);
	}
	bool CompressTo( std::string source , std::string dest );
	bool UncompressTo( std::string source , std::string dest );


	//These internal functions are actually external, but don't abuse them!
	bool PauseAutoSave( bool );
	bool RequestAutoSave();
	std::string toString( int x );

	//Settings functions:
	void SetNewLineCharacter( std::string to ){
		NewLineChar = to;
		if ( to != "" )
			StandardNewLines = false;
		else
			StandardNewLines = true;
	}
	void SetStandardNewLines(){
		StandardNewLines = true;
	}
	void SetSubGroups(bool x,bool y){
		subGroups = x;
		allowEmptyGroup = y;
		if (!y)
		{
			RemoveEmptyGroups();
			RequestAutoSave();
		}
	}
	void EscapeCharacters( bool grp , bool itm , bool val ){
		EscapeCharsInValue = val;
		EscapeCharsInItem = itm;
		EscapeCharsInGroups = grp;		
	}
	void SetReadOnly( bool la ){
		ReadOnly = la;
	}
	bool GetReadOnly(){
		return ReadOnly;
	}
	void SetRepeatModes( int grp , int itm , bool save ){
		groupRepeatSetting = grp;
		itemRepeatSetting = itm;
		saveRepeats = save;
	}

	void SetAutosaveProperties( int compress , int encrypt , std::string key )
	{
		if ( encrypt != -1 )
		{
			AutoSaveEncrypted = encrypt;
			AutoSaveKey = key;
		}
		if ( compress != -1 )
			AutoSaveCompressed = compress;
	}
	void SetAutoSave(bool as ){
		bAutoSave = as;
	}
	void SetNeverQuoteStrings(bool a){
		NeverQuoteStrings = a;
	}

	void ExportSettings( SuperINI_SettingsStruct* settings );
	void ImportSettings( SuperINI_SettingsStruct* settings );
	void ResetSettings();

	void GroupEffect( std::string grp , int eff );
	void newUndoBlock();
	void AddDirectlyForUndo( std::string group , SMap map, DynamicArray* ordering );
	int	 undoLimit, redoLimit;
	void ClearStacks( bool undo , bool redo );

	bool hasUndo() { return !undoInformation.empty(); }
	bool hasRedo() { return !redoInformation.empty(); }

//==================
// The private structure
//==================
private:

	INIMap	data;
	CMD5	md5;

	DynamicArray ordering;
	void ForceGroupExist( std::string grp );

	bool bAutoSave;
	bool bAutoSavePaused;
	bool CaseSensitive;
	bool EscapeCharsInValue;
	bool EscapeCharsInItem;
	bool EscapeCharsInGroups;
	bool NeverQuoteStrings;
	bool ReadOnly;
	bool StandardNewLines;
	bool inRedo;

	std::string NewLineChar;
	std::string autoSavePath;
	bool AutoSaveEncrypted;
	std::string AutoSaveKey;
	bool AutoSaveCompressed;

	//For the following:
	//		0		Take first value	{mmf default}
	//		1		Take last value
	//		2		Rename as [NAME].# , where # is a number
	//		3		Merge (For groups only)
	int		groupRepeatSetting;
	int		itemRepeatSetting;
	bool	saveRepeats;
	bool	subGroups;
	bool	allowEmptyGroup;

	//Internal functions
	void lc( std::string& x);
	void Strip( std::string& );
	bool SplitSubjectToEscapes( std::string what , std::string& one , std::string& two );
	std::string lcret( std::string x );
	std::string ValueEscapeCharacters( std::string what ,  int force );// -1 Force not to, 0 no force, 1 force to.
	
	std::string ApplyEscapes( std::string what , bool forReal , bool spaces , bool equals );
	bool StringReplace( std::string &str , std::string pat , std::string with );
	std::string GetNewLine()
	{
		if (StandardNewLines)
			return STANDARD_NEW_LINE;
		else
			return NewLineChar;
	}
	std::string WriteCVSLine( std::string grp , std::string seperator);
	std::string GetNthGroupNameInternal( int n ,std::string def );
	std::string GetNthItemNameInternal( std::string group , int n , std::string def );
	//std::string lcretALWAYS(std::string x );
	bool CollapseToLowercaseKey( std::string grpName );
	bool UnCollapseToPerfered( std::string grpName );
	bool RemoveEmptyGroups();
	bool AddGroupWithNoItems(std::string);
	void pushUndo( UndoElement* element );
	UndoStack undoInformation;
	UndoStack redoInformation;
	bool useRedoStack;
	bool autoAddUndoStage;


};


typedef	std::map<std::string,SuperINI*>		GlobalMap;
class GlobalObjectManager
{
public:
	GlobalObjectManager()
	{
	}
	~GlobalObjectManager()
	{
		GlobalMap::iterator x;
		for ( x = internal.begin() ; x != internal.end() ; x++ )
		{
			delete (*x).second;
		}

	}
	SuperINI* get( std::string n )
	{
		GlobalMap::iterator  x = internal.find(n);
		if ( x != internal.end() )
			return (*x).second;

		internal.insert( GlobalMap::value_type( n,new SuperINI()));
		return internal[n];
	}
	int size(){ return internal.size(); }
	std::string slot(int n){
		GlobalMap::iterator x = internal.begin();
		while (n != 0 ){ x++;n--; }	
		return ((x)->first);
	}
private:
	GlobalMap internal;
};

class UndoSetItem : public UndoElement {

	std::string group;
	std::string item;
	std::string value;

public:
	UndoSetItem( std::string g , std::string i , std::string v ) : group(g), item(i), value(v) { }

	virtual void perform( SuperINI* ini ) {
		ini->SetItem(group,item,value);
	}
};
class UndoCreateItem : public UndoElement {

	std::string group;
	std::string item;

public:
	UndoCreateItem( std::string g , std::string i ) : group(g), item(i) { }

	virtual void perform( SuperINI* ini ) {
		ini->DeleteItem(group,item);
	}
};


class UndoGroupMake : public UndoElement {

	std::string group;

public:
	UndoGroupMake( std::string g ) : group(g){ }

	virtual void perform( SuperINI* ini ) {
		ini->DeleteGroup(group);
	}
};

class UndoByRestoreGroup : public UndoElement {

	std::string group;
	SMap map;
	DynamicArray* ordering;


public:
	UndoByRestoreGroup( INIMap::iterator iter , std::string g ) : group(g) {
		map = iter->second.map;
		ordering = iter->second.ordering->Copy();
	}

	virtual void perform( SuperINI* ini ) {
		ini->AddDirectlyForUndo(group,map,ordering);
	}
	~UndoByRestoreGroup() {
		delete ordering;
	}
};




#endif
