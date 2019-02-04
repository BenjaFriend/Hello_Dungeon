/// <summary>
/// Commonly used libraries and definitions 
/// </summary>
/// <author>Ben Hoffman</author>

#include <iostream>

#if defined( _DEBUG ) 

#define _CRTDBG_MAP_ALLOC  

#include <crtdbg.h> 

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW

#else

#endif