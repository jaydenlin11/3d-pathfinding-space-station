
# Back to the Ship 🚀

A pathfinding adventure through a 3D space station maze—escape detention and reach your spacecraft hangar with the help of your trusty robot companion! 🤖

## Overview

"Back to the Ship" is a personal project where I built a program to navigate a multi-level space station. Starting from a detention cell (marked `S`), the goal is to find the quickest route to the spacecraft hangar (marked `H`) using elevators (`E`) and avoiding walls (`#`). The station’s layout comes from input files, and the program outputs the escape path—either as a map or a coordinate list—using BFS (queue) or DFS (stack) search algorithms.

## Features

- **3D Maze Navigation**: Handles up to 10 square levels with a custom coordinate system. 🌌
- **Search Algorithms**:
  - `--queue` or `-q`: Breadth-first search (BFS) for the shortest path. 🏃‍♂️
  - `--stack` or `-s`: Depth-first search (DFS) for an alternative route. 🧗‍♀️
- **Input Modes**:
  - Map mode (`M`): 2D grid layout per level. 🗺️
  - Coordinate list mode (`L`): Sparse list of key coordinates. 📍
- **Output Modes**:
  - `--output M` or `-o M`: Map with path marked (`n`, `e`, `s`, `w`, or elevator exits `0-9`). 🗺️
  - `--output L` or `-o L`: List of path coordinates. 📝
- **Command-Line Interface**: Uses `getopt_long()` for flexible options like `--help` (`-h`). 💻

## Usage

Run the program with one search method and optional output format:

```bash
./ship --queue < input.txt > output.txt          # BFS, default map output
./ship --stack --output L < input.txt > output.txt  # DFS, list output
./ship --help                                    # Show help message
```

Invalid commands (e.g., missing search option or both `-s` and `-q`) print an error to `cerr` and exit. ⚠️

## Space Station Layout 🏢

- **Tiles**: 
  - `.` (floor), `#` (wall), `S` (start), `H` (hangar), `E` (elevator). 🚪
- **Movement**: North, east, south, west (no diagonals); elevators connect levels at matching coordinates. 🔼
- **Boundaries**: Implicit walls surround each level. 🚧

## Input File Examples 📂

### Map Mode
```
M
2
4
// Two 4x4 levels
.H..
....
E..S
#..#
....
#...
E#..
#...
```

### Coordinate List Mode
```
L
2
4
// Key coordinates
(0,0,1,H)
(0,2,0,E)
(1,2,0,E)
(0,2,3,S)
(0,3,0,#)
```

## Output Examples 🖥️

### Map Mode (BFS)
```
Start in level 0, row 2, column 3
//level 0
.Hww
...n
E..n
#..#
//level 1
....
#...
E#..
#...
```

### List Mode (DFS)
```
//path taken
(0,2,3,w)
(0,2,2,w)
(0,2,1,w)
(0,2,0,n)
```

## Development Notes 🛠️

- Built with C++ and the STL (no external libraries). ⚙️
- Optimized for performance (e.g., `std::ios::sync_with_stdio(false)`). 🚀
- Tested with various station sizes—handles large mazes (up to 65,535 tiles) in under 35 seconds. ⏱️

## Why This Project? 🤔

I wanted to challenge myself with 3D pathfinding and command-line parsing while imagining a sci-fi escape scenario. It’s a mix of problem-solving and creativity—figuring out how to get back to the ship before the station self-destructs (or something equally dramatic)! 💥
