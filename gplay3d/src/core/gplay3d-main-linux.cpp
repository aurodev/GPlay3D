#ifndef GP_NO_PLATFORM
#ifdef __linux__

#include "gplay3d.h"

using namespace gameplay;

extern int __app_argc;
extern char** __app_argv;

/**
 * Main entry point.
 */
int main(int argc, char** argv)
{
    __app_argc = argc;
    __app_argv = argv;

    Game* game = Game::getInstance();

    Platform* platform = Platform::create(game);
    GP_ASSERT(platform);

    platform->start();
    while(platform->processEvents())
    {
        platform->frame();
    }
    platform->stop();

    delete platform;
    return 0;
}

#endif
#endif
