#pragma once

#include "stdafx.h"

#include <stdlib.h>     // srand, rand 
#include <time.h>       // time 
#include <unordered_map>

#define TREASURE    '!'
#define PLAYER      'P'
#define WALL        '0'
#define EMPTY       '+'

/// <summary>
/// A struct representing a position of something
/// on this map
/// </summary>
struct Vector2
{
    INT8 Row = 0;
    INT8 Col = 0;

    Vector2 operator +( const Vector2 & aOther )
    {
        Vector2 pos;
        pos.Row = this->Row + aOther.Row;
        pos.Col = this->Col + aOther.Col;
        return pos;
    }
};

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
    void AddPlayer( UINT8 aID );

    /// <summary>
    /// Attempt to move the player through the dungeon
    /// in the given direction
    /// </summary>
    /// <param name="aID">ID of the player</param>
    /// <param name="aMovement">Direction to move in the dungeon</param>
    void MovePlayer( UINT8 aID, Vector2 aMovement );

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

    /// <summary>
    /// Checks if the given row/col in the map is a treasure 
    /// or not
    /// </summary>
    /// <param name="aPos">The position to check</param>
    /// <returns>True if the position is a treasure</returns>
    inline bool IsPosTreasure( Vector2 aPos );

    /// <summary>
    /// Returns true if the given pos is within the range of the dungeon
    /// and is an empty spot
    /// </summary>
    /// <param name="aPos">The position to check</param>
    /// <returns></returns>
    inline bool IsPosValid( Vector2 aPos );

    /// <summary>
    /// Checks if this player exists in the map or not
    /// </summary>
    /// <param name="aID">Player ID to check</param>
    /// <returns>true if the player exists</returns>
    inline bool PlayerExists( UINT8 aID );

    /** The size of this map */
    UINT8 Size = 5;

    /** The value of each treasure */
    UINT32 TreasureValue = 1;

    /** The max number of treasure chests in the dungeon */
    UINT32 MaxTreasureCount = 5;

    /** The 2D grid representing the map */
    char** Map = nullptr;

    /** A map of player positions */
    std::unordered_map<UINT8, Vector2> PlayerPositions;
};