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

        void Shutdown();

        /** The socket for the server to use */
        SOCKET ServerSocket = INVALID_SOCKET;

        /** The running server thread */
        std::thread RunningThread;
        
        /** Atomic flag to check if we are done */
        std::atomic<bool> isDone;

        /** The port to run this server on */
        UINT64 Port = 50000;

        /** The value of each treasure */
        UINT32 TreasureCount;

        /** The max number of treasure chests in the dungeon */
        UINT32 MaxTreasureCount;

        // 2D array representing the dungeon

    };


}   // namespace Networking