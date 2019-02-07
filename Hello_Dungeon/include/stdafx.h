/// <summary>
/// Commonly used libraries and definitions 
/// </summary>
/// <author>Ben Hoffman</author>

/************************************************************************/
/* Common headers / libraries                                           */
/************************************************************************/
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <atomic>
#include <thread>

/************************************************************************/
/* Number definitions                                                   */
/************************************************************************/
typedef unsigned __int64        UINT64;
typedef unsigned __int32        UINT32;
typedef unsigned __int8         UINT8;
typedef unsigned short          USHORT;
typedef signed __int64          INT64;
typedef signed __int32          INT32;
typedef signed __int8           INT8;
typedef signed short            SHORT;


/************************************************************************/
/* Debug settings                                                       */
/************************************************************************/
#if defined( _DEBUG ) 

#define _CRTDBG_MAP_ALLOC  

#include <crtdbg.h> 

//#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
//#define new DEBUG_NEW

#define LOG_TRACE(a, ...) printf("%s: %d(): " a "\n", __FILE__, __LINE__, __VA_ARGS__);

#else

#define LOG_TRACE(a, ...)

#endif