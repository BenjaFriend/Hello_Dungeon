#include "stdafx.h"

#include "DungeonClient.h"

using namespace Networking;

DungeonClient::DungeonClient( CLIENT_DESC aDesc )
{
    LOG_TRACE( "Client Ctor!" );

    strcpy_s( ServerAddr, 32, aDesc.ServerAddr );
    ServerPort = aDesc.ServerPort;

    std::cout << "Start client!" << std::endl;
    std::cout << "\tServer Address: \t" << ServerAddr << std::endl;
    std::cout << "\tConnect to Server Port: \t" << ServerPort << std::endl;
    std::cout << "\tRun client on port: \t" << CurrentPort << std::endl;

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
    Command Cmd = {};
    Cmd.cmd = 'E';
    strcpy_s( Cmd.payload, DEF_BUF_SIZE, "Enter dungeon command boi!" );
    InputCmdMap.emplace( "ENTER", Cmd );

    
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
    //char buf [ DEF_BUF_SIZE ];
    //char message [ DEF_BUF_SIZE ];
    struct sockaddr_in  si_other;
    slen = sizeof( si_other );

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
            char charCmd [ DEF_BUF_SIZE ];
            memcpy( charCmd, ( void* ) ( &currentCommand ), DEF_BUF_SIZE );

            if ( sendto( serverSockID, charCmd, DEF_BUF_SIZE, 0,
                ( struct sockaddr* ) &si_other, slen ) == SOCKET_ERROR )
            {
                printf( "sendto() failed : %d ", WSAGetLastError() );
                exit( EXIT_FAILURE );
            }

            //clear buffer
            //memset( buf, '\0', DEF_BUF_SIZE );

            // Receive something from the server
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
    while ( 1 )
    {
        // Grab console input
        std::cout << "Enter your command: ";
        std::string input = "";
        std::getline( std::cin, input );

        // Convert string to upper case
        for ( auto & c : input ) c = toupper( c );

        // Special case of quiting
        if ( input == "QUIT" || input == "Q" )
        {
            IsDone = true;
            // #TODO: Enqueue a leave command to send to the server
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
    std::cout << "Command list:" << std::endl;

    for ( const auto & itr : InputCmdMap )
    {
        std::cout << "\t" << itr.first << std::endl;
    }
}
