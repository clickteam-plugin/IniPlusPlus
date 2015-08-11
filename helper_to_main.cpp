#include	"common.h"
#include	"helper_to_main.h"
#include	<algorithm>
//#include	"cncf.h"

//============================================================================
//
// Common Functions
// 
// ===========================================================================

//#define EF_ISUNICODE		113		// Returns TRUE if the editor or runtime is in Unicode mode
//#define EF_GETAPPCODEPAGE	115		// Returns the code page of the application
//__inline BOOL mvIsUnicodeVersion(LPMV mV) \
//	{ return mV->mvCallFunction(NULL, EF_ISUNICODE, (LPARAM)0, (LPARAM)0, (LPARAM)0); }
//__inline int mvGetAppCodePage(LPMV mV, LPVOID pApp) \
//	{ return mV->mvCallFunction(NULL, EF_GETAPPCODEPAGE, (LPARAM)pApp, (LPARAM)0, (LPARAM)0); }


void toMultiByte( LPRDATA rdPtr, const char* source , int nchars, char* dest ) {

	LPMV pMV = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv;
	if ( mvIsUnicodeVersion(pMV) )
	{
		LPCWSTR pNameW = (LPCWSTR)source;
		WideCharToMultiByte(mvGetAppCodePage(pMV,rdPtr->rHo.hoAdRunHeader->rhApp), 0, pNameW, nchars, dest, nchars+1, NULL, NULL);
	}
	else
		strncpy(dest,(char *)source,nchars);
}

void writeTo( LPRDATA rdPtr, const char* source , int nchars, char* dest ) {

	LPMV pMV = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv;
	if ( mvIsUnicodeVersion(pMV) )
		MultiByteToWideChar(mvGetAppCodePage(pMV,rdPtr->rHo.hoAdRunHeader->rhApp), 0, (LPCSTR)source , -1, (LPWSTR)dest, nchars );
	else
		strncpy(dest,(char *)source,nchars);
}

// Takes a char* and returns it either as a char* or a LPWSTR, as required.
char* convertToCorrectForm( LPRDATA rdPtr, const char* source , int* lenBytes  ) {

	LPMV pMV = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv;
	if ( !mvIsUnicodeVersion(pMV) ) {
		int size = strlen(source)+1;
		if ( lenBytes != 0 )
			(*lenBytes) = size;
		char* buffer = (char*)mvMalloc(pMV,size);
		strcpy(buffer,source);
		return buffer;
	}

	int size = MultiByteToWideChar(mvGetAppCodePage(pMV,rdPtr->rHo.hoAdRunHeader->rhApp), MB_PRECOMPOSED, (LPCSTR)source , -1, 0, 0)+1;
	if ( lenBytes != 0 )
		(*lenBytes) = 2*size;
	char* buffer = (char*)mvMalloc(pMV,size*2);
	MultiByteToWideChar(mvGetAppCodePage(pMV,rdPtr->rHo.hoAdRunHeader->rhApp), MB_PRECOMPOSED, (LPCSTR)source , -1, (LPWSTR)buffer, size);
	return buffer;
}




// Takes the char* or LPWSTR, as required, and returns it either as a char*
char* toMultiByteForm( LPRDATA rdPtr, const char* source ) {

	LPMV pMV = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv;
	if ( !mvIsUnicodeVersion(pMV) ) {
		int size = strlen(source)+1;
		char* buffer = (char*)malloc(size);
		strncpy(buffer,source,size);
		buffer[size-1] = 0;
		return buffer;
	}

	int size = 	WideCharToMultiByte(mvGetAppCodePage(pMV,rdPtr->rHo.hoAdRunHeader->rhApp), 0, (LPWSTR)source , -1, 0, 0 , NULL, NULL) + 2;
	char* buffer = (char*)malloc(size);
	WideCharToMultiByte(mvGetAppCodePage(pMV,rdPtr->rHo.hoAdRunHeader->rhApp), 0, (LPWSTR)source , -1, buffer, size, NULL, NULL );
	return buffer;
		
}


//TODO: This only gives back 6 digits or something. NOT GOOD ENOUGH!
float StringToFloat( const char* str )
{
	return atof(str);
}

void FloatToString( char* stringVersion , float p3f )
{
		sprintf( stringVersion , "%0.6f" , p3f );
		while ( stringVersion[ strlen(stringVersion)-1 ] == '0' )
			stringVersion[ strlen(stringVersion)-1] = 0;

		if ( stringVersion[ strlen(stringVersion)-1 ] == '.' )
			stringVersion[ strlen(stringVersion)-1] = 0;	
}



std::string FormatPathDo( std::string defaultDir , std::string x , bool folderMake )
{
	if ( PathIsRelative( x.c_str() ))
	{
		if ( defaultDir[defaultDir.length()-1] != '\\' && defaultDir[defaultDir.length()-1] != '/' )
			defaultDir = defaultDir + "\\";

		if ( x[0] == '\\' || x[0] == '/' )
			x = x.substr( 1 );
		x = defaultDir + x;
	}
	char tmp[MAX_PATH];
	PathCanonicalize( tmp , x.c_str() );


	char root[MAX_PATH];
	lstrcpyn( root , tmp , MAX_PATH );
	PathStripToRoot( root );

	char dir[MAX_PATH];
	lstrcpyn( dir , tmp , MAX_PATH );
	PathRemoveFileSpec( dir );
	std::string folder = dir;
	PathSkipRoot( dir );

	if ( GetFileAttributes( folder.c_str() ) != FILE_ATTRIBUTE_DIRECTORY && folderMake )
	{
		char* pathArray[100];
		int pos = 0;
		
		char* next = dir;
		while ( pos != 100 && ((next = PathFindNextComponent( next )) != NULL) )
			pathArray[pos++] = next;
		int pos2 = 0;
		for ( ; pos2 != pos ; pos2++ )
		{
			std::string subPath = folder;
			subPath = subPath.substr( 0 , (subPath.length()) - (((std::string)pathArray[pos2]).length()));
			CreateDirectory( subPath.c_str() , NULL );
		}
	}
	return (std::string)tmp;
}

//============================================================================
//
// Global Values and Strings functions
// 
// ===========================================================================
#define	szEmptyString	0

__inline CRunApp* GetGlobalValuesApp(CRunApp* pApp)
{
	while (pApp->m_bShareGlobalValues)
	{
		if ( pApp->m_pParentApp != NULL )
			pApp = pApp->m_pParentApp;
	}
	return pApp;
}

int	 GetNumberOfGlobalValues( CRunApp* pApp )
{
	pApp = GetGlobalValuesApp(pApp);
	return pApp->m_nGlobalValues;
}

BOOL SetNumberOfGlobalValues( LPMV pMv , CRunApp* pApp, int nValues)
{
	if ( pApp == NULL )
		return FALSE;

	// Get application that contains shared global values
	pApp = GetGlobalValuesApp(pApp);
	if ( nValues < pApp->m_nGlobalValues )
		return TRUE;

	// Re-allocate values array
	CValue* pVal;
	if (pApp->m_pGlobalValues==NULL)
		pVal=(CValue*)mvMalloc(pMv,nValues * sizeof(CValue));
	else
		pVal = (CValue*)mvReAlloc (pMv,pApp->m_pGlobalValues, nValues * sizeof(CValue));
	if ( pVal == NULL )
		return FALSE;

	pApp->m_pGlobalValues = pVal;

	// Reset new values
	for (int n=pApp->m_nGlobalValues; n<nValues; n++)
	{
		pApp->m_pGlobalValues[n].CValue::CValue();
	}

	// Update number of values
	pApp->m_nGlobalValues = nValues;

	return TRUE;
}

//In Yves' code, this was just something with 0 as a long value.
//But I am far more lazy...
#define		RETURN_DUMMY		0

CValue* GetGlobalValueAt(LPMV pMv ,CRunApp* pApp, int nIdx)
{
	if ( pApp == NULL )
		return RETURN_DUMMY;

	pApp = GetGlobalValuesApp(pApp);
	if ( nIdx < 0 )
		return RETURN_DUMMY;
	
	if ( nIdx >= pApp->m_nGlobalValues )
	{
		if (!SetNumberOfGlobalValues(pMv,pApp, nIdx+1))
		{
			return RETURN_DUMMY;
		}
	}
	return &pApp->m_pGlobalValues[nIdx];
}

void SetGlobalValueAt( LPMV pMv , CRunApp* pApp, int nIdx, CValue* pValue)
{

	if ( nIdx < 0 || pApp == NULL )
		return;
	pApp = GetGlobalValuesApp(pApp);
	if ( nIdx >= pApp->m_nGlobalValues )
	{
		if ( !SetNumberOfGlobalValues(pMv, pApp, nIdx+1) )
			return;
	}
	pApp->m_pGlobalValues[nIdx].m_type = pValue->m_type;
	//I shouldn't need to case, should it? But for some reason it only works
	//if I do. (as both unions, i should be able to look directly at the memory address)
	if ( pValue->m_type == TYPE_LONG )
		pApp->m_pGlobalValues[nIdx].m_long = pValue->m_long;
	if ( pValue->m_type == TYPE_FLOAT )
		pApp->m_pGlobalValues[nIdx].m_double = pValue->m_double;

}


__inline CRunApp* GetGlobalStringsApp(CRunApp* pApp)
{
	while (pApp->m_bShareGlobalStrings)
	{
		if ( pApp->m_pParentApp != NULL )
			pApp = pApp->m_pParentApp;
	}
	return pApp;
}

int	 GetNumberOfGlobalStrings( CRunApp* pApp )
{
	pApp = GetGlobalStringsApp(pApp);
	return pApp->m_nGlobalStrings;
}


LPCSTR GetGlobalStringAt(CRunApp* pApp, int nIdx)
{
	if ( pApp == NULL )
		return szEmptyString;
	pApp = GetGlobalStringsApp(pApp);
	if ( nIdx < 0 || nIdx >= pApp->m_nGlobalStrings || pApp->m_pGlobalString == NULL )
		return szEmptyString;
	LPCSTR ps = pApp->m_pGlobalString[nIdx];
	if ( ps == NULL )
		ps = szEmptyString;
	return ps;
}

void SetGlobalStringAt( LPMV pMv , CRunApp* pApp, int nIdx, LPCSTR ps, int stringLenBytes )
{
	if ( ps == NULL )
		ps = szEmptyString;
	if ( nIdx < 0 || pApp == NULL )
		{ return; }

	// No global string yet?
	pApp = GetGlobalStringsApp(pApp);
	if ( pApp->m_nGlobalStrings == 0 )
	{
		pApp->m_pGlobalString = (LPSTR*)mvCalloc(pMv,(nIdx+1) * sizeof(LPSTR) );
		if ( pApp->m_pGlobalString != NULL )
		{
			pApp->m_nGlobalStrings = nIdx+1;

			pApp->m_pGlobalString[nIdx] = (LPSTR)mvMalloc(pMv, stringLenBytes );
			if ( pApp->m_pGlobalString[nIdx] != NULL )
				memcpy( pApp->m_pGlobalString[nIdx], ps , stringLenBytes );
			//	strcpy(pApp->m_pGlobalString[nIdx], ps);
		}
	}
	else
	{
		// Realloc?
		if ( nIdx >= pApp->m_nGlobalStrings )
		{
			LPBYTE pb = (LPBYTE)mvReAlloc(pMv, pApp->m_pGlobalString, (nIdx+1) * sizeof(LPSTR));
			if ( pb == NULL )
				return;
			pApp->m_pGlobalString = (LPSTR*)pb;
			for (int i=pApp->m_nGlobalStrings; i<=nIdx; i++)
				pApp->m_pGlobalString[i] = NULL;
			pApp->m_nGlobalStrings = nIdx+1;
		}

		// Free existing string
		mvFree(pMv,pApp->m_pGlobalString[nIdx]);

		// Alloc and copy string
		pApp->m_pGlobalString[nIdx] = (LPSTR)mvMalloc(pMv, stringLenBytes);
		if ( pApp->m_pGlobalString[nIdx] != NULL )
			memcpy( pApp->m_pGlobalString[nIdx], ps , stringLenBytes );
		//	strcpy(pApp->m_pGlobalString[nIdx], ps);
	}
}

#define STR(x)	rdPtr->ini->toString(x)
void LoadGlobals( LPMV pMv , LPRDATA rdPtr , std::string group )
{
	LPRH rhPtr;
	rhPtr = rdPtr->rHo.hoAdRunHeader;
	CRunApp* myCRunApp = rhPtr->rhApp;

	//Load the global values.
	int n;
	int max = atoi(rdPtr->ini->GetItem( group , "vals" , "-1").c_str() );
	for ( n = 0 ; n < max ; n++ )
	{
		if ( rdPtr->ini->HasItem( group , "val" + STR(n) ) )
		{
			CValue ivalue;
			std::string value = rdPtr->ini->GetItem( group , "val" + STR(n) , "0" );
			if ( value.find(".") == -1 )
			{
				long answer = atoi( value.c_str() );
				ivalue.m_long = answer;
				ivalue.m_type = TYPE_INT;
			}
			else
			{
				float answer = atof(value.c_str());
				ivalue.m_type = TYPE_FLOAT;		
				ivalue.m_double = answer;		
			}	
			SetGlobalValueAt( pMv ,myCRunApp, n, &ivalue);
		}
	}

	max = atoi(rdPtr->ini->GetItem( group , "strs" , "-1").c_str() );
	LPMV mV = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv;
	for ( n = 0 ; n < max ; n++ )
	{
		if ( rdPtr->ini->HasItem( group , "str" + STR(n) ) ) {

			int stringLenBytes = 0;
			char* buffer = convertToCorrectForm(rdPtr, rdPtr->ini->GetItem( group , "str" + STR(n) , "" ).c_str(), &stringLenBytes );
			SetGlobalStringAt(pMv,myCRunApp, n,buffer, stringLenBytes );
			mvFree(mV,buffer);
		}

	}

}

void SaveGlobals( LPMV pMv , LPRDATA rdPtr, std::string group )
{
	int n;
	LPRH rhPtr;
	rhPtr = rdPtr->rHo.hoAdRunHeader;
	CRunApp* myCRunApp = rhPtr->rhApp;

	rdPtr->ini->PauseAutoSave(true);
	rdPtr->ini->SetItem( group , "vals" , STR(GetNumberOfGlobalValues( myCRunApp )));
	for ( n = 0 ; n < GetNumberOfGlobalValues( myCRunApp ) ; n++ )
	{
		CValue* data = GetGlobalValueAt(pMv,myCRunApp, n);
		if ( data->m_type == TYPE_LONG )
			rdPtr->ini->SetItem( group , "val" + STR(n) , STR(data->m_long) );
		else
		{
			char stringVersion[120];
			FloatToString( stringVersion , data->m_double );
			rdPtr->ini->SetItem( group , "val" + STR(n) , stringVersion );
		}
	}
	rdPtr->ini->SetItem( group , "strs" , STR(GetNumberOfGlobalStrings( myCRunApp )));
	for ( n = 0 ; n < GetNumberOfGlobalStrings( myCRunApp ) ; n++ )
	{

		char* buffer = toMultiByteForm(rdPtr, GetGlobalStringAt(myCRunApp, n) );
		rdPtr->ini->SetItem( group , "str" + STR(n) , std::string(buffer) );
		free(buffer);

		//rdPtr->ini->SetItem( group , "str" + STR(n) , GetGlobalStringAt(myCRunApp, n) );
	}
	rdPtr->ini->PauseAutoSave(false);
	rdPtr->ini->RequestAutoSave();
}




void iSaveObject( LPRDATA rdPtr , std::string grp , LPRO p1 , int p2 , int p3 )
{
	rdPtr->ini->PauseAutoSave(true);
	if ( p2 )
	{
		rdPtr->ini->SetItem( grp , "posx" 
			, rdPtr->ini->toString(	p1->roHo.hoX) );
		rdPtr->ini->SetItem( grp , "posy" 
			, rdPtr->ini->toString(p1->roHo.hoY ));

		if ( OEFLAG_MOVEMENTS & p1->roHo.hoOEFlags )
		{
			rdPtr->ini->SetItem( grp , "dir" 
				, rdPtr->ini->toString(	p1->roc.rcDir)  );
			rdPtr->ini->SetItem( grp , "spd" 
				, rdPtr->ini->toString(	p1->roc.rcSpeed)  );
			rdPtr->ini->SetItem( grp , "ani" 
				, rdPtr->ini->toString(	p1->roc.rcAnim)  );
			rdPtr->ini->SetItem( grp , "frame" 
				, rdPtr->ini->toString(	p1->roc.rcImage)  );
		}
	}

	if ( p3 && (p1->roHo.hoOEFlags & OEFLAG_VALUES))
	{
		rVal* rov = (rVal*)((LPBYTE)&p1->roHo + p1->roHo.hoOffsetValue);
		int n = 0;
			rdPtr->ini->SetItem( grp , "flags" 
				, rdPtr->ini->toString(rov->rvValueFlags) );
		for ( n = 0 ; n < VALUES_NUMBEROF_ALTERABLE ; n++ )
		{
			std::string x = rdPtr->ini->toString(n);
			if ( x.length() == 1 ) x = "0" + x;

			if (  rov->rvpValues[n].m_type == TYPE_INT )
				rdPtr->ini->SetItem( grp , "val" + x , rdPtr->ini->toString(rov->rvpValues[n].m_long) );
			else if (  rov->rvpValues[n].m_type == TYPE_FLOAT )
			{
				char MyString[120];
				FloatToString( MyString , rov->rvpValues[n].m_double );
				rdPtr->ini->SetItem( grp , "val" + x ,  MyString );
			}
		}
		for ( n = 0 ; n < STRINGS_NUMBEROF_ALTERABLE ; n++ )
		{
			std::string x = rdPtr->ini->toString(n);
			if ( x.length() == 1 ) x = "0" + x;
			if( rov->rvStrings[n] != 0 )
			{
				// It might be unicode in memory, or it might not. But buffer will always be UTF8
				char* buffer = toMultiByteForm(rdPtr,rov->rvStrings[n]);
				rdPtr->ini->SetItem( grp , "str" + x , std::string(buffer) );
				free(buffer);
			}
			else rdPtr->ini->SetItem( grp , "str" + x , "" );
		}
	}
	rdPtr->ini->PauseAutoSave(false);
	rdPtr->ini->RequestAutoSave();
}

void iLoadObject( LPRDATA rdPtr , std::string grp , LPRO obj )
{

	int newX,newY;
	if ( rdPtr->ini->HasItem( grp , "posx" ) )
		newX = atoi(rdPtr->ini->GetItem(grp,"posx","0").c_str());
	else
		newX = obj->roHo.hoX;

	if ( rdPtr->ini->HasItem( grp , "posy" ) )
		newY = atoi(rdPtr->ini->GetItem(grp,"posy","0").c_str());
	else newY = obj->roHo.hoY;

	( ((LPRH)obj->roHo.hoAdRunHeader)->rh4.rh4KpxFunctions[RFUNCTION_SETPOSITION].routine((LPHO)obj, newX, newY) );
	

	if ( obj->roHo.hoOEFlags & OEFLAG_MOVEMENTS)
	{
		if ( rdPtr->ini->HasItem( grp , "dir" ) )
			obj->roc.rcDir = atoi(rdPtr->ini->GetItem(grp,"dir","0").c_str());
		if ( rdPtr->ini->HasItem( grp , "spd" ) )
			obj->roc.rcSpeed = atoi(rdPtr->ini->GetItem(grp,"spd","0").c_str());
		if ( rdPtr->ini->HasItem( grp , "ani" ) )
			obj->roc.rcAnim = atoi(rdPtr->ini->GetItem(grp,"ani","0").c_str());
		if ( rdPtr->ini->HasItem( grp , "frame" ) )
			obj->roc.rcImage = atoi(rdPtr->ini->GetItem(grp,"frame","0").c_str());
		obj->roc.rcChanged = TRUE;
	}

	//Load the valXX items. Must decide if it is an integer or a float
	if ( obj->roHo.hoOEFlags & OEFLAG_VALUES )
	{
		rVal* rov = (rVal*)((LPBYTE)&obj->roHo + obj->roHo.hoOffsetValue);
		if ( rdPtr->ini->HasItem(grp,"flags") )
			rov->rvValueFlags = atoi(rdPtr->ini->GetItem( grp , "flags" , "0" ).c_str());
		int n = 0;
		for ( n = 0 ; n < VALUES_NUMBEROF_ALTERABLE ; n++ )
		{
			std::string x = rdPtr->ini->toString(n);
			if ( x.length() == 1 ) x = "0" + x;
			x = "val" + x;

			if ( rdPtr->ini->HasItem( grp , x ) )
			{
				std::string value = rdPtr->ini->GetItem( grp , x , "0" );
				if ( value.find(".") == -1 )
				{
					long answer = atoi( value.c_str() );
					rov->rvpValues[n].m_long = answer;
					rov->rvpValues[n].m_type = TYPE_INT;
				}
				else
				{
					float answer = atof(value.c_str());
					rov->rvpValues[n].m_double = answer;
					rov->rvpValues[n].m_type = TYPE_FLOAT;				
				}		
			}
		}
		
		//Now load the strXX items if they exist
		for ( n = 0; n < STRINGS_NUMBEROF_ALTERABLE ; n++ )
		{
			LPMV mV = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv;
			std::string x = rdPtr->ini->toString(n);
			if ( x.length() == 1 ) x = "0" + x;
			x = "str" + x;
			if ( rdPtr->ini->HasItem( grp , x ) )
			{
				std::string data = rdPtr->ini->GetItem(grp,x,"");
				char* old = rov->rvStrings[n];
				rov->rvStrings[n] = convertToCorrectForm(rdPtr,data.c_str(),0);
				if ( old != NULL )
					mvFree(mV,old);
			}
		}

	} 
}












LPRDATA FindObjectNotThis(LPCSTR pName , LPRDATA rdPtr, const char* errorMessage  )
{

	LPRDATA obj = FindObject(pName,rdPtr,IDENTIFIER,errorMessage);
	if ( ((LPRDATA)obj)->ini == rdPtr->ini ) {
		#if !defined(RUN_ONLY)
			MessageBox( NULL , "Error: The INI++ object you have selected for use is the calling object, or shares data with the calling object (i.e. they are both global on the same slot). The action will fail.\n\nThis error message will not be shown when the program is built." , "INI++ Error" , 0 );
		#endif // !defined(RUN_ONLY)
		return NULL;
	}
	return obj;
}

LPRDATA FindObject(LPCSTR pName , LPRDATA rdPtr, long ID , const char* errorMessage  )
{
	// List runtime objects
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	objectsList* pOL = rhPtr->rhObjectList;
	for (int nObjects=0; nObjects<rhPtr->rhNObjects; pOL++, nObjects++)	
	{
		// Skip deleted objects
		while(pOL->oblOffset==0)
			pOL++;

		// Valid object
		LPHO hoPtr = (LPHO)pOL->oblOffset;

		// Test if the identifier of the object is the same as the one of this extension
		if (hoPtr->hoIdentifier==ID)
		{
			// Compare the name
			LPOIL pOil=hoPtr->hoOiList;


			char ObjName[OINAME_SIZE+1];

			// CHANGE : in Unicode version the object's name is in unicode format, convert it to mbcs format
			// Note: this must be removed when a Unicode version of this extension will exist
			toMultiByte( rdPtr,  (char*)(pOil)->oilName , OINAME_SIZE, (char*)ObjName );
			ObjName[OINAME_SIZE] = '\0';

			if (strcmp(pName,ObjName)==0)
				return (LPRDATA)hoPtr;
		}
	}
	#if !defined(RUN_ONLY)
		MessageBox( NULL , errorMessage , "INI++ Error" , 0 );
	#endif // !defined(RUN_ONLY)
	return NULL;
}


int SortingFunction (const void* _X, const void* _Y)
		{return (Ini_Item_Compare(((SortGroup_ItemPair*)_X)->itemValue,((SortGroup_ItemPair*)_Y)->itemValue,((SortGroup_ItemPair*)_X)->sort_type)); }

int SortingFunction2 (const void* _X, const void* _Y)
		{return (Ini_Item_Compare(((SortGroup_ItemPair*)_X)->itemName,((SortGroup_ItemPair*)_Y)->itemName,((SortGroup_ItemPair*)_X)->sort_type)); }

int SortingFunctionFirstBit (const void* _X, const void* _Y)
{
	//This applies a standard compare on the bit before the comma on each
	std::string x,y;
	x = ((SortGroup_ItemPair*)_X)->itemValue;
	y = ((SortGroup_ItemPair*)_Y)->itemValue;
	DynamicArray xa = SplitByComma(x);
	DynamicArray ya = SplitByComma(y);
	return Ini_Item_Compare(xa[0],ya[0],((SortGroup_ItemPair*)_X)->sort_type);
}

ItemPairHolder SortGroup( LPRDATA rdPtr , std::string grpName , int mode , int sort_type )
{
	ItemPairHolder out;
	for ( int n = 1 ; n <= rdPtr->ini->ItemCount(grpName) ; n++ )
	{
		SortGroup_ItemPair x;
		x.itemName = rdPtr->ini->GetNthItemName(grpName,n,"");
		x.itemValue = rdPtr->ini->GetNthItemValue(grpName,n,"");
		x.sort_type = 2-sort_type;
		out.push_back( x );
	}
	if ( mode == 0 )
		out.sort( SortingFunction );
	if ( mode == 1 )
		out.sort( SortingFunction2 );
	if ( mode == 2 )
		out.sort( SortingFunctionFirstBit );
	return out;	
}









ItemPairHolder GetGroup( LPRDATA rdPtr , std::string grpName )
{
	ItemPairHolder out;
	for ( int n = 1 ; n <= rdPtr->ini->ItemCount(grpName) ; n++ )
	{
		SortGroup_ItemPair x;
		x.itemName = rdPtr->ini->GetNthItemName(grpName,n,"");
		x.itemValue = rdPtr->ini->GetNthItemValue(grpName,n,"");
		out.push_back( x );
	}
	return out;	
}




