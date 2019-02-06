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

#define SERVER_CMD      "DUNGEON_SRV"
#define CLIENT_CMD      "DUNGEON_CLI"

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

    UINT64 PORT = DEF_CLIENT_PORT;
    char SERVER_ADDR [ 32 ] = "127.0.0.1\0";

    Networking::SERVER_DESC server = {};

    server.Port = DEF_SERVER_PORT;
    server.MaxTreasureCount = 10;
    server.TreasureCount = 5;

    {
        // Use a unique_ptr so that there will be automatic cleanup 
        // if something crashes
        std::unique_ptr<Networking::DungeonServer> Server =
            std::make_unique<Networking::DungeonServer>( server );

        Server->Run();

        Server->Shutdown();
    }
    
    return 0;
}