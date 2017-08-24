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

 char *sendbuf = "GET / HTTP/1.1\r\nHost: ya.ru\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:55.0) Gecko/20100101Firefox/55.0\r\nAccept:text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n Accept-Encoding: gzip, deflate\r\nDNT: 1\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nCache-Control: max-age=0\r\n\r\n";
char recvbuf[DEFAULT_RECEIVE_BUFFER];
int recvlen = DEFAULT_RECEIVE_BUFFER;
int i;
unsigned long traffic = 0;
unsigned long rpacket = 0;



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
  /*  iResult = shutdown( ConnectSocket, SD_SEND );
    if( iResult == SOCKET_ERROR )
	{
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return ERR_CONNECT+4;
    }  */
	
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


// ----------------- end function GetZaprosToBot --------------------------------- //



// --------------------------------------------------------------------------------------------- //
// -------- END of FILE NetWork.c -------------------------------------------------------------- //
