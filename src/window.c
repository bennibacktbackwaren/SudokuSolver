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
#include "window.h"

static SDL_Window *window     = NULL;
static SDL_Renderer *renderer = NULL;
static bool renderHelp        = false;
static SDL_Texture *helpText  = NULL;

bool windowCreate(void) {
    window =
        SDL_CreateWindow("SudokuSolver",                   // Title
                         SDL_WINDOWPOS_CENTERED,           // X-coordinate
                         SDL_WINDOWPOS_CENTERED,           // Y-coordinate
                         240 + FIELD_SIZE * SQUARE_SIZE,   // Width
                         240 + FIELD_SIZE * SQUARE_SIZE,   // Height
                         SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_INPUT_FOCUS |
                             SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if(window == NULL) {
        applicationDisplayError(SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if(renderer == NULL) {
        SDL_DestroyWindow(window);
        applicationDisplayError(SDL_GetError());
        return false;
    }

    return true;
}

void windowDestroy(void) {
    SDL_DestroyTexture(helpText);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void windowPrepareField(void) {
    fieldPrepare(window, renderer);

    // Prepare help text
    SDL_Color white = { 0xFF, 0xFF, 0xFF, 0xFF };
    SDL_Surface *sfc =
        TTF_RenderText_Blended_Wrapped(resourcesGetFont(FS_SMALL),
                                       resourcesGetHelpText(),
                                       white,
                                       1000);
    helpText = SDL_CreateTextureFromSurface(renderer, sfc);
    SDL_FreeSurface(sfc);
}

void windowHandleEvent(const SDL_Event *event) {
    switch(event->type) {
        case SDL_KEYDOWN:
            // Block any input except escape if the help screen is open
            if(renderHelp && event->key.keysym.sym != SDLK_ESCAPE)
                return;
            switch(event->key.keysym.sym) {
                case SDLK_h:
                    fieldMove(DIR_LEFT);
                    break;

                case SDLK_j:
                    fieldMove(DIR_DOWN);
                    break;

                case SDLK_l:
                    fieldMove(DIR_RIGHT);
                    break;

                case SDLK_k:
                    fieldMove(DIR_UP);
                    break;

                case SDLK_F1:
                    renderHelp = true;
                    break;

                case SDLK_ESCAPE:
                    renderHelp = false;
                    break;

                case SDLK_F4:
                    fieldSimplify();
                    break;

                case SDLK_F5:
                    fieldSolve(window);
                    break;

                case SDLK_F6:
                    fieldClear(window);
                    break;

                default: {
                    // Map the keycode onto the field
                    int sym = event->key.keysym.sym;

                    if(sym >= SDLK_0 && sym <= SDLK_9)
                        fieldSetValue(sym - SDLK_0);
                }
            }
            break;

        case SDL_WINDOWEVENT:
            switch(event->window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    fieldUpdateLocation(window);
                    break;
            }
            break;
    }
}

void windowRedraw(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if(renderHelp)
        windowRenderHelp();
    else
        fieldDraw(renderer);

    SDL_RenderPresent(renderer);
}

void windowInitializeHighDPI(void) {
    float hscale = 1.f, vscale = 1.f;

    // Get the horizontal and the vertical dpi
    SDL_GetDisplayDPI(SDL_GetWindowDisplayIndex(window),
                      NULL,
                      &hscale,
                      &vscale);

    // Calculate scaling factors
    hscale = hscale > 1.f ?
                 applicationRunsOnOSX() ? hscale / 72.f : hscale / 96.f :
                 hscale;
    vscale = vscale > 1.f ?
                 applicationRunsOnOSX() ? vscale / 72.f : vscale / 96.f :
                 vscale;
    // Apply scaling factor on the renderer
    SDL_RenderSetScale(renderer, hscale, vscale);

    // Apply scaling factor on the window
    int width = 0, height = 0;
    SDL_GetWindowSize(window, &width, &height);
    SDL_SetWindowSize(window, width * hscale, height * vscale);
}

float windowGetDiagonalDPIScalingFactor(void) {
    float dscale = 1.f;

    // Get the diagonal dpi
    SDL_GetDisplayDPI(SDL_GetWindowDisplayIndex(window), &dscale, NULL, NULL);

    // Calculate the scaling factor
    return dscale > 1.f ?
               applicationRunsOnOSX() ? dscale / 72.f : dscale / 96.f :
               dscale;
}

void windowRenderHelp(void) {
    // Get texture size
    int width = 0, height = 0;
    SDL_QueryTexture(helpText, NULL, NULL, &width, &height);

    // Calculate destination rectangle
    SDL_Rect dest = { 50, 50, width, height };

    // Get default scale
    float defaultScaleX = 1.f, defaultScaleY = 1.f;
    SDL_RenderGetScale(renderer, &defaultScaleX, &defaultScaleY);

    // Disable dpi scaling as text gets blurry
    SDL_RenderSetScale(renderer, 1.f, 1.f);
    // Copy text
    SDL_RenderCopy(renderer, helpText, NULL, &dest);
    // Enable dpi scaling again
    SDL_RenderSetScale(renderer, defaultScaleX, defaultScaleY);
}
