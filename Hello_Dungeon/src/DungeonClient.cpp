#include "stdafx.h"

#include "DungeonClient.h"

using namespace Networking;

DungeonClient::DungeonClient( CLIENT_DESC aDesc )
{
    std::cout << "\tClient ctor!" << std::endl;

    strcpy_s( ServerAddr, 32, aDesc.ServerAddr );
    ServerPort = aDesc.ServerPort;

    CurrentPort = aDesc.RunningPort;
    
    InitSocket();
}

DungeonClient::~DungeonClient()
{
    std::cout << "\tClient dtor!" << std::endl;
    Shutdown();
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
        &Networking::DungeonClient::ClientSocketThread, 
        this 
    );
}

void DungeonClient::Run()
{


}

void Networking::DungeonClient::Shutdown()
{
    IsDone = true;

    if ( ClientSocketThread.joinable() )
    {
        ClientSocketThread.join();
    }

    // Cleanup Winsock ------------------
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
    char buf [ DEF_BUF_SIZE ];
    char message [ DEF_BUF_SIZE ];
    struct sockaddr_in  si_other;
    slen = sizeof( si_other );

    memset( ( char* ) &si_other, 0, sizeof( si_other ) );
    si_other.sin_family = AF_INET;
    si_other.sin_addr.s_addr = INADDR_ANY;
    si_other.sin_port = htons( CurrentPort );

    wchar_t wtext [ 32 ];
    size_t outSize;
    mbstowcs_s( &outSize, wtext, ServerAddr, strlen( ServerAddr ) + 1 );
    InetPton( AF_INET, wtext, &si_other.sin_addr.S_un.S_addr );
#pragma endregion


    while ( 1 )
    {

        if ( IsDone )
        {
            return;
        }

    }

}
