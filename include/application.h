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
#pragma once

// Standard includes
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// SDL2 includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Local includes
#include "field.h"
#include "resources.h"
#include "window.h"

/**
 * @brief Runs the application.
 *
 * @param argc The command line argument counter.
 * @param argv The command line argument vector.
 * @return int The exit code.
 */
int applicationRun(int argc, char *argv[]);

/**
 * @brief Processes the command line arguments.
 *
 * @param argc The command line argument counter.
 * @param argv The commnd line argument vector.
 * @return true If the command line is valid;
 * @return false otherwise.
 */
bool applicationProcessCommandLine(int argc, char *argv[]);

/**
 * @brief Initializes the 3rd party libraries.
 *
 * @return true If the initialization was successful;
 * @return false otherwise.
 */
bool applicationInitializeDependencies(void);

/**
 * @brief Quits the 3rd party libraries.
 *
 */
void applicationQuitDependencies(void);

/**
 * @brief Displays the specified error.
 *
 * @param message The message to display.
 */
void applicationDisplayError(const char *message);

/**
 * @brief Enters the main update loop.
 *
 */
void applicationEnterMessageLoop(void);

/**
 * @brief Returns a flag indicating if the application is running on Mac OS X.
 *
 * @return true If the application is running on Mac OS x;
 * @return false otherwise.
 */
bool applicationRunsOnOSX(void);
