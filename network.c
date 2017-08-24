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
#include <winsock2.h>
#include <ws2tcpip.h>

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
//char *sendbuf = "https://api.telegram.org/bot407153638:AAGLP8QI3RN78dClmfzI-f7hslQ5PGzmTbQ/getUpdates";
//char *sendbuf = "/bot407153638:AAGLP8QI3RN78dClmfzI-f7hslQ5PGzmTbQ/getUpdates";
char *sendbuf = "GET /index.html HTTP/1.1\n
				 Host: 192.168.4.254\n
				 User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\n
				 Accept: text/html\n
				 Connection: close";
char recvbuf[DEFAULT_RECEIVE_BUFFER];
int recvlen = DEFAULT_RECEIVE_BUFFER;
int i;



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
	iResult = getaddrinfo( "192.168.4.254", DEFAULT_HTTP_PORT, &hints, &result );
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
	iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    if(iResult == SOCKET_ERROR)
	{
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return ERR_CONNECT+3;
    }	
	printf( "Bytes Sent: %ld\n\n", iResult );	
	
	printf( "%s\n\n", sendbuf );
	
	
	// shutdown the connection since no more data will be sent
    iResult = shutdown( ConnectSocket, SD_SEND );
    if( iResult == SOCKET_ERROR )
	{
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return ERR_CONNECT+4;
    }
	
	// ---------------------- Receive until the peer closes the connection -----------------
    do{
        iResult = recv( ConnectSocket, recvbuf, recvlen, 0 );
        if( iResult > 0 )
		{
            printf("Bytes received: %d\n", iResult );
			printf( "%s\n", recvbuf );
		}
        else
		{
			if ( iResult == 0 )
			{
				printf("Result = 0; Connection closed\n");
			}
			else
			{
				printf("recv failed with error: %d\n", WSAGetLastError());
			}
		}
    }while( iResult > 0 );	
	
	// close socket
	WSACleanup();
	
}


// ----------------- end function GetZaprosToBot --------------------------------- //



// --------------------------------------------------------------------------------------------- //
// -------- END of FILE NetWork.c -------------------------------------------------------------- //