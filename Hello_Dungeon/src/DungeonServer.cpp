#include "stdafx.h"

#include "DungeonServer.h"

using namespace Networking;

DungeonServer::DungeonServer( SERVER_DESC aDesc )
{

}

DungeonServer::~DungeonServer()
{
    isDone = true;
    // Cleanup -------------------
    closesocket( ServerSocket );
    WSACleanup();
}

UINT64 DungeonServer::Run()
{
    
    return 1;
}

void Networking::DungeonServer::Shutdown()
{
    isDone = true;

    if ( RunningThread.joinable() )
    {
        RunningThread.join();
    }
}
