/* ----------------------------------------------------------------------------------------------- //
#//Project:		Telegram Bot test programm
#//File:		telegrambot.c
#//Date:		21.08.2017
#//Author:		T.O.
#//Version:		1.0

#//
#//
#-----------------------------------------------------------------------*/

#include "telegrambot.h"

int Result = 0, res = 0;

void main( int argc, char *argv[] )
{

	printf( "\n\nStartProgramm success\n" );

	// start logging sub system
	logInicializ();
	
	logStart_Programm();
	
	// loading config parameter
	
	// main cycle periodic question
	
		// read incoming data
		//Result = GetZaprosToBot();
		Result = GetSSLZaprosToBot();
		
		// generation answer
/*		if( result == ERR_SUCCESS )
		{
			res = GenerationData();
		}
		
		// send messages
		if( res == ERR_SUCCESS )
		{
			res = SendMessageToBot( );
		} */
	
	// end logging subsysten
	logFinish_Programm();
	logEnds();
	printf( "End program success!!!\n\n" );
}

// ------------------------------------------------------------------------------------------------- //
// --- END of FILE telegrambot.c
