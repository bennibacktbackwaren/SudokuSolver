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
#include "stack.h"

stack_t *stackNew(size_t elementSize) {
    stack_t *vec     = (stack_t *)calloc(1, sizeof(stack_t));
    vec->capacity    = 8;
    vec->data        = calloc(vec->capacity, elementSize);
    vec->elementSize = elementSize;
    vec->length      = 0;

    return vec;
}

void stackDelete(stack_t *stack) {
    free(stack->data);
    free(stack);
}

void stackPush(stack_t *stack, void *value) {
    // Allocate more memory if there's not enough left
    if(stack->capacity == stack->length) {
        stack->capacity += 8;
        stack->data =
            realloc(stack->data, stack->capacity * stack->elementSize);
    }

    memcpy((uint8_t *)stack->data + stack->length * stack->elementSize,
           value,
           stack->elementSize);
    stack->length++;
}

void *stackPop(stack_t *stack) {
    static uint8_t elem[4];
    memcpy(elem,
           (uint8_t *)stack->data + (stack->length - 1) * stack->elementSize,
           stack->elementSize);
    stack->length--;

    // Free some space if theres too much memory unused
    if(stack->capacity - stack->length > 10 &&
       stack->capacity >= 9 * stack->elementSize) {
        stack->data =
            realloc(stack->data, stack->capacity - 8 * stack->elementSize);
        stack->capacity -= 8;
    }

    return elem;
}

bool stackContains(stack_t *stack, void *elem) {
    for(size_t i = 0; i < stack->length; i++) {
        if(memcmp(elem,
                  (uint8_t *)stack->data + i * stack->elementSize,
                  stack->elementSize) == 0)
            return true;
    }

    return false;
}
