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

/* Do not change unless you know what you are doing.
 * Other field sizes than 9 aren't implemented.
 */
#define FIELD_SIZE  9    // sqrt of the number of squares
#define SQUARE_SIZE 32   // Size of the squares in pixels

// Standard includes
#include <stdint.h>

// SDL2 includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Local includes
#include "direction.h"
#include "resources.h"
#include "stack.h"

/**
 * @brief Prepares the field and caches some textures.
 *
 * @param window The window to render on.
 * @param renderer The renderer to render with.
 */
void fieldPrepare(SDL_Window *window, SDL_Renderer *renderer);

/**
 * @brief Cleans up the field and frees some resources.
 *
 */
void fieldCleanUp(void);

/**
 * @brief Moves the selection.
 *
 * @param direction The direction in which the selection should be moved.
 */
void fieldMove(direction_t direction);

/**
 * @brief Sets the value of the selected square.
 *
 * @param value The new value for the selected square.
 */
void fieldSetValue(uint8_t value);

/**
 * @brief Draws the field.
 *
 * @param renderer The renderer to draw with.
 */
void fieldDraw(SDL_Renderer *renderer);

/**
 * @brief Treats the field as an exact cover problem and simplifies it.
 *
 * @remarks Mainly with simple Sudokus the puzzle is sometimes solved completely
 * by this method alone. But sometimes not.
 */
void fieldSimplify(void);

/**
 * @brief Solves the field.
 *
 * @param window The parent window handle in case a message needs to be
 * displayed.
 */
void fieldSolve(SDL_Window *window);

/**
 * @brief Clears the field.
 *
 * @param window The parent window for the confirmation message box.
 */
void fieldClear(SDL_Window *window);

/**
 * @brief Highlights every second large square.
 *
 * @param renderer The renderer to draw with.
 * @param fieldX The x-coordinate of the field.
 * @param fieldY The y-coordinate of the field.
 */
void fieldHighlightLargeSquares(SDL_Renderer *renderer, int fieldX, int fieldY);

/**
 * @brief Highlights the selected square.
 *
 * @param renderer The renderer to draw with.
 * @param fieldX The x-coordinate of the field.
 * @param fieldY The y-coordinate of the field.
 */
void fieldHighlightSelectedSquare(SDL_Renderer *renderer,
                                  int fieldX,
                                  int fieldY);

/**
 * @brief Draws the grid and the numbers.
 *
 * @param renderer The renderer to draw with.
 * @param fieldX The x-coordinate of the field.
 * @param fieldY The y-coordinate of the field.
 *
 * @remarks For performance reasons, I have combined the drawing of the grid and
 * the numbers, as this saves us a loop.
 */
void fieldDrawGridAndNumbers(SDL_Renderer *renderer, int fieldX, int fieldY);

/**
 * @brief Gets all possible numbers for the specified square.
 *
 * @param x The x-coordinate of the field.
 * @param y The y-coordinate of the field.
 * @return stack_t* A stack containing all possible number.
 */
stack_t *fieldGetPossibleNumbers(uint8_t x, uint8_t y);

/**
 * @brief Gets a value indicating whether a square contains a valid number. Used
 * for backtracking.
 *
 * @param x The x-coordinate of the field.
 * @param y The y-coordinate of the field.
 * @return true If the field contains a valid number;
 * @return false otherwise.
 */
bool fieldIsSquareValid(uint8_t x, uint8_t y);

/**
 * @brief Fills the next square. Used for backtracking.
 *
 * @param x The x-coordinate of the field.
 * @param y The y-coordinate of the field.
 * @return true If the branch does not produce a contradiction;
 * @return false otherwise.
 */
bool fieldFillNextSquare(uint8_t x, uint8_t y);

/**
 * @brief Updates the of the field so that it's centered on the specified
 * window.
 *
 * @param window The window to center the field on.
 */
void fieldUpdateLocation(SDL_Window *window);
