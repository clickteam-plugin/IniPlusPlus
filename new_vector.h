#define STANDARD_NEW_LINE "\r\n"
#include <string>
#include "cfcFile.h"

//typedef std::string String;

//Needs:
//		size				OK
//		[ ]
//		push_back			OK
//		pop_back
//		empty				OK
//		clear

template< class String >
struct DynamicArrayGeneral
{
	DynamicArrayGeneral( const DynamicArrayGeneral& old )
	{
		elements = 0;
		dataelements = 0;
		EnforceSize(old.size());
		for ( int n = 0 ; n < old.size() ; n++ )
			set( n , old.get(n) );

	}
	bool empty(){ return elements==0; }
	void set(int x, const String& to )
	{
		EnforceSize(x+1);
		dataelements[x] = to;
	}

	DynamicArrayGeneral* Copy()
	{
		DynamicArrayGeneral<String>* out = new DynamicArrayGeneral<String>();
		EnforceSize(size());
		for ( int n = 0 ; n < size() ; n++ )
			out->set( n , get(n) );
		return out;
	}

	void push_back( const String& with )
	{
		set(elements,with);
	}
	int size() const { return elements; }
	void clear(){ if ( dataelements != 0 ){ delete [] dataelements; dataelements = 0;elements=0;} }
	DynamicArrayGeneral(){ dataelements = 0 ; elements = 0; }
	~DynamicArrayGeneral(){ clear(); }
	String get(int x) const
	{
		if ( x >= 0 && x < elements )
			return dataelements[x];
		return String();
	}
	String pop_back()
	{
		String tmp = get(elements-1);
		removeAt(elements-1);
		//EnforceSize(elements-1);
		return tmp;
	}
	String operator[](int pos) const
	{
		return get(pos);
	}

	void remove( const String& item )
	{
		int x = find(item);
		if ( x != -1 )
			removeAt( x );
	}
	int find( const String& item )
	{
		for ( int n = 0 ; n < size() ; n++ )
			if ( get(n) == item )
				return n;
		return -1;
	}
	void removeAt( int n )
	{
			if ( n < 0 && n >= elements )
				return;

			String* tmp_elements = new String[size()-1];

			int k;
			for ( k = 0 ; k < elements && k < n ; k++ )
				tmp_elements[k] = dataelements[k];
			for ( k = n+1 ; k < elements ; k++ )
				tmp_elements[k-1] = dataelements[k];

			delete [] dataelements;
			dataelements = tmp_elements;
			elements--;		
	}
	void sort(  int (__cdecl *compare )(const void *elem1, const void *elem2 ) )
	{
		qsort( dataelements , elements , sizeof(String) , compare );
	}
	void reverse()
	{
		for ( int n = 0 ; n != size()/2 ; n++ )
		{
			String tmp = get(n);
			set(n,get(size()-(n+1)));
			set(size()-(n+1),tmp);
		}

	}
	void swap( String a , String b )
	{
		int index1 = find(a);
		int index2 = find(b);
		if ( index1 == -1 || index2 == -1 ) return;
		set(index1,b);
		set(index2,a);
	}
	void EnforceSize(int n)
	{
		if ( size() < n )
		{
			String* tmp_elements = new String[n];
			if ( dataelements != 0 )
			{
				for ( int k = 0 ; k < elements && k < n ; k++ )
					tmp_elements[k] = dataelements[k];
				delete [] dataelements;
			}
			dataelements = tmp_elements;
			elements = n;
		}
	}

private:
	int elements;
	String* dataelements;

};

typedef DynamicArrayGeneral<std::string>	DynamicArray;


struct Set
{
	DynamicArray x;
	bool HasItem(const std::string& h)
	{
		for ( int n = 0; n != x.size() ; n++ )
			if ( x[n] == h ) return true;
		return false;
	}
	void Insert(const std::string & h)
	{
		x.push_back(h);
	}
};



#define	FileClass_READ		0
#define	FileClass_WRITE		1

/*
struct FileClass
{

	HFILE		handle;

	bool is_open() { return (    (handle) != -1      ); } 
	void close(){ if( is_open() ) {File_Close(handle);handle=-1;} }
	~FileClass(){ close(); }
	bool Open(const std::string& x, int mode)
	{
		close();
		if ( mode != FileClass_WRITE )
			handle = File_Open(x.c_str(),OF_READ );
		else
			handle = File_Create(x.c_str());
		return (is_open());
	}
	FileClass(const std::string& x,int mode)
	{ handle=-1;Open(x,mode); }
	//-1 if unknown.
	unsigned int size(){ return File_GetLength(handle); }
	std::string GetAllData()
	{
		std::string out = "";
		File_SeekBegin(handle,0);
		char tmp[5000];
		while( !eof() )
		{
			File_Read(handle, tmp , 5000 );
			out += tmp;
		}
		return out;
	}
	char* GetAllDataC()
	{
		int s = size();
		char* data = new char[s+2];
		File_Read( handle , data , s+1 );
		data[s] = 0;
		return data;
	}
	void write( unsigned char* buffer , unsigned int size )
	{
		File_Write (handle, (void*)buffer, size);
	}
	bool eof(){ return (File_GetPosition(handle) >= size()); }
	std::string getline()
	{
		bool done = false;
		char tmp;
		std::string thusFar = "";
		while ( !eof() && !done )
		{
			File_Read(handle,&tmp,1);
		//	tmp[99] = 0;
			thusFar += tmp;
			if ( thusFar.find(STANDARD_NEW_LINE) != -1 )
			{
				thusFar = thusFar.substr(0,thusFar.find(STANDARD_NEW_LINE));
				done = true;
			}
		}
		return thusFar;

		/*
		std::string out = "";
		if ( eof() ) return out;
		char next;
		do
		{
			next = fgetc(handle);
			out += next;
		}
		while (next != '\n' && !eof());
		return out;
		*/
//	}


//};
//*/

struct FileClass
{
	FILE*	handle;
	unsigned int isize;

	bool is_open() { return handle != NULL; }
	void close(){ if( handle != NULL) {fclose(handle);handle=NULL;isize = 0;} }
	~FileClass(){ close(); }
	bool Open(const std::string& x,int mode)
	{
		close();
		std::string smode = "";
		if ( mode == 0 ) smode += "r";
		else smode += "w";
		smode += "b";
		handle = fopen(x.c_str(),smode.c_str());
		return (handle != NULL);
	}
	FileClass(const std::string& x,const int mode)
	{ isize=0;handle=NULL;Open(x,mode); }
	//-1 if unknown.
	unsigned int size(){ return isize; }
	std::string GetAllData()
	{
		std::string out = "";
		isize = 0;
		rewind(handle);
		char tmp[1001];
		while( !feof(handle) )
		{
			int thissize = fread( tmp , sizeof(char) , 1000 , handle );
			tmp[thissize] = 0;				
			isize += thissize;
			out += tmp;
		}
		return out;
	}
	char* GetAllDataC()
	{
		long curGuess = 1024;
		long pos = 0;
		char* buffer = (char*)malloc(curGuess);
		
		rewind(handle);

		// While it is open, try this...
		while ( !feof(handle) ) {

			// Reallocate the next bit of memory...
			if ( pos != 0 ) {
				curGuess *= 2;
				buffer = (char*)realloc(buffer,curGuess);
			}

			int can = curGuess - pos;
			int read = fread( buffer + pos , sizeof(char) , can , handle );
			pos += read;

		}

		// WE do this to make sure the result is allocated by 'new' - but above
		// we wanted to use realloc.
		buffer[pos] = 0;
		char* out = new char[pos+1];
		memcpy(out,buffer,pos+1);
		out[pos] = 0; //should already have happened
		free(buffer);
		return out;

		//buffer = (char*)realloc(buffer,pos+1);
		
	//	return buffer;
	}
	void write( const unsigned char* buffer , unsigned int size )
	{
		fwrite( (char*)buffer , sizeof(char) , size , handle );
	}
	bool eof(){ return feof(handle); }
	std::string getline()
	{
		std::string out = "";
		if ( eof() ) return out;
		char next;
		do
		{
			next = fgetc(handle);
			out += next;
		}
		while (next != '\n' && !eof());
		return out;
	}


};