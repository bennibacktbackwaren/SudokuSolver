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
#include "resources.h"

static bool wasInit        = false;
static TTF_Font *smallFont = NULL, *largeFont = NULL;

bool resourcesLoad(float diagonalScalingFactor) {
    // Inhibit invalid function call
    if(wasInit) {
        applicationDisplayError("The resources have already been loaded.");
        return false;
    }

    // Open the font with a small size
    smallFont = TTF_OpenFont("./SourceSansPro-Regular.ttf",
                             (int)(14 * diagonalScalingFactor));
    if(smallFont == NULL) {
        applicationDisplayError(TTF_GetError());
        return false;
    }

    TTF_SetFontHinting(smallFont, TTF_HINTING_LIGHT);

    // Open the font with a large size
    largeFont = TTF_OpenFont("./SourceSansPro-Regular.ttf",
                             (int)(16 * diagonalScalingFactor));
    if(largeFont == NULL) {
        TTF_CloseFont(smallFont);
        applicationDisplayError(TTF_GetError());
        return false;
    }

    wasInit = true;
    return true;
}

void resourcesFree(void) {
    // Inhibit invalid function call
    if(!wasInit) {
        return;
    }

    TTF_CloseFont(largeFont);
    TTF_CloseFont(smallFont);
    wasInit = false;
}

TTF_Font *resourcesGetFont(fontsize_t size) {
    // Inhibit invalid function call
    if(!wasInit) {
        applicationDisplayError("The resources haven't yet been loaded.");
        return NULL;
    }

    switch(size) {
        case FS_SMALL:
            return smallFont;
        default:
        case FS_LARGE:
            return largeFont;
    }
}

char *resourcesGetHelpText(void) {
    // This is very dirty!
    static char text[] =
        "SudokuSolver\n"
        "Version 1.0 (built " __DATE__ " at " __TIME__ ")\n"
        "\n"
        "\n"
        "Movement:\n"
        "\n"
        "      k\n"
        "h          l\n"
        "       j\n"
        "\n"
        "\n"
        "Controls:\n"
        "\n"
        "Press one of the number keys 1-9 to set the corresponding value to\n"
        "the current field. To delete the value from a field, press the 0.\n"
        "\n"
        "F1   Opens this help screen.\n"
        "F4   Simplifies the sudoku. *\n"
        "F5   Solves the sudoku.\n"
        "F6   Clears the field.\n"
        "\n"
        "Press ESC to return to the game screen.\n"
        "\n"
        "\n"
        "\n"
        "* This operation may solve the whole sudoku in some cases.";
    return text;
}
