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

void DungeonMap::AddPlayer()
{
    // Add a player to the map of players
}

void DungeonMap::SpawnTreasure()
{
    // Randomly pick spots to spawn a treasure within the 


}

inline void DungeonMap::PrintTopBorder()
{
    printf( "  " );
    for ( size_t i = 0; i < Size; ++i ) printf( "- " );
    printf( "\n" );
}