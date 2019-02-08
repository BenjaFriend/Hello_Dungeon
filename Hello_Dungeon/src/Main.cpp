/// <summary>
/// Entry pointer of the program, parses the command 
/// line input and creates a client or server accordingly
/// </summary>

#include "stdafx.h"
#include <iostream>
#include <memory>       // unique_ptr

#include "Commands.h"
#include "DungeonClient.h"
#include "DungeonServer.h"

#define SERVER_CMD      "-DUNGEON_SRV"
#define CLIENT_CMD      "-DUNGEON_CLI"
#define HELP_FLAG       "-h"
#define DUNG_SIZE_FLAG  "-s"
#define TRS_CNT_FLAG    "-t"
#define MAX_CNT_FLAG    "-m"
#define PORT_FLAG       "-p"


#include "Helpers.h"


int main( int argc, char* argv [] )
{
#if defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif

    if ( argc <= 1 )
    {
        Helpers::PrintHelp();
        //return -1;
    }

    // Parse the command line args
    bool isClient = false;
    bool isServer = false;

    Networking::SERVER_DESC serverInfo = {};
    Networking::CLIENT_DESC clientInfo = {};

    for ( size_t i = 0; i < argc; ++i )
    {
        if ( strcmp( argv [ i ], HELP_FLAG ) == 0 )
        {
            Helpers::PrintHelp();
            exit( EXIT_FAILURE );
        }
        else if ( strcmp( argv [ i ], SERVER_CMD ) == 0 )
        {
            isServer = true;
            isClient = false;
        }
        else if ( strcmp( argv [ i ], CLIENT_CMD ) == 0 )
        {
            isServer = false;
            isClient = true;
        }
        else if ( strcmp( argv [ i ], PORT_FLAG ) == 0 )
        {
            sscanf_s( argv [ i + 1 ], "%d", &serverInfo.Port );
            sscanf_s( argv [ i + 1 ], "%d", &clientInfo.ServerPort);
        }
        // #TODO: Check other port flags 
    }

    if ( isServer )
    {
        std::cout << "--- Run Server ---" << std::endl;
        // Use a unique_ptr so that there will be automatic cleanup 
        // if something crashes
        std::unique_ptr<Networking::DungeonServer> Server =
            std::make_unique<Networking::DungeonServer>( serverInfo );

        Server->Run();

        Server->Shutdown();
    }
    else if ( isClient )
    {
        std::cout << "--- Run Client ---" << std::endl;
        std::unique_ptr<Networking::DungeonClient> Client =
            std::make_unique<Networking::DungeonClient>( clientInfo );

        Client->Run();

        Client->Shutdown();
    }

    std::cout << "Program exit!" << std::endl;

    return 0;
}