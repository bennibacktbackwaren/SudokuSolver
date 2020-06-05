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

// SDL2 includes
#include <SDL2/SDL_ttf.h>

// Local includes
#include "application.h"

typedef enum fontsize { FS_SMALL, FS_LARGE } fontsize_t;

/**
 * @brief Loads the shared resources.
 *
 * @param diagonalScalingFactor The diagonal dpi scaling factor.
 * @return true If the initialization was successful;
 * @return false otherwise.
 */
bool resourcesLoad(float diagonalScalingFactor);

/**
 * @brief Frees the shared resources.
 *
 */
void resourcesFree(void);

/**
 * @brief Get the font to draw width.
 *
 * @param size The size of the font.
 * @return TTF_Font* The font to draw with.
 */
TTF_Font *resourcesGetFont(fontsize_t size);

/**
 * @brief Gets the help text.
 *
 * @return char* A string containing the help text.
 */
char *resourcesGetHelpText(void);
