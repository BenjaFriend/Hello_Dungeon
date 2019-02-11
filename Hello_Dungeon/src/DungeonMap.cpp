#pragma once

#include "DungeonMap.h"

DungeonMap::DungeonMap( UINT8 aSize, UINT32 aTreasureValue, UINT32 aMaxTreasureCount )
    : Size( aSize ), TreasureValue( aTreasureValue ), MaxTreasureCount( aMaxTreasureCount )
{
    // Initialize the map to be empty 
    Map = new char* [ Size ];
    for ( size_t i = 0; i < Size; ++i )
    {
        Map [ i ] = new char [ Size ];
    }

    for ( size_t r = 0; r < Size; ++r )
    {
        for ( size_t c = 0; c < Size; ++c )
        {
            Map [ r ] [ c ] = EMPTY;
        }
    }

    SpawnTreasure();
}

DungeonMap::~DungeonMap()
{
    if ( Map != nullptr )
    {
        for ( size_t i = 0; i < Size; ++i )
        {
            if ( Map [ i ] != nullptr )
            {
                delete []( Map [ i ] );
            }
        }
        delete [] Map;
    }
}

void DungeonMap::PrintMap()
{
    PrintTopBorder();

    for ( size_t r = 0; r < Size; ++r )
    {
        printf( "| " );
        for ( size_t c = 0; c < Size; ++c )
        {
            printf( "%c ", Map [ r ] [ c ] );
        }
        printf( "| \n" );
    }

    PrintTopBorder();
}

void DungeonMap::AddPlayer( UINT8 aID )
{
    if ( PlayerExists( aID ) )
    {
        LOG_TRACE( "Player '%c' already exists in the map!", aID );
        return;
    }
    // Add a player to the map of players
    Vector2 spawnPos = GetRandomEmptyPos();

    PlayerPositions [ aID ] = spawnPos;
    Map [ spawnPos.Row ] [ spawnPos.Col ] = aID;
}

void DungeonMap::RemovePlayer( UINT8 aID )
{
    if ( !PlayerExists( aID ) ) return;

    // Set map position to empty onces the player exits
    Map [ PlayerPositions [ aID ].Row ] [ PlayerPositions [ aID ].Col ] = EMPTY;

    PlayerPositions.erase( aID );
}

void DungeonMap::MovePlayer( UINT8 aID, Vector2 aMovement )
{
    // Ensure that the player actually exists
    if ( !PlayerExists( aID ) )
    {
        LOG_TRACE( "Player '%c' does not exist in the map!", aID );
        return;
    }

    // Calc new position
    Vector2 CurPos = PlayerPositions [ aID ];
    Vector2 NewPos = CurPos + aMovement;

    // You can't move into treasure
    if ( !IsPosEmpty( NewPos ) ) return;

    // Set this players position to the new position
    PlayerPositions [ aID ] = NewPos;

    // Update the map with the new player position
    Map [ CurPos.Row ] [ CurPos.Col ] = EMPTY;
    Map [ NewPos.Row ] [ NewPos.Col ] = aID;
}

UINT32 DungeonMap::AttemptPickUp( UINT8 aID )
{
    if ( !PlayerExists( aID ) ) return 0;

    UINT32 Score = 0;

    static INT8 numNeighbors = LOCAL_MAP_SIZE;
    static INT8 rowNeighbors [] = { -1, -1, -1, +0,   +0, +1, +1, +1 };
    static INT8 colNeighbors [] = { -1, +0, +1, -1,   +1, -1, +0, +1 };

    Vector2 PlayerPos = PlayerPositions [ aID ];

    for ( size_t i = 0; i < numNeighbors; ++i )
    {
        INT8 testX = PlayerPos.Row + rowNeighbors [ i ];
        INT8 testY = PlayerPos.Col + colNeighbors [ i ];

        if ( IsPosSafe( testX, testY ) && Map [ testX ] [ testY ] == TREASURE )
        {
            Score += TreasureValue;
            Map [ testX ] [ testY ] = EMPTY;
            // Only allow for one item to be picked up at a time
            return Score;
        }
    }
    return Score;
}

void DungeonMap::GetAdjacentTiles( UINT8 aID, char * aBuf, size_t aBufSize )
{
    if ( !PlayerExists( aID ) ) return;

    static INT8 numNeighbors = LOCAL_MAP_SIZE;

    assert( aBuf != nullptr && aBufSize >= numNeighbors );

    static INT8 rowNeighbors [] = { -1, -1, -1, +0,   +0, +1, +1, +1 };
    static INT8 colNeighbors [] = { -1, +0, +1, -1,   +1, -1, +0, +1 };

    Vector2 PlayerPos = PlayerPositions [ aID ];
    printf( "Adj. Tiles: \n" );
    // Add neighbors into the char buffer in this order
    for ( int i = 0; i < numNeighbors; ++i )
    {
        INT8 testX = PlayerPos.Row + rowNeighbors [ i ];
        INT8 testY = PlayerPos.Col + colNeighbors [ i ];

        if ( IsPosSafe( testX, testY ) )
        {
            // Put the value in the char buff
            aBuf [ i ] = Map [ testX ] [ testY ];
        }
        else
        {
            // Add an invalid symbol if 
            aBuf [ i ] = INVALID;
        }

        printf( "%c ", aBuf [ i ] );
        if ( i == 2 || i == 4 ) printf( "\n" );
        if ( i == 3 ) printf( "%c ", aID );
    }
    printf( "\n" );

    // Null terminate
    aBuf [ numNeighbors ] = '\0';
    printf( "MAP BUF: %s \n\n", aBuf );
}

void DungeonMap::SpawnTreasure()
{
    srand( static_cast< unsigned int >( time( NULL ) ) );
    // Set treasure value to be random between 1 - value
    TreasureValue = rand() % TreasureValue + 1;

    // Spawn random treasures
    for ( size_t i = 0; i < MaxTreasureCount; ++i )
    {
        UINT8 randRow = static_cast< UINT8 > ( rand() % ( Size - 1 ) );
        UINT8 randCol = static_cast< UINT8 > ( rand() % ( Size - 1 ) );
        Map [ randRow ] [ randCol ] = TREASURE;
    }
}

inline void DungeonMap::PrintTopBorder()
{
    printf( "  " );
    for ( size_t i = 0; i < Size; ++i ) printf( "- " );
    printf( "\n" );
}

inline bool DungeonMap::IsPosSafe( INT8 row, INT8 col )
{
    // Check for the map bounds
    if ( row < 0 || row >= Size ) return false;
    if ( col < 0 || col >= Size ) return false;
    return true;
}

inline bool DungeonMap::IsPosTreasure( Vector2 aPos )
{
    if ( !IsPosSafe( aPos.Row, aPos.Col ) ) return false;

    return ( Map [ aPos.Row ] [ aPos.Col ] == TREASURE );
}

inline bool DungeonMap::IsPosEmpty( Vector2 aPos )
{
    if ( !IsPosSafe( aPos.Row, aPos.Col ) ) return false;

    return ( Map [ aPos.Row ] [ aPos.Col ] == EMPTY );
}

inline bool DungeonMap::PlayerExists( UINT8 aID )
{
    return ( PlayerPositions.find( aID ) != PlayerPositions.end() );
}

inline Vector2 DungeonMap::GetRandomEmptyPos()
{
    Vector2 newPos = { 0, 0 };

    // Loop until we get a empty spot on the map
    while ( Map [ newPos.Row ] [ newPos.Col ] != EMPTY )
    {
        newPos.Row = static_cast< UINT8 > ( rand() % ( Size - 1 ) );
        newPos.Col = static_cast< UINT8 > ( rand() % ( Size - 1 ) );
    }

    return newPos;
}