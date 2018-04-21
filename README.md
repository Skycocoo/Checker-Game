# CS4613-AI
Homework for CS4613; Spring 2018

## Installation

Clone this project:

```bash
git clone git@github.com:Skycocoo/CS4613-Artificial-Intelligence.git
cd ./CS4613-Artificial-Intelligence
```

Make build directory for cmake:

```bash
mkdir build
cd ./build/
```

Run cmake:

```bash
cmake ..
```

Run the executable of this project:

```bash
./main
```

## Note

If only one move available: just take the move?

Reset: need to find the exact location to be resumed (because loop is based on index)

> solution: change the structure of Move: mark as dead instead of setting the index directly to -1 -1 (so in reset we could compare the original indices)

> if the two captured checkers have same location? -> its fine? what about the index (they should keep the same index?)


Iterative Deepening Search: need too much space; need to save previous calculation (or change a method to match 15 sec req)


Reset Captured should also keep the index

## Todo

1. If all actions lead to failure: choose random one

2. If only one move available before alphabeta search: just take the move

3. If no move for current player & its no terminal state: call recursion for the opponent
