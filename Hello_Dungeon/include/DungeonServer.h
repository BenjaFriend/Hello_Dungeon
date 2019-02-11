#pragma once

#include "Commands.h"
#include "DungeonMap.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <vector>

#include "SocketUse.h"

namespace Networking
{
    /// <summary>
    /// The instantiation information for creating a server
    /// </summary>
    struct SERVER_DESC
    {
        /** The port to run this server on */
        USHORT Port = DEF_SERVER_PORT;

        /** The value of each treasure */
        UINT32 TreasureValue = 1;

        /** The max number of treasure in this dungeon */
        UINT32 MaxTreasureCount = 5;

        /** The size of the dungeon to create */
        UINT8 DungeonSize = 5;
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

        /// <summary>
        /// Add a player to the list of current players
        /// if they do not already exist
        /// </summary>
        /// <param name="aID">Player ID to add</param>
        inline void AddNewPlayer( UINT8 aID );

        /** The socket for the server to use */
        SOCKET ServerSocket = INVALID_SOCKET;

        /** The running server thread */
        std::thread RunningThread;

        /** Atomic flag to check if we are done */
        std::atomic<bool> isDone;

        /** The port to run this server on */
        USHORT Port = 50000;

        /** The dungeon map to use for the player */
        std::unique_ptr<DungeonMap> Map;

        /** A vector of currently connected players */
        std::vector<UINT8> CurrentPlayers;

        /** A map of inventories to each player */
        std::unordered_map < UINT8, Status::PlayerInventory_s > PlayerInvMap;
    };

}   // namespace Networking