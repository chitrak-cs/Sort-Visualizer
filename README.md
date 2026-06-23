# 🎨 Sorting Visualizer

A modern Sorting Visualizer built in **C** using the **raylib** graphics library.

It demonstrates how popular sorting algorithms work step-by-step with real-time animations, statistics, and interactive controls.

---

## ✨ Features

- 📊 Animated sorting visualization
- 🎮 Interactive controls
- ⚡ Adjustable animation speed
- 📈 Real-time statistics
  - Comparisons
  - Swaps
- 🔀 Shuffle array instantly
- ⏸ Pause / Resume animation
- 👣 Step-by-step execution
- 📏 Dynamic array size
- 🎨 Modern dark themed UI

---

## Supported Algorithms

- Bubble Sort
- Selection Sort
- Insertion Sort
- Merge Sort
- Quick Sort
- Heap Sort

---

## Controls

| Key | Action |
|------|--------|
| **1** | Bubble Sort |
| **2** | Selection Sort |
| **3** | Insertion Sort |
| **4** | Merge Sort |
| **5** | Quick Sort |
| **6** | Heap Sort |
| **SPACE** | Pause / Resume |
| **R** | Shuffle Array |
| **UP** | Increase Array Size |
| **DOWN** | Decrease Array Size |
| **+** | Increase Speed |
| **-** | Decrease Speed |
| **N** | Execute One Step (Paused Mode) |

---

## Project Structure

```
SortVisualizer/
│
├── src/
│   ├── algorithms/
│   │   ├── bubble_sort.c
│   │   ├── selection_sort.c
│   │   ├── insertion_sort.c
│   │   ├── merge_sort.c
│   │   ├── quick_sort.c
│   │   ├── heap_sort.c
│   │   ├── algorithm.h
│   │   └── ...
│   │
│   ├── renderer.c
│   ├── array.c
│   ├── metrics.c
│   ├── algorithm_manager.c
│   ├── main.c
│   └── ...
│
├── Makefile
└── README.md
```

---

# Build Instructions

## Prerequisites

- GCC
- Make
- raylib

### Ubuntu

Install raylib:

```bash
sudo apt update
sudo apt install libraylib-dev
```

If your package manager doesn't provide raylib:

```bash
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build
cd build
cmake ..
make
sudo make install
```

---

## Clone Repository

```bash
git clone https://github.com/chitrak-cs/Sort-Visualizer.git
cd Sort-Visualizer
```

---

## Build

```bash
make
```

---

## Run

```bash
./visualizer
```

---

## Clean Build

```bash
make clean
```

---

## Technologies Used

- C
- raylib
- GCC
- Makefile

---

## Screenshots

> Add screenshots here after the UI is finalized.

Example:

```
images/main.png
images/bubble_sort.png
images/quick_sort.png
```

---

## Future Improvements

- Radix Sort
- Counting Sort
- Shell Sort
- Cocktail Sort
- Tim Sort
- Intro Sort
- Smooth bar animations
- Sound effects
- Gradient bars
- FPS limiter
- Light/Dark themes
- Export statistics
- Benchmark mode

---

## Author

**Chitrak Betal**

- GitHub: https://github.com/chitrak-cs
- LinkedIn: *(Add your LinkedIn profile here)*

---

## License

This project is licensed under the MIT License.