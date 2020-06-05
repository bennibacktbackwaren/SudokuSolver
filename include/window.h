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

// SDL includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Local includes
#include "application.h"
#include "field.h"
#include "resources.h"

/**
 * @brief Creates the main window.
 *
 * @return true If the creation was successfull;
 * @return false otherwise.
 */
bool windowCreate(void);

/**
 * @brief Destroys the main window.
 *
 */
void windowDestroy(void);

/**
 * @brief Prepares the field and caches some textures.
 *
 */
void windowPrepareField(void);

/**
 * @brief Handles the specified event.
 *
 * @param event The event to handle.
 */
void windowHandleEvent(const SDL_Event *event);

/**
 * @brief Redraws the window.
 *
 */
void windowRedraw(void);

/**
 * @brief Initialies the high dpi drawing.
 *
 */
void windowInitializeHighDPI(void);

/**
 * @brief Gets the diagonal dpi scaling factor.
 *
 * @return float The diagonal dpi scaling factor to scale fonts.
 */
float windowGetDiagonalDPIScalingFactor(void);

/**
 * @brief Renders the help screen.
 *
 */
void windowRenderHelp(void);
