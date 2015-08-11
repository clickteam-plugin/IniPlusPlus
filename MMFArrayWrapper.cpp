#include	"common.h"
#include "MMFArrayWrapper.h"
#include "helper_to_main.h"
#include	"cncf.h"


MMFArray* FindArrayObject(LPCSTR pName , LPRDATA rdPtr )
{

	LPRDATA r = NULL;
	#ifdef RUN_ONLY
		r = FindObject(pName,rdPtr,0x12fd53a0,NULL);
	#else
		r = FindObject(pName,rdPtr,0x12fd53a0,"Error: The Array object you have selected for use with either the 'Load from Array' or 'Save to Array' action does not exist (or is not a valid array object). The action will fail.\n\nThis error message will not be shown when the program is built." );
	#endif
	if ( r != 0 )
		return new MMFArray((ARRAYOBJ*)r);
	return NULL;
}


std::string MMFArray::CoordinatesToText( int x , int y , int z )
{
	int offset = IsBase1()?1:0 ;
	char tx[10],ty[10],tz[10];
	_itoa(x+offset,tx,10); _itoa(y+offset,ty,10); _itoa(z+offset,tz,10);
	return std::string(tx) + "," + std::string(ty) + "," + std::string(tz);
}



//TODO: STRINGS, SHIFT INDEX, CHECK IN BOUNDS
void MMFArray::Set(int x, int y, int z,std::string val)
{
	if ( IsText() )
		SetText(x,y,z,val);
	else if ( IsNumber() )
		SetNum(x,y,z,atoi(val.c_str()));
}
void MMFArray::SetText(int x, int y, int z,std::string val)
{
	return;
	/*
	//Not sure yet!
	LPSTR* offset = (OffsetT() + GetW()*GetH()*z + GetW()*y + x);
//	free(*offset);
	*offset = (LPSTR)malloc((val.length()+1)*sizeof(char));
	strcpy( *offset , val.c_str() );
	*/


}
void MMFArray::SetNum(int x, int y, int z,int val)
{
	*(OffsetN() + GetW()*GetH()*z + GetW()*y + x) = val;
}