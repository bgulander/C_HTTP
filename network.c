/* ----------------------------------------------------------------------------------------------- //
#//Project:		Telegram Bot test programm
#//File:		network.c
#//Date:		22.08.2017
#//Author:		T.O.
#//Version:		1.0

#//
#//
#-----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/crypto.h>


#include "telegrambot.h"


struct addrinfo *result = NULL;
struct addrinfo *ptrres = NULL;
struct addrinfo hints;

// --------------------------------------------------------------------------------------------------------- 

int GetZaprosToBot( void )
{

int iResult = 0;
WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
char recvbuf[DEFAULT_RECEIVE_BUFFER];
int recvlen = DEFAULT_RECEIVE_BUFFER;
int i;
unsigned long traffic = 0;
unsigned long rpacket = 0;


const SSL_METHOD *meth;
SSL_CTX *ctx;
SSL *ssl;



	// -------------------------------------------------------------------------------
	/* ---------------------------------------------------------- *
	* These function calls initialize openssl for correct work.  * 	* initialize SSL library and register algorithms             *
	* ---------------------------------------------------------- */
	SSLeay_add_ssl_algorithms();
	meth = (SSL_METHOD *)SSLv3_client_method();
	SSL_load_error_strings();
	ctx = SSL_CTX_new (meth);                        

	if( ctx == NULL )
	{
		printf( "error in SSL_CTX_new function. Return NULL. \n" );
		return 10;
	}

    // Initialize Winsock
    iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
    if ( iResult != 0 )
	{
        printf("WSAStartup failed with error: %d\n", iResult);
        return ERR_CONNECT;
    }
	printf( "Initialize Winsock success!\n" );

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;


    // Resolve the server address and port
	//iResult = getaddrinfo( DEFAULT_SERVER_ADDRESS, DEFAULT_HTTPS_PORT, &hints, &result );
	iResult = getaddrinfo( "ya.ru", DEFAULT_HTTPS_PORT, &hints, &result );
    if( iResult != 0 )
	{
        printf( "getaddrinfo failed with error: %d\n", iResult );
        WSACleanup();
        return ERR_CONNECT+1;
    }
	printf( "GetAddrInfo execute success!\n" );
	
	// attemt to connect
	for( ptrres = result; ptrres != NULL; ptrres = ptrres->ai_next )
	{
		
		ConnectSocket = socket( ptrres->ai_family, ptrres->ai_socktype, ptrres->ai_protocol );
        if( ConnectSocket == INVALID_SOCKET )
		{
            printf( "socket failed with error: %ld\n", WSAGetLastError() );
            WSACleanup();
            return ERR_CONNECT+2;
        }		
		
		iResult = connect( ConnectSocket, ptrres->ai_addr, (int)ptrres->ai_addrlen );
		if( iResult == SOCKET_ERROR )
		{
			printf("Unable to connect to server!\n");
			continue;    
		}
		break;
	}
	
	printf( "IP address: %hhu.%hhu.%hhu.%hhu \n", ptrres->ai_addr->sa_data[2], ptrres->ai_addr->sa_data[3], ptrres->ai_addr->sa_data[4], ptrres->ai_addr->sa_data[5] );
	
	
	ssl = SSL_new (ctx);                         
	if( ssl == NULL )
	{
		printf( "SSL_new error! \n" );
		return 11;
	}
	else
	{
		printf( "SSL_new success!\n" );
	}
	//SSL_set_fd(ssl, sd);
	//err = SSL_connect (ssl);                     CHK_SSL(err);

	// -------------------------- send data ---------------------------------------------------------------
	/*iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    if(iResult == SOCKET_ERROR)
	{
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return ERR_CONNECT+3;
    }	
	printf( "Bytes Sent: %ld\n\n", iResult );	
	
	printf( "%s\n\n", sendbuf );
	
	
	// ---------------------- Receive until the peer closes the connection -----------------
    do{
        iResult = recv( ConnectSocket, recvbuf, recvlen, 0 );
        if( iResult > 0 )
		{
            traffic += iResult;
			rpacket++;
			printf("Bytes received: %d\n", iResult );
			printf( "%s\n", recvbuf );
		}
        else
		{
			if ( iResult == 0 )
			{
				rpacket++;
				printf("Result = 0; Connection closed\n");
			}
			else
			{
				printf("recv failed with error: %d\n", WSAGetLastError());
			}
		}
    }while( iResult > 0 );	
	*/
	// shutdown the connection since no more data will be sent
    iResult = shutdown( ConnectSocket, SD_SEND );
    if( iResult == SOCKET_ERROR )
	{
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return ERR_CONNECT+4;
    }
	printf( "Closed Connection. Total recived bytes: %u  Packet received %u.\n", traffic, rpacket ); 

	// close socket
	WSACleanup();
	
}


// ----------------- end function GetZaprosToBot --------------------------------- //



// --------------------------------------------------------------------------------------------- //
// -------- END of FILE NetWork.c -------------------------------------------------------------- //