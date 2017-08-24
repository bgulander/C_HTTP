/* --------------------------------- 
Project:	KernelModeEthernetBridge
File:		stringwork.c
Data:		20.08.2010
Author:		T.O.
version:	1.0.0

Desc: file to work with string.
------------------------------------*/
#include <windows.h>
#include <stdio.h>
//#include "bridgemain.h"
#include "telegrambot.h"

unsigned char *errmessages[] = 
{
	"all OK",
	"miss parameters <filename>",
	"could not open input configure file",
	"could not read input configure file",
	"bad input parameters",
	"error in string in config.file",
	"configure file have not all required parameters",
	"bad XML configuration file",
	"XML file have no only one rules",
	"bad argument in the command line",
	"could not open kmethbridge driver",
	"could not connect to kmethbridge driver",
	"could not create directory to logs:",
	"too many parameters in request_parameter tag in access matrix",
	"",
};


unsigned char *errstring( unsigned int err )
{
	if( err < MAXERRORS )
	{
		return errmessages[err];
	}
	else
	{
		return "unknown error";
	}
}


unsigned int strlength( unsigned char *str )
{
	// with 0-terminator string
	// test ok
	unsigned int i;
	if( str == NULL )
	{
		return 0;
	}
	for( i = 0; str[i]!=0; i++ );

	return i;
}



void rmspacestr( unsigned char *str, unsigned int *strlen)
{
	// test ok
	unsigned int i, k, s;
	if( strlen == 0 || str == NULL )
	{
		return;
	}
	s = *strlen;
	for( i = 0; str[i] != 0 && i < s; i++ )
	{
		if( isspace( str[i] ) != 0 )
		{
			for( k = i+1; k < s; k++ )
			{
				str[k-1] = str[k];
			}
			s--;
			i--;
		}
	}
	*strlen = s;
	return;
}




unsigned int isdigitstring( unsigned char *str, unsigned int len )
{
	// return 0 - if digit ( 0..9 ) else return !0
	// test OK.

	if( len == 0 || str == NULL )
	{
		return 0xFFFF;
	}

	if( str[len-1] == 0 )
	{
		len -= 1;
	} 

	for( len-=1; len != 0; len-- )
	{
		if( isdigit( str[len] ) == 0 )
		{
			return 0xFFFF;
		}
	}
	if( isdigit( str[0] ) == 0 )
	{
		return 0xFFFF;
	}

	return ERR_SUCCESS;
}

// --------------------------------------------------------------------------------------------//

unsigned int mystrtoint( unsigned char *str, unsigned int len, unsigned long *res )
{
	// if convert OK - return 0, else !0
	// Test OK.
	
	unsigned int i, t=0;
	unsigned long a, b;

	// check input param
	if( len == 0 || str == NULL )
	{
		return 0xffff;
	}
	if( isdigitstring( str, len ) != 0 )
	{
		return 0xffff;
	}
	if( len > 10 )
	{
		return 0xffff;
	}

	if( str[len-1] == 0 )
	{
		len -= 1;
	}
	len -= 1;
	a = (str[len]-48);
	b = 10;
	
	for( ; len!=0; len--, b=b*10 )
	{
		a += ((str[len-1]-48)*b);
	}

	if( a > 0x7FFFFFFF )
	{
		return 0xffff;
	}

	*res = (unsigned long)a;
	return ERR_SUCCESS;
}

//-----------------------------------------------------------------------------//

unsigned int myinttostr( unsigned char *str, unsigned int len, unsigned int num )
{
	// if success - return 0, else - return error
	// test OK


	unsigned int i, a;
	unsigned char t[5];

	if( str == NULL || len < 5 || num > 0xFFFF )
	{
		return ERR_NULLSTR;
	}

	memset( str, 0, len );
	for( i = 0;  (num / 10) != 0; i++, num /= 10 )
	{
		t[i] = (num % 10) + 0x30;
	}
	t[i] = (num % 10) + 0x30;
	for( a = 0; (i-a)!= 0; a++ )
	{
		str[a] = t[i-a];
	}
	str[a] = t[i-a];
	return ERR_SUCCESS;
}
// ---------------------------------------------------------------------------------------------//

unsigned int mystrncmp( unsigned char *s1, unsigned int l1, unsigned char *s2, unsigned int l2 )
{
	// Test OK!
	// return 0 if s1 == s2
	// else return !0

	unsigned int i;
	// check input param
	if( l1 == 0 || l2 == 0 || s1 == NULL || s2 == NULL )
	{
		return ERR_NULLSTR;
	}

	if( l1 != l2 )
	{
		return 0xFFFF;
	}

	for( i = 0; i <= l1; i++ )
	{
		if( s1[i] != s2[i] )
		{
			return 0xffff;
		}
	}

	return ERR_SUCCESS;
}


//-----------------------------------------------------------------------------------//

unsigned int verifyip( unsigned char *str, unsigned int len, unsigned char *ip )
{
	// Test OK.
	
	// if right ip adr - return 0
	// else return !0
	
	unsigned char separated[] = ".";
	unsigned char *tok;
	unsigned char temp[IP_ADDRESS_LEN];
	unsigned int l, r, cn = 0;
	unsigned char mass[4];

	// check input param
	if( len < 7 || len > IP_ADDRESS_LEN || str == NULL )
	{
		return 0xffff;
	}
	memset( temp, 0, IP_ADDRESS_LEN );
	memset( mass, 0, 4 );
	memcpy( temp, str, len );
	tok = (unsigned char *)strtok( temp, separated );
	while( tok != NULL )
	{
		l = strlength( tok );
		if( isdigitstring( tok, l ) != 0 )
		{
			return 0xFFFF;
		}
		if( mystrtoint( tok, l, &r ) != 0 )
		{ 
			return 0xFFFF;
		}
		if( r > 255 )
		{
			return 0xFFFF;
		}
		mass[cn] = r;
		cn++;
		tok = (UCHAR *)strtok( NULL, separated );
	}

	if( cn != 4 )
	{
		return 0xFFFF;
	}
	if( ip != NULL )
	{
		memcpy( ip, mass, 4 );
		//printf( "verifyIP: mass - %d.%d.%d.%d.    ip: %d.%d.%d.%d.\n", mass[0], mass[1], mass[2], mass[3], ip[0], ip[1], ip[2], ip[3] );
	}

	return ERR_SUCCESS;
}

//-------------------------------------------------------------------------//

unsigned int verifymac( unsigned char *str, unsigned int len, unsigned char *mac )
{
	// if right mac adr - return 0
	// else return !0
	
	unsigned char separated[] = ":-\"\t";
	unsigned char *tok;
	unsigned char temp[MAC_ADDRESS_LEN];
	unsigned char mass[ETHER_ADDR_LEN];
	unsigned int l, cn = 0;

	// check input param
	if( (len+1) != MAC_ADDRESS_LEN || str == NULL )
	{
		return 0xffff;
	}
	memset( temp, 0, MAC_ADDRESS_LEN );
	memset( mass, 0, ETHER_ADDR_LEN );
	memcpy( temp, str, len );
	tok = (UCHAR *)strtok( temp, separated );
	while( tok != NULL )
	{
		l = strlength( tok );
		if( l != 2 )
		{
			return 0xffff;
		}
		if( (isxdigit( tok[0] ) == 0) || (isxdigit( tok[1] ) == 0) )
		{
			return 0xffff;
		}
		if( mystrtohex( tok, l, &mass[cn] ) != ERR_SUCCESS )
		{
			return 0xffff;
		}
		cn++;
		tok = (UCHAR *)strtok( NULL, separated );
	}
	if( cn != 6 )
	{
		return 0xffff;
	}
	if( mac != NULL )
	{
		memcpy( mac, mass, ETHER_ADDR_LEN );
	} 

	return ERR_SUCCESS;
}

// ----------------------------------------------------------------------------------------//

int mystrtohex( unsigned char *str, unsigned int len, unsigned char *res )
{
	// test OK
	// if success translate return 0
	// else return !0
	unsigned int a, b;
	
	if( str == NULL || len != 2 )
	{
		return 0xFFFF;
	}
	if( isxdigit( str[0] ) == 0 || isxdigit( str[1] ) == 0 )
	{
		return 0xFFFF;
	}
	
	if( 48 <= str[0] && str[0] <= 57 )
	{
		a = str[0] - 0x30;
	}
	if( 65 <= str[0] && str[0] <= 70 )
	{
		a = str[0] - 55;
	}
	if( 97 <= str[0] && str[0] <= 102 )
	{
		a = str[0] - 87;
	}

	if( 48 <= str[1] && str[1] <= 57 )
	{
		b = str[1] - 0x30;
	}
	if( 65 <= str[1] && str[1] <= 70 )
	{
		b = str[1] - 55;
	}
	if( 97 <= str[1] && str[1] <= 102 )
	{
		b = str[1] - 87;
	}
	*res = (a<<4)+b;
	return ERR_SUCCESS;
}

//------------------------------------------------------------------------------------

int  cp1251_to_uft16( unsigned char * inbuf, size_t inlen, wchar_t * outbuf, size_t outlen )
{
    unsigned int res_code = 0xffff;
    int res;

    if( (inbuf == NULL) || (inlen == 0) || (outbuf == NULL) || (outlen == 0) )
	{
		return res_code;
	}

    memset( outbuf, 0, outlen );
    res = MultiByteToWideChar(CP_ACP, 0, inbuf, inlen, outbuf, outlen );
	if( res == 0 )
	{
		res_code = 0xffff;
	}
    else 
	{
		res_code = ERR_SUCCESS;
	}
    return (res_code);
}

// --------------------//-------------------------------------//------------------------------//

/* Purpose: Make a safe copy of one string to another */
// author: Dmitriy Stefankov
//static 
unsigned char *  safe_strncpy ( char *dest, const char *src, size_t n )
{
    char          * s;                      /* Temporary  */

    for( s = dest; 0 < (n-1) && *src!= '\0'; --n )
	{
         *s++ = *src++;                     /* Copy at most n-1 chars */
	}

    for( ; 0 < n; --n )
	{
         *s++ = '\0';                       /* Padding */
	}

    return( dest );
}

//============================================================================================//


void  time2str( time_t this_time, unsigned char * tmpbuf, unsigned int bn )
{
  char * p;

  _snprintf( tmpbuf, bn, "%s", asctime(gmtime(&this_time)) );
  p = strchr( tmpbuf, '\n');
  if( p != NULL )
  {
	  *p = '\0';
  }

}

// ---------------------------------------------------- () -----------------------------------//



/// end of file.