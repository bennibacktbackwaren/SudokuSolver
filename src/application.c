/*
    SudokuSolver - A simple program that solves classic 9x9 Sudokus.
    Copyright (C) 2020 bennibacktbackwaren

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "application.h"

static bool runsOnOSX = false;

int applicationRun(int argc, char *argv[]) {
    // Process the command line arguments
    if(!applicationProcessCommandLine(argc, argv))
        return EXIT_FAILURE;

    // Initialize the 3rd party libraries
    if(!applicationInitializeDependencies())
        return EXIT_FAILURE;

    // Create the window
    if(!windowCreate()) {
        applicationQuitDependencies();
        return EXIT_FAILURE;
    }

    // Load the shared resources
    if(!resourcesLoad(windowGetDiagonalDPIScalingFactor())) {
        applicationQuitDependencies();
        return EXIT_FAILURE;
    }

    // Initialize the high dpi rendering
    windowInitializeHighDPI();

    // Prepare the field
    windowPrepareField();

    // Enter the main update loop
    applicationEnterMessageLoop();

    // Clean up and exit
    fieldCleanUp();
    windowDestroy();
    resourcesFree();
    applicationQuitDependencies();

    return EXIT_SUCCESS;
}

bool applicationProcessCommandLine(int argc, char *argv[]) {
    if(argc > 1) {
        applicationDisplayError("Invalid command line.");
        return false;
    }

    return true;
}

bool applicationInitializeDependencies(void) {
    // Initialize SDL2
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        applicationDisplayError(SDL_GetError());
        return false;
    }

    // Initialize SDL2_ttf
    if(TTF_Init() != 0) {
        applicationDisplayError(TTF_GetError());
        SDL_Quit();
        return false;
    }

    // Set some configuration flags; saves us some work creating the renderer
    const char *platform = SDL_GetPlatform();
    if(strcmp(platform, "Windows") == 0) {
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d");
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    } else if(strcmp(platform, "Mac OS X") == 0) {
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        runsOnOSX = true;
    } else {
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    }
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    return true;
}

void applicationQuitDependencies(void) {
    TTF_Quit();
    SDL_Quit();
}

void applicationDisplayError(const char *message) {
    // SDL guarantees that we can call this function even if SDL isn't
    // initialized
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,   // Flags
                             "An error occurred",    // Title
                             message,                // Message
                             NULL);                  // Parent window handle
}

void applicationEnterMessageLoop(void) {
    SDL_Event event;

    do {
        // Wait for unhandled events
        SDL_WaitEvent(&event);
        // Dispatch the event
        windowHandleEvent(&event);
        // Redraw
        windowRedraw();
    } while(event.type != SDL_QUIT);
}

bool applicationRunsOnOSX(void) {
    return runsOnOSX;
}
