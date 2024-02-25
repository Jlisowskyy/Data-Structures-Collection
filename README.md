# Data Structures Collection

## Table of Contents
1. [Introduction](#introduction)
2. [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Integration](#integration)
    - [Installation](#installation)
3. [Roadmap](#roadmap)
4. [License](#license)
## Introduction

This repository is a practical collection of various data structures that I've put together through my studies. 
It provides a good mix of both basic and complex data structures.
The repository also includes several easy-to-understand presentation functions.
These let me get a hands-on feel of these data structures, using own examples to truly grasp how they work.
What's more, parts of this code can be easily adjusted to fit into other projects.

## Getting Started

### Prerequisites

- any C++ compiler
- cmake - optionally

### Integration:

Most of the structures are implemented as templates inside their own headers,
so they can be easily linked statically to any project.
To do so, simply copy one of the headers into your project's directory and include it inside your sources.

### Installation

Firstly, clone the repository:

```shell
git clone https://github.com/Jlisowskyy/Data-Structures-Collection ; cd Data-Structures-Collection
```

Simple compilation, for example, with gcc compiler:

```sh
g++ -O3 -std=c++20 main.cpp src/* -o MyProgram.exe
```

Or use cmake:

```shell
cmake CMakeLists.txt ; make
```

## Roadmap

Progress in this repository may be slower due to demanding university duties.
However, plans include the incorporation of the following structures and algorithms:

1) Sorting algorithms:
    - [x] Quick Sort for lists and arrays
    - [ ] Merge Sort and modification
    - [ ] Insertion and Selection sorts
    - [ ] Shell Sort
    - [ ] Intro Sort
2) Priority queues:
    - [x] Simple array-based heap
    - [x] Binary array-based heap - beap
    - [x] Leftist heap
    - [x] Binomial Queue 
    - [ ] Fibonacci heap
    - [ ] Pairing heap
3) Dictionaries:
    - [x] Dynamic Perfect Hashing - chain hashing with plain hashmaps inside the buckets
    - [x] Chain hashing with list buckets
    - [x] BST-Tree
    - [x] Splay-Tree
    - [x] AVL Tree
    - [ ] Red-Black Tree
4) Positional search trees:
    - [ ] RST tree
    - [ ] TRIE tree
    - [ ] PATRICIA tree

The Last three are already implemented, but not using templates, so I didn't mark them.

## License

Distributed under the MIT License. See `LICENSE.txt` for more information.
