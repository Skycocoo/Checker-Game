# CS4613-AI

Homework for CS4613; Spring 2018

[Checker Game 6 * 6](https://github.com/Skycocoo/CS4613-Artificial-Intelligence/blob/master/req/rule.png)

## Installation with cmake

<!-- Clone this project:

```bash
git clone git@github.com:Skycocoo/CS4613-Artificial-Intelligence.git
cd ./CS4613-Artificial-Intelligence
``` -->

Open the zipfile and cd into the folder CS4613-AI in shell

Make build directory for cmake:

```bash
mkdir build
cd ./build/
```

Run cmake & build executable for this project:

```bash
cmake ..
make
```

Run the executable of this project:

```bash
./main
```


## General Design

```
Checker (game)
    -> AvaMoves (human)     -> Move (available moves) -> Point (location)
    -> AvaMoves (computer)  -> Move (available moves) -> Point (location)
    -> Board (checker board)

    -> Search (computer)
            -> AvaMoves (human)     -> Move (available moves) -> Point (location)
            -> AvaMoves (computer)  -> Move (available moves) -> Point (location)
            -> Board (checker board)
            -> Result (result of the search)

```

### Search

#### Availability of moves

I uses AvaMoves to generalize availability of moves for human and computer. Inside the class the capture moves and regular moves are differentiated, and the moves are clearly printed to enhance playing experience.

For Search, the execution and reset of a move are implemented with minimum space usage: each specific move is recorded before the execution, and reset after the execution. Because the available moves for human and computer is looped by index in Search, the correct indexing is necessary to ensure the performance of Search.

#### AI algorithm

This project uses [Alpha-Beta Pruning algorithm](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning) for [Min-Max Search](https://www.neverstopbuilding.com/blog/2013/12/13/tic-tac-toe-understanding-the-minimax-algorithm13), which reduces time complexity for traditional Min-Max Search.

In addition, to meet 15 seconds maximum time usage for the searching algorithm, [Iterative Deepening algorithm](https://www.geeksforgeeks.org/iterative-deepening-searchids-iterative-deepening-depth-first-searchiddfs/) is utilized to ensure there is some optimal result before termination of the search.

For each iteration, Alpha-Beta search provide the optimal move from the current search of specific depth, the number of max-val pruning (for Min-Max Search), the number of min-val pruning (for Min-Max Search), and the number of total nodes generated by the search.

When 15 seconds are used up by the search (calculated with standard library chrono), the entire search will be stopped and the last optimal move will be returned to the main game for execution.

### Terminal States & Utility & Heuristics for computer

#### Terminal States

Terminal States are determined by two cases:

1. One of the player (human / computer) has no checker on the board

2. Both players (human & computer) do not have any legal move

If in case 1, then the utility is determined by whether computer has checker on the board. If yes, then the utility is 6 (computer wins), or else, the utility is -6 (computer loses).

If in case 2, then the utility is determined by the number of computer checkers subtracted by the number of human checkers. For example, if there is no legal move for both checker, and there are 4 human checkers and 3 computer checkers on the board, the utility becomes 3 - 4 = -1.

#### Heuristics

There are 4 heuristics to be considered to estimate the utility. Because the range of utility is [-6, 6] (defined earlier in Terminal States), the estimated utility should also be within the range.

1. the number of computer [0, 6]

2. negative: number of enemy [-6, 0]

3. for each available checker of enemy (human): distance to the other end [0, 30]

4. negative: for each available checker of computer: distance to the other end [-30, 0]

In other words, the further the enemy is, and the fewer checkers the enemy has, the better chance to win the game.


### Level of Difficulty

The level of difficulty of the game is implemented by the maximum depth the Search can go to.

The difficulty ranges from 1 to 3.

When the difficulty is 1: the depth is restricted to 10; 2: the depth is restricted to 15; 3: the depth is unlimited (to 80)


### Demo

```
Checker Game, 6 * 6 board
The Board is Row-Major;
e.g. the bottom-left W checker's position is (5 0)

Board:
| |0|1|2|3|4|5|
|0| |W| |W| |W|
|1|W| |W| |W| |
|2| | | | | | |
|3| | | | | | |
|4| |B| |B| |B|
|5|B| |B| |B| |
Number of human: 6
Number of computer: 6


You can choose to move first or second
Input 1 to take first move; 2 to take second move
1
You choose to take first move

You can choose the difficulty of the game
The difficulty ranges from 1 (easiest) to 3 (hardest)
Input desired difficulty: 1, 2, or 3
3
You choose difficulty 3

------------Start of the game------------

------------Human turn------------
Human: 5 available moves
	Checker (4, 1)  
		Regular move: (3, 0)  (3, 2)  
	Checker (4, 3)  
		Regular move: (3, 2)  (3, 4)  
	Checker (4, 5)  
		Regular move: (3, 4)  

Please select the checker in 'x y' format
4 1
You selected 	Checker (4, 1)  
		Regular move: (3, 0)  (3, 2)  
Please choose the location to move in 'x y' format
3 0
Board:
| |0|1|2|3|4|5|
|0| |W| |W| |W|
|1|W| |W| |W| |
|2| | | | | | |
|3|B| | | | | |
|4| | | |B| |B|
|5|B| |B| |B| |
Number of human: 6
Number of computer: 6

------------Computer turn------------
Computer: 5 available moves
	Checker (1, 0)  
		Regular move: (2, 1)  
	Checker (1, 2)  
		Regular move: (2, 1)  (2, 3)  
	Checker (1, 4)  
		Regular move: (2, 3)  (2, 5)  

Start of searching for computer...
Board:
| |0|1|2|3|4|5|
|0| |W| |W| |W|
|1|W| |W| |W| |
|2| | | | | | |
|3|B| | | | | |
|4| | | |B| |B|
|5|B| |B| |B| |
Number of human: 6
Number of computer: 6

This search goes to depth 24
Result of this search:
	utility: 0 number of max pruning: 483175 min pruning: 1402297 nodes: 4817062
	From (1, 0) to (2, 1)
Board:
| |0|1|2|3|4|5|
|0| |W| |W| |W|
|1| | |W| |W| |
|2| |W| | | | |
|3|B| | | | | |
|4| | | |B| |B|
|5|B| |B| |B| |
Number of human: 6
Number of computer: 6

------------Human turn------------
Human: 5 available moves
	Checker (4, 3)  
		Regular move: (3, 2)  (3, 4)  
	Checker (4, 5)  
		Regular move: (3, 4)  
	Checker (5, 0)  
		Regular move: (4, 1)  
	Checker (5, 2)  
		Regular move: (4, 1)  

Please select the checker in 'x y' format
4 2
Not a legal checker to be moved; please correct location
4 3
You selected 	Checker (4, 3)  
		Regular move: (3, 2)  (3, 4)  
Please choose the location to move in 'x y' format
3 2
Board:
| |0|1|2|3|4|5|
|0| |W| |W| |W|
|1| | |W| |W| |
|2| |W| | | | |
|3|B| |B| | | |
|4| | | | | |B|
|5|B| |B| |B| |
Number of human: 6
Number of computer: 6

------------Computer turn------------
Computer: 1 available moves
	Checker (2, 1)  
		Capture move: (4, 3)  

Start of searching for computer...
Board:
| |0|1|2|3|4|5|
|0| |W| |W| |W|
|1| | |W| |W| |
|2| |W| | | | |
|3|B| |B| | | |
|4| | | | | |B|
|5|B| |B| |B| |
Number of human: 6
Number of computer: 6

Only one move available for computer
Board:
| |0|1|2|3|4|5|
|0| |W| |W| |W|
|1| | |W| |W| |
|2| | | | | | |
|3|B| | | | | |
|4| | | |W| |B|
|5|B| |B| |B| |
Number of human: 5
Number of computer: 6

------------Human turn------------
Human: 2 available moves
	Checker (5, 2)  
		Capture move: (3, 4)  
	Checker (5, 4)  
		Capture move: (3, 2)  

Please select the checker in 'x y' format
[...]
```

<!-- ### Checker game rules

![alt-text](https://raw.githubusercontent.com/Skycocoo/CS4613-Artificial-Intelligence/master/req/rule.png?token=AVi1i2_xmQ5NbJzwQiruIj_xGn8viu-oks5a5P-hwA%3D%3D) -->
