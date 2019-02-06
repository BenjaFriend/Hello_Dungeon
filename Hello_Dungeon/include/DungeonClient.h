#pragma once

#include "Commands.h"
#include "SocketUse.h"

#include "concurrentqueue.h"

#include <iostream>

namespace Networking
{
    struct CLIENT_DESC
    {
        char* ServerAddr;
        USHORT ServerPort   = DEF_SERVER_PORT;
        USHORT RunningPort  = DEF_CLIENT_PORT;
    };

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
        /// <param name="aDesc">Description of this client</param>
        DungeonClient( CLIENT_DESC aDesc );

        /// <summary>
        /// Shut down the client 
        /// </summary>
        ~DungeonClient();

        /// <summary>
        /// Start running the client
        /// </summary>
        void Run();

        /// <summary>
        /// 
        /// </summary>
        void Shutdown();

    private:

        /// <summary>
        /// Do socket initialization with winSoc2 
        /// </summary>
        void InitSocket();

        /// <summary>
        /// Worker thread for the client to listen to data from 
        /// the server on. Handle the processing of recieved commands
        /// </summary>
        void ClientWorker();
        
        /** The socket for the client to use */
        SOCKET ClientSocket = INVALID_SOCKET;

        /** ID for the connection to the server */
        UINT64 serverSockID;

        /** Check for if the client is done or not */
        std::atomic<bool> IsDone;

        /** The thread for running the client socket options */
        std::thread ClientSocketThread;

        /** The port to connect to on the server */
        USHORT CurrentPort = 50001;

        /** The server address for this socket to connect to */
        char ServerAddr [ 32 ];

        USHORT ServerPort = 50000;
        
        /** Lock-less command queue */
        //moodycamel::ConcurrentQueue<Command> CommandQueue;

    };

}   // namespace Networking