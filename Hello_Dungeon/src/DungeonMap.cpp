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
            Map [ r ] [ c ] = WALL;
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

void DungeonMap::AddPlayer( PlayerID aID )
{
    // Add a player to the map of players
    Position spawnPos = {};
    // Spawn the player someone random on the map that doesn't have
    // treasure
    spawnPos.Row = 0;
    spawnPos.Col = 0;

    PlayerPositions [ aID ] = spawnPos;
    Map [ spawnPos.Row ] [ spawnPos.Col ] = aID;
}

void DungeonMap::SpawnTreasure()
{
    srand( time( NULL ) );
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

inline bool DungeonMap::IsPosTreasure( Position aPos )
{
    // Check for the map bounds
    if ( aPos.Row < 0 || aPos.Row >= Size ) return false;
    if ( aPos.Col < 0 || aPos.Col >= Size ) return false;

    return ( Map [ aPos.Row ] [ aPos.Col ] == TREASURE );
}