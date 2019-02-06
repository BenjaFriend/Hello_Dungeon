#include "stdafx.h"

#include "DungeonServer.h"

using namespace Networking;

DungeonServer::DungeonServer()
{
    std::cout << "\tServer ctor!" << std::endl;
}

DungeonServer::DungeonServer( UINT64 aPort, UINT32 aTreasureCount, UINT32 aMaxTreasureCount )
{
}

DungeonServer::~DungeonServer()
{
    std::cout << "\tServer dtor!" << std::endl;
}

UINT64 DungeonServer::Run()
{
    return 1;
}
