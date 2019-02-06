#pragma once

#include "Commands.h"

#include <iostream>
#include <unordered_map>

#include "SocketUse.h"

namespace Networking
{
    /// <summary>
    /// The authoritative server for the dungeon in charge of handling 
    /// what clients are connected, and processing commands
    /// </summary>
    class DungeonServer
    {
    public:
        DungeonServer();

        DungeonServer( UINT64 aPort, UINT32 aTreasureCount, UINT32 aMaxTreasureCount );

        ~DungeonServer();

        /// <summary>
        /// Run this server
        /// </summary>
        /// <returns>Result of running the server</returns>
        UINT64 Run();

    private:

        /** The port to run this server on */
        UINT64 Port;

        UINT32 TreasureCount;

        UINT32 MaxTreasureCount;

    };


}   // namespace Networking