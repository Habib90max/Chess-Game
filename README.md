# ♟️ Chess Game

A fully functional 2-player chess game built with **C++** and **Raylib**.

## Features
- Complete chess rules (all piece movements)
- Check & Checkmate detection
- Legal move highlighting
- Visual check indicator (red highlight on king)
- Start screen with Play/Exit buttons
- Two-player local gameplay

## Prerequisites
- [Raylib](https://www.raylib.com/) installed on your system
- A C++ compiler (g++, clang++, MSVC)

## Build & Run

### Linux / macOS
```bash
g++ chess.cpp -o chess -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./chess
```

### Windows (MinGW)
```bash
g++ chess.cpp -o chess.exe -lraylib -lopengl32 -lgdi32 -lwinmm
chess.exe
```

## Assets
Place the following files in an `assets/` folder next to the executable:
- `Board.png` — the chessboard
- `Pieces.png` — piece spritesheet (6 pieces × 2 colors)
- `Chess.png` — start screen background

## How to Play
1. Launch the game and click **Start Game**
2. Click a piece to select it — valid moves appear as dots
3. Click a highlighted square to move
4. The king flashes **red** when in check
5. Game ends on **checkmate**

