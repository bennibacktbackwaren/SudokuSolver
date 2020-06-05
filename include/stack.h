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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct stack {
    size_t elementSize;
    size_t length;
    size_t capacity;
    void *data;
} stack_t;

/**
 * @brief Initializes a new stack.
 *
 * @param elementSize The size of an element.
 * @return stack_t* A handle to the new stack.
 */
stack_t *stackNew(size_t elementSize);

/**
 * @brief Frees the specified stack.
 *
 * @param stack The stack to destroy.
 */
void stackDelete(stack_t *stack);

/**
 * @brief Pushes the specified value onto the stack.
 *
 * @param stack The stack to push the element on.
 * @param value The element to push.
 */
void stackPush(stack_t *stack, void *value);

/**
 * @brief Pops the back element of the stack.
 *
 * @param stack The stack to pop the element from.
 * @return void* The value of the back element.
 */
void *stackPop(stack_t *stack);

/**
 * @brief Checks whether the specified stack contains the specified element.
 *
 * @param stack The stack to be searched.
 * @param elem The element to be searched.
 * @return true If the stack contains the specified element;
 * @return false otherwise.
 */
bool stackContains(stack_t *stack, void *elem);
