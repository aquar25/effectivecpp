####Using SDL2.0 on Ubuntu16.04 with Clion
1. Install sdl2.0 `sudo apt-get install libsdl2-dev`
2. CMakeLists.txt file
```
cmake_minimum_required(VERSION 3.5)
project(effectivestl)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -gdwarf-3")

set(SOURCE_FILES
        src/main.cpp
        src/baseitem.cpp
        include/baseitem.h
        include/utility.h)

add_executable(effectivestl ${SOURCE_FILES})

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
```
3. Test the dev-environment, main.cpp
```
#include <SDL2/SDL.h>
// pay attention to the header file's path

/******************************************************************************
	example00: Setting up SDL
******************************************************************************/
void example00()
{
    SDL_Window	*pWindow = NULL;
    SDL_Renderer*pRenderer = NULL;

    // 1. initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf ("SDL initialize fail:%s\n", SDL_GetError());
        return;
    }

    // 2. create window
    pWindow = SDL_CreateWindow("example00:Setting up SDL",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               640, 480,
                               SDL_WINDOW_SHOWN);
    if (NULL == pWindow)
    {
        printf ("Create window fail:%s\n", SDL_GetError());
    }

    // 3. create renderer
    pRenderer = SDL_CreateRenderer(pWindow, -1, 0);

    // 4. clear the window to green
    SDL_SetRenderDrawColor(pRenderer,0,255,0,255);
    SDL_RenderClear(pRenderer);

    // 5. show the window
    SDL_RenderPresent(pRenderer);

    SDL_Delay(5000);  // for display

    // 6. exit
    SDL_Quit();
}
```
