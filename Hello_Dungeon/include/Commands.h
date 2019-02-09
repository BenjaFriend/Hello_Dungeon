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

    struct Command
    {
        ECommandType CmdType;

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

            // Quit and enter don't really need any data to be sent along 
            struct
            {
                UINT8 ID;
            } EnterData;

        } PacketData;
    };

    struct Status
    {
        unsigned char status;
        char payload [ DEF_BUF_SIZE ];
    };

}
