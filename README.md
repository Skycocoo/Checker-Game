# Checker Game

Yuxi Luo; Project for CS4613; Spring 2018

[Checker Game 6 * 6](https://github.com/Skycocoo/CS4613-Artificial-Intelligence/blob/master/img/rule.png)


# Table of Contents
- [Dependencies](#dependencies)
- [Installation](#installation-with-cmake)
- [General Deign](#general-design)
    - [Classes](#classes)
    - [Search](#search)
    - [Terminal States and Utility](#terminal-states-and-utility)
    - [Level of Difficulty](#level-of-difficulty)
- [Demo](#demo)
    - [GUI](#gui)
    - [Command Line](#command-line)



# Dependencies

Please make sure you have these libraries before installation

```
- OpenGL (find packages with cmake)
- GLEW (find packages with cmake)
- glm (included in the folder)

- SDL2 (find packages with cmake modules)
- SDL2_Image (find packages with cmake modules)
```


# Installation with cmake

Clone this project: (Note that I use .gitmodules to include glm, the clone should be in recursive mode)

```bash
git clone git@github.com:Skycocoo/CS4613-Artificial-Intelligence.git --recursive
cd ./CS4613-Artificial-Intelligence
```

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

# General Design

```
GameState (display)
    -> Text / Object (render)
        -> GLuint texture (texture of elements)
        -> ShaderProgram (graphics)
            -> Matrix (transformation of graphics)

    -> Checker (game)
        -> Board (checker board)
        -> AvaMoves (human)     -> Board -> Move (available moves) -> Point (location)
        -> AvaMoves (computer)  -> Board -> Move (available moves) -> Point (location)
        -> Search (computer)
                -> Board (checker board)
                -> AvaMoves (human)     -> Board -> Move (available moves) -> Point (location)
                -> AvaMoves (computer)  -> Board -> Move (available moves) -> Point (location)
                -> Result (optimal move)
        -> Text / Object (render)

```


## Classes


### AvaMoves

Provides available moves for human / computer. It stores all the checkers for current player, and processes those checkers for every iteration of board, so that it updates the availability of the checkers.

### Board

Provides board for the game. It uses 2D-vector to represent the 6 * 6 board, and updates the board by the execution of moves by human / computer.

### Checker

Manages human and computer players. It stores necessary instances to perform human-turn and computer-turn. For GUI it also includes background and text to display the board & the statistics of Search for GameState.

It takes in the input from GameState and process the human move. After human mvoes, the computer-turn is called immediately, providing corresponding Search results and continue the game.

### GameState

Manages three levels of game mode: Menu level, Game level, and End level

In Menu level, GameState displays instructions of the game, and takes in player's input for their selection to move first / second, and to play with easy / medium / hard mode.

In End level, GameState displays the winner of the game.

In Game level, GameState renders the checker board and generates corresponding checkers on the board. In this level, GameState takes inputs from the mouse to select the checker for player, and to move the checker to the target position. In addition, the statistics of last result from Search are displayed.

When player selected a move, if the move is available, the corresponding available target positions will be highlighted.

### Matrix

Provides mathematics for computing and multiplying matrices

### Move

Holds current checker's position, and contains possible positions for regular move and capture move of the checker

### Object

Stores necessary instances for ShaderProgram to update and render the graphics

### Result

Holds the result of Search. Provides selected position and desired target position for computer.

### Search

Contains human and computer players that separates from Checker, in order to simulate moves for searching optimal move for computer.

### setUp

Contains basic operations to set up window for rendering and generating shaders.

### ShaderProgram

Contains corresponding values that manipulates the attributes of an element that will be rendered.

### Text

Inherits from Object. Provides methods to display texts from font sprite sheet.

### XMLLoad

Processes xml to provide data for sprite sheet, which contains all the textures of checkers.


## Search

### Availability of moves

I uses AvaMoves to generalize availability of moves for human and computer. Inside the class the capture moves and regular moves are differentiated, and the moves are clearly printed to enhance playing experience.

For Search, the execution and reset of a move are implemented with minimum space usage: each specific move is recorded before the execution, and reset after the execution. Because the available moves for human and computer is looped by index in Search, the correct indexing is necessary to ensure the performance of Search. Because of resetting, the space usage is largely reduced (comparing to do a deep copy for every iteration).

### AI algorithm

This project uses [Alpha-Beta Pruning algorithm](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning) for [Min-Max Search](https://www.neverstopbuilding.com/blog/2013/12/13/tic-tac-toe-understanding-the-minimax-algorithm13), which reduces time complexity for traditional Min-Max Search.

In addition, to meet 15 seconds maximum time usage for the searching algorithm, [Iterative Deepening algorithm](https://www.geeksforgeeks.org/iterative-deepening-searchids-iterative-deepening-depth-first-searchiddfs/) is utilized to ensure there is some optimal result before termination of the search.

For each iteration, Alpha-Beta search provide the optimal move from the current search of specific depth, the number of max-val pruning (for Min-Max Search), the number of min-val pruning (for Min-Max Search), and the number of total nodes generated by the search.

When 15 seconds are used up by the search (calculated with standard library chrono), the entire search will be stopped and the last optimal move will be returned to the main game for execution.



## Terminal States and Utility

### Terminal States

Terminal States are determined by two cases:

1. One of the player (human / computer) has no checker on the board

2. Both players (human & computer) do not have any legal move

If in case 1, then the utility is determined by whether computer has checker on the board. If yes, then the utility is 6 (computer wins), or else, the utility is -6 (computer loses).

If in case 2, then the utility is determined by the number of computer checkers subtracted by the number of human checkers. For example, if there is no legal move for both checker, and there are 4 human checkers and 3 computer checkers on the board, the utility becomes 3 - 4 = -1.

### Heuristics

There are 4 heuristics to be considered to estimate the utility. Because the range of utility is [-6, 6] (defined earlier in Terminal States), the estimated utility should also be within the range.

1. the number of computer [0, 6] (weighted by 5 to match the 3 and 4 feature)

2. negative: number of enemy [-6, 0] (weighted by 5 to match the 3 and 4 feature)

3. for each available checker of enemy (human): distance to the other end [0, 30]

4. negative: for each available checker of computer: distance to the other end [-30, 0]

In other words, the further the enemy is, and the fewer checkers the enemy has, the better chance to win the game.



## Level of Difficulty

The level of difficulty of the game is implemented by the maximum depth the Search can go to.

The difficulty ranges from 1 to 3.

When the difficulty is 1: the depth is restricted to 5; 2: the depth is restricted to 15; 3: the depth is unlimited (to 80)



## Demo

### GUI

Menu level and End level:

![alt-text](https://raw.githubusercontent.com/Skycocoo/CS4613-Artificial-Intelligence/master/img/title.png?token=AVi1i8EemFwMo1xWOzqZ5ldSoy9IFjpsks5a6ddUwA%3D%3D)

Game level:

![alt-text](https://raw.githubusercontent.com/Skycocoo/CS4613-Artificial-Intelligence/master/img/game.png?token=AVi1i029mj8DmKLp1ICeI_e7BdcR_jPHks5a6ddmwA%3D%3D)


### Command Line

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
