#pragma once

namespace Helpers
{
    void PrintHelp()
    {
        fprintf( stderr, "\nUSAGE: ./Hello_Dungeon.exe [-%s] <SERVER OPTIONS> ... [-%s] <CLIENT OPTIONS>\n", SERVER_CMD, CLIENT_CMD );

        fprintf( stderr, "\t-h\tPrint this helpful message.\n" );
        fprintf( stderr, "\nSERVER Options: \n" );
        fprintf( stderr, "\t-s\tSize of dungeon\n" );
        fprintf( stderr, "\t-t\tthe treasure count\n" );
        fprintf( stderr, "\t-m\tMax Treasure Count\n" );
        fprintf( stderr, "\t-p\tPort to connect to\n" );
    }
}