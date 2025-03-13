# Minesweeper Game

A classic Minesweeper implementation using C++ and the Raylib graphics library.

## Description
This is a simple implementation of the classic Minesweeper game. The player must reveal all cells without mines to win, using number clues to identify where mines are located.

## Features
- **10×10 game grid**
- **Graphical UI** with flag and mine indicators
- **Game timer**
- **Win/lose detection**
- **First-click safety** (you won't hit a mine on your first click)

## Prerequisites
Make sure you have the following installed on your system:
- **C++ compiler (g++)**
- **Raylib library**
- **X11 development libraries**

## Installation
1. **Clone this repository:**
   ```sh
   git clone https://github.com/yourusername/minesweeper.git
   cd minesweeper
   ```
2. **Ensure Raylib is installed** on your system. The Makefile expects it to be in `$(HOME)/raylib/src`.
3. **Compile the game using `make`:**
   ```sh
   make
   ```

## How to Play
### Run the game:
```sh
./minesweeper
```

### Controls:
- **Left-click:** Reveal a cell
- **Right-click:** Place/remove a flag
- **R key:** Restart the game
- **ESC key:** Exit the game

### Game Rules:
- Reveal all **non-mine** cells to win.
- If you reveal a **mine**, you lose.
- Numbers indicate how many mines are in the adjacent cells.
- Use **flags** to mark cells you think contain mines.

## Project Structure
```
minesweeper/
│── minesweeper.cpp    # Main source code
│── resources/         # Contains game assets
│   ├── minesweeper_bomb.png  # Mine image
│   ├── minesweeper_flag.png  # Flag image
│── Makefile           # Build configuration
```

## Building from Source
To compile the game, simply run:
```sh
make
```

To clean the build files:
```sh
make clean
```
