
typedef struct ARRAYHEADER
{
	long			lDimensionX;  // current size
	long			lDimensionY;
	long			lDimensionZ;

	long			lFlags;  // numeric/text, etc

	long			lIndexA; // current index
	long			lIndexB;
	long			lIndexC;

	long			lArraySize;

} ARRAYHEADER;
typedef ARRAYHEADER *LPARRAYHEADER;

typedef struct TAGArrayObj
{
	headerObject 	rHo;

	LPARRAYHEADER	pArray;

} ARRAYOBJ;

#define	ARRAY_TYPENUM	0x0001 // numeric array
#define	ARRAY_TYPETXT	0x0002 // text array
#define INDEX_BASE1		0x0004 // 1-based indexes

class MMFArray
{
	ARRAYOBJ* inst;

	DWORD* OffsetN(){ return (DWORD *)(inst->pArray + 1); }
	LPSTR* OffsetT(){ return (LPSTR*)(&(inst->pArray)[1]); }

public:
	MMFArray( ARRAYOBJ* _inst ) : inst(_inst) {}
	long GetW(){ return inst->pArray->lDimensionX; }
	long GetH(){ return inst->pArray->lDimensionY; }
	long GetD(){ return inst->pArray->lDimensionZ; }

	bool IsNumber(){ return ((inst->pArray->lFlags & ARRAY_TYPENUM) != 0); }
	bool IsText(){ return ((inst->pArray->lFlags & ARRAY_TYPETXT) != 0); }
	bool IsBase1(){ return ((inst->pArray->lFlags & INDEX_BASE1) != 0); }

	LPSTR GetText(int x,int y,int z)
	{
		if ( !IsText() )
			return NULL;
		return *(OffsetT() + GetW()*GetH()*z + GetW()*y + x);
	}
	long GetNum(int x,int y,int z)
	{
		if ( !IsNumber() )
			return 0;
		return *(OffsetN() + GetW()*GetH()*z + GetW()*y + x);

	}
	std::string Get(int x, int y, int z)
	{
		if ( IsText() )
		{
			LPSTR str = GetText(x,y,z);
			if ( str != 0 )
				return std::string(str);
			return "";
		}
		long n = GetNum(x,y,z);
		char buf[25];
		_itoa(n,buf,10);
		std::string tmp = std::string(buf);
		return tmp;
	}
	bool Has(int x, int y, int z )
	{
		if ( IsText() && GetText(x,y,z) == 0 )
			return false;
		return true;
	}
	std::string CoordinatesToText( int x , int y , int z );
	void Set(int x, int y, int z,std::string val);
	void SetText(int x, int y, int z,std::string val);
	void SetNum(int x, int y, int z,int val);

};

MMFArray* FindArrayObject(LPCSTR pName , LPRDATA rdPtr );