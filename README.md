# SudokuSolver

Version 1.0

SudokuSolver is a simple program with intuitive handling to solve classic 9x9 Sudokus. It is written in C99 using SDL2 and SDL2_ttf, renders hardware
accelerated, and runs on all three major platforms. Furthermore SudokuSolver is compatible with high DPI screens.
You can find a pre-built executable for Windows in the bin folder.

##### Table of contents
* [Introduction](#sudokusolver)
* [Usage](#usage)
* [Screenshots](#screenshots)
* [Building](#building)
  * [Linux](#linux)
  * [Windows](#windows-using-msys2)
  * [Mac OS X](#mac-os-x)
* [Legal](#legal)
* [Credits](#credits)

## Usage

The usage is quite intuitive. The cursor is controlled with the classic HJKL control of vi. With the corresponding number keys (1-9) you set the value of a field, with the 0 you delete a field.

* `F4` simplifies the field using the Exact Cover method. Attention: In some cases (with very simple Sudokus) the whole game is solved by simplifying.
* `F5` solves the Sudoku by backtracking with Brute Force.
* `F6` clears the field.

## Building

#### Linux

To compile the project you need to install a usable toolchain with GCC and make. Furthermore the libraries SDL2 and SDL2_ttf and their dependencies are needed. Then simply call make in the project folder.

#### Windows (using MSYS2)

MSYS2 is required to compile the project. Here you can find the download and an installation guide: https://www.msys2.org/
Next, the toolchain and the libraries must be installed. This can be done easily via the MSYS2 shell with the following command:

```pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf```

Finally, all you need to do is run make respectively mingw32-make in the project folder.

###### Additional steps

* In order to remove the automatically displayed console under Windows, the commented out addition at the end of line 3 must be commented back in the makefile.

* To enable support for screens with high DPI values and to enable the visual styles for the dialog boxes, the following command from the Visual Studio Developer Command Prompt must be used to link the supplied manifest file to the executable:

```mt -manifest main.manifest -outputresource:main.exe```

#### Mac OS X

Since I don't own a Mac, I can't give precise instructions for its use. But it will probably be very similar to the procedure under Linux.

## Legal

#### FreeType

Portions of this software are copyright © 2020 The FreeType Project (https://www.freetype.org). All rights reserved.

#### SourceSansPro

Copyright 2010, 2012, 2014 Adobe Systems Incorporated (https://www.adobe.com/), with Reserved Font Name ‘Source’. The full _Open Font License_ can be found in the bin folder.

## Credits

At this point I would like to thank @eikesauer, because I used his code to implement the backtracking:
https://ein-eike.de/2012/03/10/sudoku-per-backtracking-loesen/

## Screenshots

![Screenshot 1](https://github.com/bennibacktbackwaren/SudokuSolver/blob/master/doc/scr1.PNG)
![Screenshot 2](https://github.com/bennibacktbackwaren/SudokuSolver/blob/master/doc/scr2.PNG)
