#pragma once

/// <summary>
/// Common structure definitions of commands and other socket uses
/// </summary>
/// <author>Ben Hoffman</author>

#define DEF_BUF_SIZE 256

namespace Networking
{
    /// <summary>
    /// Types of commands that the client can send to the server
    /// </summary>
    enum class ECommandType : UINT8
    {
        ENTER,
        MOVE,
        PICKUP,
        QUIT
    };

    /// <summary>
    /// Types of responses that the server can send to their currently
    /// connected players. 
    /// </summary>
    enum class EResponseType : UINT8
    {
        MAP,        // The map around the player
        INVENTORY       // The stats of this player
    };

    /// <summary>
    /// Data about what things a player has available to them
    /// </summary>
    struct PlayerInventory
    {
        UINT32 TreasureAmount = 0;
    };

    /// <summary>
    /// Command structure for a client to send to the server
    /// </summary>
    struct Command
    {
        /** The type of command that this is */
        ECommandType CmdType;

        /** The ID of the player giving the command */
        UINT8 ID;

        // Command structure
        union
        {
            // MOVE
            struct
            {
                UINT8 Left  : 1;    // Flags for what direction 
                UINT8 Right : 1;    // the player may move
                UINT8 Up    : 1;
                UINT8 Down  : 1;
            } Direction;    // 4 BITS

        } PacketData;
    };

    /// <summary>
    /// A response status that the server can send to their clients
    /// </summary>
    struct Status
    {
        /** The type of response that the server is sending */
        EResponseType ResType;

        // Response structure
        union
        {
            // MAP
            struct
            {
                /** This will be used to tell the player what adjacent tiles 
                are currently around them. */
                char map [ 16 ];
            } MapData;

            // INVENTORY
            PlayerInventory Inventory;

        } PacketData;
    };

}   // namespace Networking