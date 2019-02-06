#pragma once

#include "Commands.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>

#include "SocketUse.h"

namespace Networking
{
    struct SERVER_DESC
    {
        USHORT Port = DEF_SERVER_PORT;
        UINT32 TreasureCount;
        UINT32 MaxTreasureCount;
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
        /// Run this server and listens for commands
        /// </summary>
        /// <returns>Result of running the server</returns>
        UINT64 Run();

        /// <summary>
        /// Shut down the server socket and let the clients 
        /// know that the game has ended
        /// </summary>
        void Shutdown();

    private:

        /// <summary>
        /// Logic for listening to commands sent from clients
        /// </summary>
        void ListenThread();

        /// <summary>
        /// Function for handling the local server commands
        /// </summary>
        void ProcessLocalConsole();

        /** The socket for the server to use */
        SOCKET ServerSocket = INVALID_SOCKET;

        /** The running server thread */
        std::thread RunningThread;
        
        /** Atomic flag to check if we are done */
        std::atomic<bool> isDone;

        /** The port to run this server on */
        USHORT Port = 50000;

        /** The value of each treasure */
        UINT32 TreasureCount;

        /** The max number of treasure chests in the dungeon */
        UINT32 MaxTreasureCount;

        // #TODO: 2D array representing the dungeon

    };

}   // namespace Networking