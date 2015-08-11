#include "zlib/zlib.h"

bool CompressHuffman( unsigned char* input , int inlen , unsigned char* & output , int& outlen ) {
	
	unsigned long bound = compressBound( inlen ) ;
	output = new unsigned char[bound+4];

	*((unsigned long*)output) = inlen;
	int ret = compress( output+4 , &bound , input , inlen );
	outlen = bound + 4;

	return ( ret == Z_OK );
}

// The output buffer is zero ternminated, but outlen does not include that byte.
bool DecompressHuffman( unsigned char* input , int inlen , unsigned char* & output , int& outlen ) {

	unsigned long readlen = *((unsigned long*)input);
	input += 4;
	inlen -= 4;

	output = new unsigned char[readlen + 1];

	unsigned long actuallen = readlen;
	if ( output == 0 ) return false;
	output[readlen] = 0;


	int ret = uncompress( output, &actuallen, input, inlen );
	outlen = actuallen;
	// actuallen == readlen, we think!
	return ret ==Z_OK;
}
