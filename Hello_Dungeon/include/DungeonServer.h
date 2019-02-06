#pragma once

#include "Commands.h"

#include <iostream>
#include <unordered_map>

#include "SocketUse.h"

namespace Networking
{
    struct SERVER_DESC
    {
        UINT64 aPort = DEF_SERVER_PORT;
        UINT32 aTreasureCount;
        UINT32 aMaxTreasureCount;
    };

    /// <summary>
    /// The authoritative server for the dungeon in charge of handling 
    /// what clients are connected, and processing commands
    /// </summary>
    /// <author>Ben Hoffman</author>
    class DungeonServer
    {
    public:

        /// <summary>
        /// Create the server with the given description
        /// Generate the dungeon and treasure
        /// Create/bind the server socket
        /// </summary>
        /// <param name="aDesc">The description of this server </param>
        DungeonServer( SERVER_DESC aDesc );

        /// <summary>
        /// Cleanup the socket connections and the dungeon
        /// </summary>
        ~DungeonServer();

        /// <summary>
        /// Run this server
        /// </summary>
        /// <returns>Result of running the server</returns>
        UINT64 Run();

        /// <summary>
        /// Shut down the server socket and let the clients 
        /// know that the game has ended
        /// </summary>
        void Shutdown();

    private:

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

        // #TODO: 2D array representing the dungeon

    };

}   // namespace Networking