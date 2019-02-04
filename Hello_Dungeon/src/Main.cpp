/// <summary>
/// Entry pointer of the program, parses the command 
/// line input and creates a client or server accordingly
/// </summary>

#include <iostream>
#include <stdlib.h>

#include "Commands.h"
#include "DungeonClient.h"
#include "DungeonServer.h"

#define SERVER_CMD      "DUNGEON_SRV"
#define CLIENT_CMD      "DUNGEON_CLI"

void PrintHelp()
{
    fprintf( stderr, "\nUSAGE: ./Hello_Dungeon.exe [-%s] <SERVER OPTIONS> ... [-%s] <CLIENT OPTIONS>\n", SERVER_CMD, CLIENT_CMD );

    fprintf( stderr, "\t-h\tPrint this helpful message.\n" );
    fprintf( stderr, "\nSERVER Options: \n" );
    fprintf( stderr, "\t-s\tSize of dungeon\n" );
    fprintf( stderr, "\t-t\tthe treasure count\n" );
    fprintf( stderr, "\t-m\tMax Treasure Count\n" );
    fprintf( stderr, "\t-p\tPort to connect to\n" );
}

int main( int argc, char* argv [] )
{
#if defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif

    if ( argc <= 1 )
    {
        PrintHelp();
        return -1;
    }

    // Parse the command line to determine if we should make a client or server

    std::cout << "Hello Dungeon!" << std::endl;

    return 0;
}