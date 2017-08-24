/* ----------------------------------------------------------------------------------------------- //
#//Project:		Telegram Bot test programm
#//File:		networkssl.c
#//Date:		22.08.2017
#//Author:		T.O.
#//Version:		1.0

#//
#//
#-----------------------------------------------------------------------*/

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "telegrambot.h"


struct addrinfo *result = NULL;
struct addrinfo *ptrres = NULL;
struct addrinfo hints;

// --------------------------------------------------------------------------------------------------------- 

int GetSSLZaprosToBot( void )
{

int iResult = 0;
WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
char recvbuf[DEFAULT_RECEIVE_BUFFER];
int recvlen = DEFAULT_RECEIVE_BUFFER;
int i;
unsigned long traffic = 0;
unsigned long rpacket = 0;

static unsigned char clientHello[] = 
{
	0x16, // handshake
	0x03, 0x03, // version (TLS 1.2)
	0x00, 0x2F, // length of handshake
	// handshake
	0x01, // handshake type (client hello)
	0x00, 0x00, 0x2B, // length of handshake payload 
	// client hello
	0x03, 0x03, // highest version supported (TLS 1.2)
	0x45, 0xFA, 0x01, 0x19, 0x74, 0x55, 0x18, 0x36, 
	0x42, 0x05, 0xC1, 0xDD, 0x4A, 0x21, 0x80, 0x80, 
	0xEC, 0x37, 0x11, 0x93, 0x16, 0xF4, 0x66, 0x00, 
	0x12, 0x67, 0xAB, 0xBA, 0xFF, 0x29, 0x13, 0x9E, // 32 random bytes
	0x00, // session id length
	0x00, 0x02, // chiper suites length
	0x00, 0x3D, // RSA_WITH_AES_256_CBC_SHA256
	0x01, // compression methods length
	0x00,  // no compression
	0x00, 0x00 // extensions length
};



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
	iResult = getaddrinfo( "149.154.167.99", DEFAULT_HTTPS_PORT, &hints, &result );
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
	
	
	// -------------------------- send data ---------------------------------------------------------------
	//iResult = send( ConnectSocket, clientHello, (int)strlen(clientHello), 0 );
	iResult = send( ConnectSocket, clientHello, 52, 0 );
    if(iResult == SOCKET_ERROR)
	{
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return ERR_CONNECT+3;
    }	
	printf( "Bytes Sent: %ld\n\n", iResult );	
	
	//printf( "%s\n\n", clientHello );
	

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



// --------------------------------------------------------------------------------------------- //
// -------- END of FILE NetWorkSSL.c -------------------------------------------------------------- //