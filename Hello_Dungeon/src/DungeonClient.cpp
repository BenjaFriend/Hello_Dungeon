#include "stdafx.h"

#include "DungeonClient.h"

using namespace Networking;

DungeonClient::DungeonClient( const char* aServerAddr, const int aPort )
{
    std::cout << "\tClient ctor!" << std::endl;
    
}

DungeonClient::~DungeonClient()
{
    std::cout << "\tClient dtor!" << std::endl;
}