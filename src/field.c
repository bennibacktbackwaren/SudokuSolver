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
#include "field.h"

/**
 * @brief The square currently selected.
 *
 */
static int8_t selectionX = 0, selectionY = 0;
/**
 * @brief The game field.
 *
 */
static uint8_t field[FIELD_SIZE][FIELD_SIZE];
/**
 * @brief Cached textures.
 *
 * @remarks textures[0] contains the help text.
 *          textures[1-9] contains the corresponding number.
 *
 */
static SDL_Texture *textures[10];
/**
 * @brief The size of the window.
 *
 */
static int windowWidth = 0, windowHeight = 0;
/**
 * @brief The default dpi scaling.
 *
 */
static float defaultScaleX = 0, defaultScaleY = 0;
/**
 * @brief A flag indicating if the input square should move automatically.
 *
 */
bool autoMove = true;

void fieldPrepare(SDL_Window *window, SDL_Renderer *renderer) {
    // Get window size
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    // Get default dpi scaling
    SDL_RenderGetScale(renderer, &defaultScaleX, &defaultScaleY);

    /* Cache the textures for performance improvement */
    SDL_Color foreground = { 0xFF, 0xFF, 0xFF, 0xFF };
    SDL_Surface *surface = NULL;

    // Help text
    surface     = TTF_RenderText_Blended(resourcesGetFont(FS_SMALL),
                                     "Press F1 for help.",
                                     foreground);
    textures[0] = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Numbers
    for(int i = 1; i <= 9; i++) {
        surface     = TTF_RenderGlyph_Blended(resourcesGetFont(FS_LARGE),
                                          i + 0x30,
                                          foreground);
        textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void fieldCleanUp(void) {
    size_t num = sizeof(textures) / sizeof(SDL_Texture *);

    for(size_t i = 0; i < num; i++) {
        SDL_DestroyTexture(textures[i]);
    }
}

void fieldMove(direction_t direction) {
    switch(direction) {
        case DIR_UP:
            selectionY = selectionY == 0 ? 0 : selectionY - 1;
            break;

        case DIR_LEFT:
            selectionX--;
            if(selectionX < 0) {
                // Move to the last field of the previous line if automove is
                // activated
                if(autoMove && selectionY > 0) {
                    selectionX = FIELD_SIZE - 1;
                    fieldMove(DIR_UP);
                    // otherwise clip input
                } else {
                    selectionX = 0;
                }
            }
            break;

        case DIR_RIGHT:
            selectionX++;
            if(selectionX == FIELD_SIZE) {
                // Move to the first element of the next line if automove is
                // activated
                if(autoMove && selectionY < FIELD_SIZE - 1) {
                    selectionX = 0;
                    fieldMove(DIR_DOWN);
                    // otherwise clip input
                } else {
                    selectionX = FIELD_SIZE - 1;
                }
            }
            break;

        case DIR_DOWN:
            selectionY =
                selectionY == FIELD_SIZE - 1 ? FIELD_SIZE - 1 : selectionY + 1;
            break;
    }
}

void fieldSetValue(uint8_t value) {
    field[selectionX][selectionY] = value;
    if(autoMove)
        fieldMove(DIR_RIGHT);
}

void fieldDraw(SDL_Renderer *renderer) {
    int fieldSize = FIELD_SIZE * SQUARE_SIZE;
    int fieldX    = (windowWidth / defaultScaleX) / 2 - fieldSize / 2;
    int fieldY    = (windowHeight / defaultScaleY) / 2 - fieldSize / 2;

    fieldHighlightLargeSquares(renderer, fieldX, fieldY);
    fieldHighlightSelectedSquare(renderer, fieldX, fieldY);
    fieldDrawGridAndNumbers(renderer, fieldX, fieldY);

    /* Draw help text */

    // Disable dpi scaling as text gets blurry
    SDL_RenderSetScale(renderer, 1.f, 1.f);
    // Get pre-rendered texture
    SDL_Texture *texture = textures[0];
    // Get texture size
    int w = 0, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    // Determine destination rectangle
    SDL_FRect dest = { defaultScaleX * fieldX,
                       defaultScaleY * (fieldY + fieldSize + 10),
                       w,
                       h };
    // Copy texture
    SDL_RenderCopyF(renderer, texture, NULL, &dest);
    // Enable dpi scaling again
    SDL_RenderSetScale(renderer, defaultScaleX, defaultScaleY);
}

void fieldSimplify(void) {
    bool somethingChanged;

    do {
        somethingChanged = false;

        // Iterate over every field
        for(uint8_t x = 0; x < FIELD_SIZE; x++) {
            for(uint8_t y = 0; y < FIELD_SIZE; y++) {
                // Skip non-empty fields
                if(field[x][y] != 0) {
                    continue;
                }

                // Get all possible numbers for this field
                stack_t *possible = fieldGetPossibleNumbers(x, y);

                // Set the field if its value is explicitly determined
                if(possible->length == 1) {
                    uint8_t value    = *((uint8_t *)stackPop(possible));
                    field[x][y]      = value;
                    somethingChanged = true;
                }

                stackDelete(possible);
            }
        }
    } while(somethingChanged);
}

void fieldSolve(SDL_Window *window) {
    fieldSimplify();

    if(!fieldFillNextSquare(0, 0)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,        // Flags
                                 "SudokuSolver",                    // Title
                                 "The Sudoku can not be solved.",   // Text
                                 window);   // Parent window handle
    }
}

void fieldClear(SDL_Window *window) {
    SDL_MessageBoxData msg;
    SDL_MessageBoxButtonData buttons[2];
    int selection = -1;

    // Initialize message box buttons
    buttons[0].buttonid = 0;
    buttons[0].flags    = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
    buttons[0].text     = "No";

    buttons[1].buttonid = 1;
    buttons[1].flags    = 0;
    buttons[1].text     = "Yes";

    // Initialize message box
    msg.window      = window;
    msg.title       = "SudokuSolver";
    msg.message     = "Are you sure you want to clear the field?";
    msg.numbuttons  = 2;
    msg.flags       = SDL_MESSAGEBOX_WARNING;
    msg.colorScheme = NULL;
    msg.buttons     = buttons;

    // Confirm action
    SDL_ShowMessageBox(&msg, &selection);

    if(selection <= 0)
        return;

    // Clear the field
    memset(field, 0, FIELD_SIZE * FIELD_SIZE * sizeof(uint8_t));
}

void fieldHighlightLargeSquares(SDL_Renderer *renderer,
                                int fieldX,
                                int fieldY) {
    // Defines the four lighter squares of the field
    SDL_Rect lighterSquares[] = {
        { fieldX + 3 * SQUARE_SIZE, fieldY, 3 * SQUARE_SIZE, 3 * SQUARE_SIZE },
        { fieldX, fieldY + 3 * SQUARE_SIZE, 3 * SQUARE_SIZE, 3 * SQUARE_SIZE },
        { fieldX + 6 * SQUARE_SIZE,
          fieldY + 3 * SQUARE_SIZE,
          3 * SQUARE_SIZE,
          3 * SQUARE_SIZE },
        { fieldX + 3 * SQUARE_SIZE,
          fieldY + 6 * SQUARE_SIZE,
          3 * SQUARE_SIZE,
          3 * SQUARE_SIZE }
    };
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderFillRects(renderer, lighterSquares, 4);
}

void fieldHighlightSelectedSquare(SDL_Renderer *renderer,
                                  int fieldX,
                                  int fieldY) {
    SDL_Rect selectedSquare = { fieldX + selectionX * SQUARE_SIZE,
                                fieldY + selectionY * SQUARE_SIZE,
                                SQUARE_SIZE,
                                SQUARE_SIZE };

    SDL_SetRenderDrawColor(renderer, 70, 120, 70, 255);
    SDL_RenderFillRect(renderer, &selectedSquare);
}

void fieldDrawGridAndNumbers(SDL_Renderer *renderer, int fieldX, int fieldY) {
    int size = FIELD_SIZE * SQUARE_SIZE;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(uint8_t x = 0; x < FIELD_SIZE + 1; x++) {
        // Draw horizontal lines
        SDL_RenderDrawLine(renderer,
                           fieldX,
                           fieldY + x * SQUARE_SIZE,
                           fieldX + size,
                           fieldY + x * SQUARE_SIZE);
        // Draw vertical lines
        SDL_RenderDrawLine(renderer,
                           fieldX + x * SQUARE_SIZE,
                           fieldY,
                           fieldX + x * SQUARE_SIZE,
                           fieldY + size);

        for(uint8_t y = 0; y < FIELD_SIZE; y++) {
            // The outer for loop goes up to FIELD_SIZE which would cause an
            // invalid access on the field array
            if(x > FIELD_SIZE - 1)
                break;

            // Draw a 0 as an empty field
            if(field[x][y] == 0)
                continue;

            // Disable dpi scaling as text gets blurry
            SDL_RenderSetScale(renderer, 1.f, 1.f);
            // Get pre-renderered texture
            SDL_Texture *texture = textures[field[x][y]];
            // Get texture size
            int w = 0, h = 0;
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            // Determine destination rectangle
            SDL_FRect dest = { defaultScaleX * (fieldX + x * SQUARE_SIZE) +
                                   ((SQUARE_SIZE * defaultScaleX) / 2 - w / 2),
                               defaultScaleY * (fieldY + y * SQUARE_SIZE) +
                                   ((SQUARE_SIZE * defaultScaleY) / 2 - h / 2),
                               w,
                               h };
            // Copy texture
            SDL_RenderCopyF(renderer, texture, NULL, &dest);
            // Enable dpi scaling again
            SDL_RenderSetScale(renderer, defaultScaleX, defaultScaleY);
        }
    }
}

stack_t *fieldGetPossibleNumbers(uint8_t x, uint8_t y) {
    stack_t *usedNumbers = stackNew(sizeof(uint8_t));
    stack_t *possible    = stackNew(sizeof(uint8_t));

    // Add all used numbers of row x
    for(size_t col = 0; col < FIELD_SIZE; col++) {
        if(field[x][col] != 0)
            stackPush(usedNumbers, &field[x][col]);
    }

    // Add all used numbers of column y
    for(size_t row = 0; row < FIELD_SIZE; row++) {
        if(field[row][y] != 0)
            stackPush(usedNumbers, &field[row][y]);
    }

    // Add all used numbers of the large square
    uint8_t squareX = x / 3;
    uint8_t squareY = y / 3;

    for(size_t row = 0; row < 3; row++) {
        for(size_t col = 0; col < 3; col++) {
            if(field[squareX * 3 + row][squareY * 3 + col] != 0)
                stackPush(usedNumbers,
                          &field[squareX * 3 + row][squareY * 3 + col]);
        }
    }

    // Now add all numbers to possible that aren't stored in usedNumbers
    for(uint8_t i = 1; i < 10; i++) {
        if(!stackContains(usedNumbers, &i))
            stackPush(possible, &i);
    }

    // Clean up and return
    stackDelete(usedNumbers);
    return possible;
}

bool fieldIsSquareValid(uint8_t x, uint8_t y) {
    // Get the current value of the field
    uint8_t val = field[x][y];

    // Check whether the column contains the value already
    for(size_t col = 0; col < FIELD_SIZE; col++) {
        if(field[x][col] == val && col != y)
            return false;
    }

    // Check whether the row contains the value already
    for(size_t row = 0; row < FIELD_SIZE; row++) {
        if(field[row][y] == val && row != x)
            return false;
    }

    // Check whether the large square contains the value already
    uint8_t squareX = x / 3;
    uint8_t squareY = y / 3;

    for(size_t row = 0; row < 3; row++) {
        for(size_t col = 0; col < 3; col++) {
            if(field[squareX * 3 + row][squareY * 3 + col] == val &&
               (squareX * 3 + row) != x && (squareY * 3 + col) != y)
                return false;
        }
    }

    // Everything is fine
    return true;
}

bool fieldFillNextSquare(uint8_t x, uint8_t y) {
    /* Source based on
     * https://ein-eike.de/2012/03/10/sudoku-per-backtracking-loesen/ */

    // Jump to the next row if we are at the end of a row
    if(y >= FIELD_SIZE) {
        y = 0;
        x++;
    }

    // Terminate the recursion if we are done
    if(x >= FIELD_SIZE) {
        return true;
    }

    // Validate the square if it isn't empty
    if(field[x][y] != 0) {
        if(!fieldIsSquareValid(x, y))
            return false;

        // Move to the next square
        return fieldFillNextSquare(x, y + 1);
    } else /* Try every number */ {
        for(field[x][y] = 1; field[x][y] <= 9; field[x][y]++) {
            if(!fieldIsSquareValid(x, y))
                continue;
            if(fieldFillNextSquare(x, y + 1))
                return true;
        }
    }

    // Otherwise reset the field
    field[x][y] = 0;
    return false;
}

void fieldUpdateLocation(SDL_Window *window) {
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
}
