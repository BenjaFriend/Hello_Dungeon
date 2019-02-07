#pragma once

#include "Commands.h"
#include "SocketUse.h"

#include "concurrentqueue.h"

#include <iostream>
#include <string>
#include <unordered_map>

namespace Networking
{
    /// <summary>
    /// Description of how this client should run
    /// </summary>
    struct CLIENT_DESC
    {
        /** Server address */
        char ServerAddr [ 32 ] = "127.0.0.1\0";

        /** The port that the server is running on */
        USHORT ServerPort = DEF_SERVER_PORT;

        /** The port that this client's socket should bind to */
        USHORT RunningPort = DEF_CLIENT_PORT;
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
        /// Shuts down the connection to the server
        /// Cleans up winsoc and any other socket info
        /// </summary>
        void Shutdown();

    private:

        /// <summary>
        /// Initilze the map of commands that we can send as 
        /// a client
        /// </summary>
        void InitCommandMap();

        /// <summary>
        /// Do socket initialization with winSoc2 
        /// </summary>
        void InitSocket();

        /// <summary>
        /// Worker thread for the client to listen to data from 
        /// the server on. Handle the processing of received commands
        /// </summary>
        void ClientWorker();

        /// <summary>
        /// Process the input that the user types on the keyboard
        /// </summary>
        void ProcessInput();
        
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

        /** The port that the server should run on */
        USHORT ServerPort = 50000;

        /** Lock-less command queue */
        moodycamel::ConcurrentQueue<Command> CommandQueue;

        /** A map of possible inputs to their corresponding commands */
        std::unordered_map<std::string, Command> InputCmdMap;

    };

}   // namespace Networking