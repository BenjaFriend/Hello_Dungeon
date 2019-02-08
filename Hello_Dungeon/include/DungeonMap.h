#pragma once

#include "stdafx.h"

#define TREASURE    'X'
#define PLAYER      'P'
#define WALL        '+'
#define EMPTY       ' '

/// <summary>
/// The map that represents the current dungeon map.
/// The map is an NxN matrix based on the given size
/// </summary>
/// <author>Ben Hoffman</author>
class DungeonMap
{
public:

    /// <summary>
    /// Create a map to use as a dungeon 
    /// </summary>
    /// <param name="aSize">Size of the dungeon ( NxN )</param>
    /// <param name="aTreasureValue">The value of each treasure</param>
    /// <param name="aMaxTreasureCount">Max number of treasure chests in dungeon</param>
    DungeonMap( UINT8 aSize, UINT32 aTreasureValue, UINT32 aMaxTreasureCount );

    ~DungeonMap();

    /// <summary>
    /// Print out the map of the dungeon
    /// </summary>
    void PrintMap();

    /// <summary>
    /// Add a player to the map, putting them in a default
    /// spawn position
    /// </summary>
    void AddPlayer();

    /************************************************************************/
    /* Accessors                                                            */
    /************************************************************************/
    inline const UINT8 GetSize() const { return Size; }
    inline const UINT32 GetTreasureValue() const { return TreasureValue; }
    inline const UINT32 GetMaxTreasureCount() const { return MaxTreasureCount; }

private:

    /// <summary>
    /// Spawn the treasure on the map randomly
    /// </summary>
    void SpawnTreasure();

    /// <summary>
    /// Helper to print the top border of the maze
    /// </summary>
    inline void PrintTopBorder();

    /** The size of this map */
    UINT8 Size = 5;

    /** The value of each treasure */
    UINT32 TreasureValue = 1;

    /** The max number of treasure chests in the dungeon */
    UINT32 MaxTreasureCount = 5;

    /** The 2D grid representing the map */
    char** Map = nullptr;
};