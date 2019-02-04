#pragma once

/// <summary>
/// Common structure definitions of commands and other socket uses
/// </summary>
/// <author>Ben Hoffman</author>


#define DEF_BUF_SIZE 256


namespace Networking
{

    struct Command
    {
        unsigned char cmd;
        char payload [ DEF_BUF_SIZE ];
    };

    struct Status
    {
        unsigned char status;
        char payload [ DEF_BUF_SIZE ];
    };

}
