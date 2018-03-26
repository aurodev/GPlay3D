#ifndef GP_NO_PLATFORM
#ifdef _WIN32

#include "gplay3d.h"

using namespace gameplay;

#ifndef _WINDOWS_
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

/**
 * Main entry point.
 */
//extern "C" int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
int main(int argc, char *argv[])
{
    Game* game = Game::getInstance();
    Platform* platform = Platform::create(game);
    GP_ASSERT(platform);
    int result = platform->enterMessagePump();
    delete platform;
    return result;
}

#endif
#endif
