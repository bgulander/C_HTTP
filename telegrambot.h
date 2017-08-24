/* ----------------------------------------------------------------------------------------------- //
#//Project:		Telegram Bot test programm
#//File:		telegrambot.h
#//Date:		21.08.2017
#//Author:		T.O.
#//Version:		1.0

#//
#//
#-----------------------------------------------------------------------*/


#define ERR_SUCCESS 	0
#define ERR_NULLSTR		20
#define ERR_CONNECT		30



#define IP_ADDRESS_LEN		16
#define MAC_ADDRESS_LEN		18
#define ETHER_ADDR_LEN		6

#define MAXERRORS				20
#define LOG_MAX_MESSAGE_LEN		1024

#define LOG_FILE_NAME		"Telegram_Bot_System.log"

//#define DEFAULT_SERVER_ADDRESS	"https://api.telegram.org"
//#define DEFAULT_SERVER_ADDRESS	"api.telegram.org"
#define DEFAULT_SERVER_ADDRESS	"149.154.167.99"
#define DEFAULT_HTTPS_PORT		"443"
#define DEFAULT_HTTP_PORT		"80"
#define DEFAULT_RECEIVE_BUFFER  1500



// ------------------------- log function prototypes ------------------

//void logInicializ();
void logEnds();

void logStart_Programm();
void logFinish_Programm();


// -------------------------------- net function prototypes --------------------------------------

int GetZaprosToBot( void );
int GetSSLZaprosToBot( void );










// ------------------------ END of FILE telegrambot.h -----------------------------