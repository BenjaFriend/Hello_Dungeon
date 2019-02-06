#include "stdafx.h"

#include "DungeonServer.h"

using namespace Networking;

DungeonServer::DungeonServer( SERVER_DESC aDesc )
    :
    Port( aDesc.Port ),
    TreasureCount( aDesc.TreasureCount ),
    MaxTreasureCount( aDesc.MaxTreasureCount )
{
    LOG_TRACE( "Create the server!" );
    LOG_TRACE( "\tPort:\t%d", Port );
    LOG_TRACE( "\tMax Treasure:\t%d", MaxTreasureCount );
    LOG_TRACE( "\tTreasure Count:\t%d", TreasureCount );
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

    // Listen for IO
    //ProcessLocalConsole();
    
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
    }

    LOG_TRACE( "Socket created." );

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( Port );

    if ( bind( ServerSocket, ( struct sockaddr* )&server, sizeof( server ) ) == SOCKET_ERROR )
    {
        LOG_TRACE( "Could not create socket : %d ", WSAGetLastError() );
        exit( EXIT_FAILURE );
    }

    LOG_TRACE( "Bind done." );

    u_long iMode = 0;
    HRESULT iResult = ioctlsocket( ServerSocket, FIONBIO, &iMode );
    if ( iResult != NO_ERROR )
    {
        LOG_TRACE( "ioctlsocket failed with error: %ld\n", iResult );
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
            exit( EXIT_FAILURE );
        }

        // Get the IP of this data and see if the client exists
        memset( ip_str, '\0', INET_ADDRSTRLEN );
        inet_ntop( AF_INET, &( si_other.sin_addr ), ip_str, INET_ADDRSTRLEN );
        std::string newIP = ip_str;

        LOG_TRACE( "Data received from %s", ip_str );
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
