#include "stdafx.h"

#include "DungeonClient.h"

using namespace Networking;

DungeonClient::DungeonClient( CLIENT_DESC aDesc )
{
    LOG_TRACE( "Client Ctor!" );

    strcpy_s( ServerAddr, 32, aDesc.ServerAddr );
    ServerPort = aDesc.ServerPort;

    std::cout << "Start client!" << std::endl;
    std::cout << "\tServer Address:" << ServerAddr << std::endl;
    std::cout << "\tConnect to Server Port:" << ServerPort << std::endl;
    std::cout << "\tRun client on port:" << CurrentPort << std::endl;

    InitCommandMap();

    InitSocket();
}

DungeonClient::~DungeonClient()
{
    LOG_TRACE( "Client Dtor!" );
    Shutdown();
}

void DungeonClient::InitCommandMap()
{
    // Quit ----------------------------------------
    Command QuitCmd = {};
    QuitCmd.CmdType = ECommandType::QUIT;
    InputCmdMap.emplace( "QUIT", QuitCmd );

    // Movement ----------------------------------------
    Command MoveRightCmd = {};
    MoveRightCmd.CmdType = ECommandType::MOVE;
    MoveRightCmd.PacketData.Direction.Right = 1;
    InputCmdMap.emplace( "RIGHT", MoveRightCmd );

    Command MoveLeftCmd = {};
    MoveLeftCmd.CmdType = ECommandType::MOVE;
    MoveLeftCmd.PacketData.Direction.Left = 1;
    InputCmdMap.emplace( "LEFT", MoveLeftCmd );

    Command MoveUpCmd = {};
    MoveUpCmd.CmdType = ECommandType::MOVE;
    MoveUpCmd.PacketData.Direction.Up = 1;
    InputCmdMap.emplace( "UP", MoveUpCmd );

    Command MoveDownCmd = {};
    MoveDownCmd.CmdType = ECommandType::MOVE;
    MoveDownCmd.PacketData.Direction.Down = 1;
    InputCmdMap.emplace( "DOWN", MoveDownCmd );

    // Pickup ----------------------------------------
    Command PickupCmd = {};
    PickupCmd.CmdType = ECommandType::PICKUP;
    InputCmdMap.emplace( "PICKUP", PickupCmd );
}

void DungeonClient::InitSocket()
{
    WSADATA wsaData;
    ClientSocket = INVALID_SOCKET;

    if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
    {
        fprintf( stderr, "FAILED. Error code : %d ", WSAGetLastError() );
        exit( EXIT_FAILURE );
    }

    // Create listen socket -----------------------
    if ( ( serverSockID = ClientSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) == SOCKET_ERROR )
    {
        fprintf( stderr, "Could not create socket: %d ", WSAGetLastError() );
        exit( EXIT_FAILURE );
    }
    LOG_TRACE( "Socket created.\n" );

    IsDone = false;

    // Init the socket thread!
    ClientSocketThread = std::thread(
        &Networking::DungeonClient::ClientWorker,
        this
    );
}

void DungeonClient::Run()
{
    // Process the input of the user
    ProcessInput();
}

void DungeonClient::Shutdown()
{
    IsDone = true;

    if ( ClientSocketThread.joinable() )
    {
        ClientSocketThread.join();
    }

    // Cleanup winsoc ------------------
    if ( ClientSocket != INVALID_SOCKET )
    {
        closesocket( ClientSocket );
    }
    WSACleanup();
}

void DungeonClient::ClientWorker()
{

#pragma region Init Socket info
    int slen;
    struct sockaddr_in  si_other;
    slen = sizeof( si_other );
    char buf [ DEF_BUF_SIZE ];

    memset( ( char* ) &si_other, 0, sizeof( si_other ) );
    si_other.sin_family = AF_INET;
    si_other.sin_addr.s_addr = INADDR_ANY;
    si_other.sin_port = htons( ServerPort );

    wchar_t wtext [ 32 ];
    size_t outSize;
    mbstowcs_s( &outSize, wtext, ServerAddr, strlen( ServerAddr ) + 1 );
    InetPton( AF_INET, wtext, &si_other.sin_addr.S_un.S_addr );
#pragma endregion

    // Loop checking for if the client wants to send commands
    while ( 1 )
    {
        // Check if the client should stop
        if ( IsDone ) return;

        // Check if there is a command to send in the queue
        Command currentCommand = {};
        bool found = CommandQueue.try_dequeue( currentCommand );

        if ( found )
        {
            // Send this command to the server
            char charCmd [ sizeof( Command ) ];
            memcpy( charCmd, ( void* ) ( &currentCommand ), sizeof( Command ) );

            if ( sendto( serverSockID, charCmd, sizeof( Command ), 0,
                ( struct sockaddr* ) &si_other, slen ) == SOCKET_ERROR )
            {
                printf( "sendto() failed : %d ", WSAGetLastError() );
                Shutdown();
                exit( EXIT_FAILURE );
            }

            //clear buffer
            //memset( buf, '\0', DEF_BUF_SIZE );

            // Receive the game state back from the server
            //block until recieved a packet
            if ( recvfrom( serverSockID, buf, sizeof( Status ), 0, ( struct sockaddr* )&si_other, &slen ) == SOCKET_ERROR )
            {
                printf( "recvfrom() failed : %d ", WSAGetLastError() );
                Shutdown();
                exit( EXIT_FAILURE );
            }

            Status res = {};
            memcpy( &res, ( void* ) buf, sizeof( Status ) );
            switch ( res.ResType )
            {
                case EResponseType::MAP:
                {
                    PrintMutex.lock();
                    PrintMap( res.PacketData.MapData.map, MAP_BUF_SIZE );
                    printf( "\n\tYou're treasure count is: %ud\n", res.Inventory.TreasureAmount );
                    PrintMutex.unlock();
                }
                break;
                case EResponseType::INVENTORY:
                {

                }
                break;
            default:
                break;
            }
        }
        else
        {
            // Sleep this thread so it doesn't eat the CPU
            std::this_thread::sleep_for( std::chrono::milliseconds( 3 ) );
        }
    }
}

void DungeonClient::ProcessInput()
{
    PrintMutex.lock();
    std::cout << "Welcome to the dungeon!" << std::endl;
    std::cout << "\t* (For a list of commands, enter \"HELP\"):" << std::endl;

    // Enter a character name
    std::cout << "Enter one character for your player ID: ";
    ClientPlayerID = std::getchar();
    std::cout << "You selected player ID of " << ClientPlayerID << std::endl;
    PrintMutex.unlock();

    // Update all the commands to have the proper client ID
    for ( auto & cmd : InputCmdMap )
    {
        cmd.second.ID = ClientPlayerID;
    }

    // Send the enter command
    {
        Command EnterCmd = {};
        EnterCmd.CmdType = ECommandType::ENTER;
        EnterCmd.ID = ClientPlayerID;
        CommandQueue.enqueue( EnterCmd );
    }

    while ( 1 )
    {
        PrintMutex.lock();
        // Grab console input
        std::cout << "Enter your command: ";
        std::string input = "";
        std::getline( std::cin, input );
        PrintMutex.unlock();

        // Convert string to upper case
        for ( auto & c : input ) c = toupper( c );

        // Special case of quiting
        if ( input == "QUIT" || input == "Q" )
        {
            // Send a quit command
            Command QuitCmd = {};
            QuitCmd.CmdType = ECommandType::QUIT;
            QuitCmd.ID = ClientPlayerID;
            CommandQueue.enqueue( QuitCmd );

            // Sleep so that the quit command can be sent hopefully
            std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );

            // Flag that we are done
            IsDone = true;
            return;
        }
        else if ( input == "HELP" || input == "H" )
        {
            PrintCommandList();
        }
        else
        {
            // Check if it is a command that we recognize
            for ( const auto & itr : InputCmdMap )
            {
                if ( itr.first == input )
                {
                    CommandQueue.enqueue( itr.second );
                }
            }
        }
    }
}

void DungeonClient::PrintCommandList()
{
    PrintMutex.lock();
    std::cout << "Command list:" << std::endl;

    for ( const auto & itr : InputCmdMap )
    {
        std::cout << "\t" << itr.first << std::endl;
    }
    PrintMutex.unlock();
}

void DungeonClient::PrintMap( char * aBuf, size_t aLen )
{
    printf( "MAP BUF: %s \n\n", aBuf );

    for ( size_t i = 0; i < aLen; ++i )
    {
        printf( "%c ", aBuf [ i ] );
        if ( i == 2 || i == 4 ) printf( "\n" );
        if ( i == 3 ) printf( "%c ", ClientPlayerID );
    }
    printf( "\n" );
}
