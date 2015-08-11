
// ===========================================================================
void FloatToString( char* stringVersion , float p3f );
std::string FormatPathDo( std::string defaultDir , std::string x , bool folderMake );
void LoadGlobals( LPMV pMv, LPRDATA rdPtr , std::string group );
void SaveGlobals( LPMV pMv, LPRDATA rdPtr, std::string group );
void iLoadObject( LPRDATA rdPtr , std::string grp , LPRO obj );
void iSaveObject( LPRDATA rdPtr , std::string grp , LPRO p1 , int p2 , int p3 );



LPRDATA FindObject(LPCSTR pName , LPRDATA rdPtr, long ID , const char* errorMessage  );
LPRDATA FindObjectNotThis(LPCSTR pName , LPRDATA rdPtr, const char* errorMessage  );

#if !defined(RUN_ONLY)

	#define FINDOBJECT(pName,ID,errorMessage) FindObject(pName,rdPtr,ID,errorMessage);
	#define FINDOBJECTNOTTHIS(pName,errorMessage) FindObjectNotThis(pName,rdPtr,errorMessage);

#else

	#define FINDOBJECT(pName,ID,errorMessage) FindObject(pName,rdPtr,ID,NULL);
	#define FINDOBJECTNOTTHIS(pName,errorMessage) FindObjectNotThis(pName,rdPtr,NULL);


#endif


float StringToFloat( const char* str );

struct SortGroup_ItemPair
{
	std::string itemName;
	std::string	itemValue;
	int sort_type;

	bool operator==( SortGroup_ItemPair a ) const
	{
		return (itemName == a.itemName && itemValue == a.itemValue );
	}
};

typedef DynamicArrayGeneral<SortGroup_ItemPair>		ItemPairHolder;

ItemPairHolder SortGroup( LPRDATA rdPtr , std::string grpName , int mode , int sort_type );
ItemPairHolder GetGroup( LPRDATA rdPtr , std::string grpName );