#include "stdafx.h"

#include "DungeonServer.h"

using namespace Networking;

DungeonServer::DungeonServer()
{
    std::cout << "\tServer ctor!" << std::endl;
    // Generate the dungeon for the clients to use
    // Spawn treasures based on the given input

    // Init socket connection and running thread
}

DungeonServer::DungeonServer( UINT64 aPort, UINT32 aTreasureCount, UINT32 aMaxTreasureCount ) 
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
