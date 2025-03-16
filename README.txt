# 3D Pathfinding Space Station

A fun project to navigate a **3D space station** from a detention cell to a spaceship hangar using **maze-solving algorithms**. The goal is to escape from **start (`S`)** to **hangar (`H`)** in a multi-level station using:

- **BFS (queue, `--queue`)** for the shortest path  
- **DFS (stack, `--stack`)** for a valid path  

## 🫠 Build & Run  

### Build the Project  
```sh
make
```

### Run the Pathfinder  
- **DFS (map output)**  
  ```sh
  ./pathfinder --stack < input.txt > output.txt
  ```
- **BFS (coordinate list output)**  
  ```sh
  ./pathfinder --queue --output L < input.txt > output.txt
  ```

## ⚙️ Command-Line Options  
| Option | Description |
|--------|-------------|
| `--stack, -s` | Use DFS |
| `--queue, -q` | Use BFS |
| `--output M|L, -o M|L` | Output as **map (`M`)** or **list (`L`)** (default: `M`) |
| `--help, -h` | Show usage information |
👉 **Must specify either `--stack` or `--queue` (not both).**  

## 🏢 Space Station Layout  
The station has up to **10 square levels** with the following elements:  
- `.` → Floor  
- `#` → Wall  
- `S` → Start position  
- `H` → Hangar (goal)  
- `E` → Elevator  
- Moves: `n`, `e`, `s`, `w` (cardinal directions) or `0-9` (elevator exit)  

## 📚 Input Formats  

### **1️⃣ Map Format (`M`)**  
```
M
2
4
.H..
....
E..S
#..#
....
#...
E#..
#...
```

### **2️⃣ Coordinate List Format (`L`)**  
```
L
2
4
(0,0,1,H)
(0,2,0,E)
(1,2,0,E)
(0,2,3,S)
(0,3,0,#)
```

## 📄 Output Examples  

### **Map Output (BFS Example)**
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

### **List Output (BFS Example)**
```
//path taken
(0,2,3,n)
(0,1,3,n)
(0,0,3,w)
(0,0,2,w)
```

## 🗂 Files  
- `pathfinder.cpp` → Implementation code  
- `Makefile` → Compiles the project  
- `test-*.txt` → Sample test files (e.g., `test-1-qm.txt`)  

## ⚡ Performance  
- **Built in C++** using `std::deque` for stack/queue  
- Uses `getopt_long()` for argument parsing  
- Handles up to **65,535 tiles** in **under 35 seconds**  
- **BFS is fast**, DFS feels **more exploratory**  

Try it out, escape the station, and share your **maze ideas**! 🚀  

📅 *Last updated: March 16, 2025*

