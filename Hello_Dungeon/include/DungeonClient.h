#pragma once

#include "Commands.h"
#include "SocketUse.h"

#include <iostream>

namespace Networking
{
    /// <summary>
    /// The client for the dungeon
    /// </summary>
    class DungeonClient
    {
    public:

        /// <summary>
        /// Connects the client to the given server and spawns a thread
        /// for processing commands
        /// </summary>
        /// <param name="aServerAddr">The server address to connect o</param>
        /// <param name="aPort">Port on the server to connect to </param>
        DungeonClient( const char* aServerAddr, const int aPort );

        ~DungeonClient();

        /// <summary>
        /// Start running the client
        /// </summary>
        void Run();

    private:

        /// <summary>
        /// Worker thread for the client to lsiten to data from 
        /// the server on. Handle the processing of recieved commands
        /// </summary>
        void ClientSocketThread();
        
        /** The socket for the client to use */
        SOCKET ClientSocket = INVALID_SOCKET;

        /** The port to connect to on the server */
        int CurrentPort = 50000;

        /** The server address for this socket to connect to */
        char ServerAddr [ 32 ];

        /** Thread where client is listening for data from the server */
        std::thread ClientThread;



    };

}   // namespace Networking