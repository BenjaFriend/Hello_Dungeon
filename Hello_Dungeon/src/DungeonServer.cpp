#include "stdafx.h"

#include "DungeonServer.h"

using namespace Networking;

DungeonServer::DungeonServer( SERVER_DESC aDesc )
    : Port( aDesc.Port )
{
    // Print out init info
    std::cout << "Create the server!" << std::endl;
    std::cout << "\tPort: " << Port << std::endl;
    std::cout << "\tMax Treasure: " << aDesc.MaxTreasureCount << std::endl;
    std::cout << "\tTreasure Value: " << aDesc.TreasureValue << std::endl;

    // Init the map
    Map = std::make_unique<DungeonMap>(
        aDesc.DungeonSize,
        aDesc.TreasureValue,
        aDesc.MaxTreasureCount
        );

    Map->PrintMap();
}

DungeonServer::~DungeonServer()
{
    Shutdown();

    // Cleanup -------------------
    closesocket( ServerSocket );
    WSACleanup();
}

UINT64 DungeonServer::Run()
{
    // Start the listener thread
    RunningThread = std::thread( &Networking::DungeonServer::ListenThread, this );

    RunningThread.join();

    return 1;
}

void DungeonServer::Shutdown()
{
    isDone = true;

    if ( RunningThread.joinable() )
    {
        RunningThread.join();
    }
}

void DungeonServer::ListenThread()
{
#pragma  region Init Socket
    isDone = false;
    // Init winsoc ---------------------
    WSADATA wsaData;

    struct sockaddr_in server, si_other;
    int slen, recv_len;
    char buf [ DEF_BUF_SIZE ];
    char ip_str [ INET_ADDRSTRLEN ];
    slen = sizeof( si_other );

    LOG_TRACE( "Initializing winsock..." );

    if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
    {
        fprintf( stderr, "FAILED. Error code : %d ", WSAGetLastError() );
        exit( EXIT_FAILURE );
    }

    LOG_TRACE( "Initialized winsock!" );

    // Create listen socket -----------------------
    if ( ( ServerSocket = socket( AF_INET, SOCK_DGRAM, 0 ) ) == SOCKET_ERROR )
    {
        LOG_TRACE( "Could not create socket : %d ", WSAGetLastError() );
        Shutdown();
        return;
    }

    LOG_TRACE( "Socket created." );

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( Port );
#pragma  endregion

    if ( bind( ServerSocket, ( struct sockaddr* )&server, sizeof( server ) ) == SOCKET_ERROR )
    {
        LOG_TRACE( "Could not create socket : %d ", WSAGetLastError() );
        Shutdown();
        exit( EXIT_FAILURE );
    }

    LOG_TRACE( "Bind done." );

    u_long iMode = 0;
    HRESULT iResult = ioctlsocket( ServerSocket, FIONBIO, &iMode );
    if ( iResult != NO_ERROR )
    {
        LOG_TRACE( "ioctlsocket failed with error: %ld\n", iResult );
        Shutdown();
        exit( EXIT_FAILURE );
    }

    // Wait for data ------------------------------
    while ( true )
    {
        if ( isDone )
        {
            return;
        }
        LOG_TRACE( "Waiting for data..." );
        fflush( stdout );

        memset( buf, '\0', DEF_BUF_SIZE );
        if ( ( recv_len = recvfrom( ServerSocket, buf, DEF_BUF_SIZE, 0, ( struct sockaddr* ) &si_other, &slen ) ) == SOCKET_ERROR )
        {
            LOG_TRACE( "recvfrom failed with error code : %d", WSAGetLastError() );
            Shutdown();
            exit( EXIT_FAILURE );
        }

        // Get the IP of this data and see if the client exists
        memset( ip_str, '\0', INET_ADDRSTRLEN );
        inet_ntop( AF_INET, &( si_other.sin_addr ), ip_str, INET_ADDRSTRLEN );
        std::string newIP = ip_str;

        LOG_TRACE( "Data received from %s", ip_str );

        // Memcpy the data that was received into a command data structure
        Command cmd = {};

        // #TODO: Do some kind of data validation so that we don't just copy 
        // random received data to memory
        memcpy( &cmd, ( void* ) buf, sizeof( Command ) );

        switch ( cmd.CmdType )
        {
        case Networking::ECommandType::ENTER:
            AddNewPlayer( cmd.ID );
            break;
        case Networking::ECommandType::MOVE:
        {
            Vector2 movePos = { 0, 0 };
            if ( cmd.PacketData.Direction.Down )
            {
                movePos.Row = 1;
            }
            if ( cmd.PacketData.Direction.Up )
            {
                movePos.Row = -1;
            }
            if ( cmd.PacketData.Direction.Left )
            {
                movePos.Col = -1;
            }
            if ( cmd.PacketData.Direction.Right )
            {
                movePos.Col = 1;
            }

            Map->MovePlayer( cmd.ID, movePos );
            // attempt to move the object
            Map->PrintMap();
        }
        break;
        case Networking::ECommandType::PICKUP:
        {
            // Attempt to pick something up
            PlayerInvMap [ cmd.ID ].TreasureAmount += Map->AttemptPickUp( cmd.ID );
        }
        break;
        case Networking::ECommandType::QUIT:
        {
            LOG_TRACE( "Player %c quits!", cmd.ID );
            Map->RemovePlayer( cmd.ID );
        }
        break;

        default:
        {

        }
        break;
        }
        Map->PrintMap();

        char neighbors [ MAP_BUF_SIZE ] = "\0";

        // Check if the client can pick something up
        Map->GetAdjacentTiles( cmd.ID, neighbors, MAP_BUF_SIZE );

        // Send a response to the client
        char resStatus [ sizeof( Status ) ];
        memset( resStatus, '\0', sizeof( Status ) );

        Status res = {};
        res.ResType = EResponseType::MAP;
        res.Inventory = PlayerInvMap [ cmd.ID ];
        strcpy_s( res.PacketData.MapData.map, MAP_BUF_SIZE, neighbors );
        memcpy( resStatus, ( void* ) ( &res ), sizeof( Status ) );

        // Send a response back to the client with their map info
        if ( sendto(
            ServerSocket,
            resStatus,
            sizeof( Status ),
            0,
            ( struct sockaddr* )&si_other,
            slen ) == SOCKET_ERROR )
        {
            LOG_TRACE( "sendto failed with error code : %d", WSAGetLastError() );
            Shutdown();
            exit( EXIT_FAILURE );
        }

    }
}

void DungeonServer::ProcessLocalConsole()
{
    std::string input = "";
    // #TODO: Actually get the server to be able to process 
    // inputs while listening as well. 
    // The problem is that `recvfrom` is a blocking function, so 
    // the server will never exit that loop unless ... it receives a quit packet?
    // From itself? That seems bad
}

inline void DungeonServer::AddNewPlayer( UINT8 aID )
{
    for ( auto const & id : CurrentPlayers )
    {
        if ( id == aID )
        {
            return;
        }
    }
    LOG_TRACE( "Add new player : %c", aID );
    CurrentPlayers.push_back( aID );
    Map->AddPlayer( aID );
}
