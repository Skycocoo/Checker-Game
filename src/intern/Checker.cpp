// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#include "../extern/Checker.h"
#include "../extern/setUp.h"
#include <string>
#include <SDL.h>

#define RESOURCE_FOLDER "../src/asset/"


// represent difficulty of computer
int difficulty = 1;

extern ShaderProgram textured;
extern float screenWidth;
extern float screenHeight;

// constructor
Checker::Checker():
board(), human(board, HUSS), comp(board, COMP), search(human, comp, board), humanSelect(false){
    GLuint font;
    textured = setTextured("font.png", font);
    text = Text(&textured, font);

    GLuint bg;
    textured = setTextured("background.png", bg);
    background = Object(&textured, bg);
    background.setShape(glm::vec3(screenWidth, screenHeight, 0));
    background.setScale(2 * screenHeight);


    XMLLoad xml("sprites.xml");
    GLuint texture;
    textured = setTextured("sprites.png", texture);
    checkerH = Object(&textured, texture, xml.getData("h.png"));
    checkerH2 = Object(&textured, texture, xml.getData("h-1.png"));
    checkerC = Object(&textured, texture, xml.getData("c.png"));
}


void Checker::update(){
    background.update();
}

void Checker::render(){
    background.render();
    text.render("Checker Game", 1, 2, 0, 3.5);

    float halfTile = float(120) / float (720) * screenHeight;
    glm::vec3 zero(-screenWidth, screenHeight, 0);
    glm::vec3 off(halfTile, 3 * halfTile, 0);

    for (size_t i = 0; i < board.b.size(); i++){
        for (size_t j = 0; j < board.b[i].size(); j++){
            if (board.b[i][j] == HUSS){
                glm::vec3 off((2 * j + 1) * halfTile, (2 * i + 1) * halfTile, 0);
                checkerH.setPos(zero.x + off.x, zero.y - off.y);
                checkerH.update();
                checkerH.render();
            } else if (board.b[i][j] == COMP){
                glm::vec3 off((2 * j + 1) * halfTile, (2 * i + 1) * halfTile, 0);
                checkerC.setPos(zero.x + off.x, zero.y - off.y);
                checkerC.update();
                checkerC.render();
            }

        }
    }

}

// convert mouse position to board position
void Checker::convertMouse(int& x, int& y) const {
    // x and y on the board:
    // x goes from -screenHeight to screenHeight
    // y goes from -screenWidth to screenWidth
    int resultX = y / 120;
    int resultY =  x / 120;
    x = resultX;
    y = resultY;
}

void Checker::mouseSelect(int x, int y){
    convertMouse(x, y);
    std::cout << x << " " << y << std::endl;

    // select checker
    // humanSelect = false & want to select the checker
    if (!humanSelect){
        humanSelect = human.select(x, y);
    } else {
        // humanSelect = true & want to reselect
        int originalCur = human.cur;
        bool select = human.select(x, y);
        std::cout << std::boolalpha << "select?" << select << std::endl;
        if (select){
            humanSelect = true;
        } else {
            // humanSelect = true & want to move & need to reset humanSelect
            human.cur = originalCur;
            int origx = human.moves[human.cur].cur.x,
                origy = human.moves[human.cur].cur.y;

            bool target = human.checkMove(x, y);
            if (target) {
                move(origx, origy, x, y, HUSS);
                search.update(origx, origy, x, y, HUSS);
                humanSelect = false;
            } else {
                humanSelect = false;
            }
        }
    }
    std::cout << board;
    updateMoves();
}



// human turn
void Checker::humanTurn(){
    std::cout << "------------Human turn------------" << std::endl;

    // if no legal move for human: do nothing
    if (human.avaMoves() == 0){
        std::cout << "Human does not have any available legal move" << std::endl;
        return;
    }

    // display available checkers for player
    std::cout << human;

    bool select = false;
    int x, y, targX, targY;
    uint bottonS = 0, bottonT = 0;

    while (!select){
        while (bottonS != 1){
            bottonS = SDL_GetMouseState(&x, &y);
            // std::cout << "mouse " << x << " " << y;
        }
        convertMouse(x, y);
        std::cout << x << " " << y;
        if ((select = human.select(x, y))){
            // std::cout << "Please choose the location to move in \'x y\' format" << std::endl;
            bool target = false;
            while (!target){
                while (bottonT != 1){
                    bottonT = SDL_GetMouseState(&targX, &targY);
                }
                convertMouse(targX, targY);
                if ((target = human.checkMove(targX, targY))){
                    move(x, y, targX, targY, HUSS);
                    // update moves for search
                    search.update(x, y, targX, targY, HUSS);
                }
            }
        }
    }

    // display current board
    std::cout << board;
    // update availability for both human and computer
    updateMoves();

    // std::cout << "Please select the checker in \'x y\' format" << std::endl;
    // bool select = false;
    // int x = 0, y = 0;
    // while (!select){
    //     // reads in position for selection
    //     std::cin >> x >> y;
    //     // if the selection is legal
    //     if ((select = human.select(x, y))){
    //         std::cout << "Please choose the location to move in \'x y\' format" << std::endl;
    //         bool target = false;
    //         int targx = 0, targy = 0;
    //         while (!target){
    //             // reads in target position for the selected checker
    //             std::cin >> targx >> targy;
    //             // if the target position is legal
    //             // make the move & update the checker
    //             if ((target = human.checkMove(targx, targy))) {
    //                 move(x, y, targx, targy, HUSS);
    //                 // update moves for search
    //                 search.update(x, y, targx, targy, HUSS);
    //             }
    //             else std::cout << "Not a legal target location; please input correct locaion" << std::endl;
    //         }
    //     } else std::cout << "Not a legal checker to be moved; please correct location" << std::endl;
    // }



}

// computer turn
void Checker::computerTurn(){
    std::cout << "------------Computer turn------------" << std::endl;

    // if no legal move for computer: do nothing
    if (comp.avaMoves() == 0){
        std::cout << "Computer does not have any available legal move" << std::endl;
        return;
    }

    // display available checkers for player
    std::cout << comp;

    // retrieve result from Search
    Result result = search.search(board);
    int x = result.x, y = result.y,
        targx = result.targX, targy = result.targY;

    // update move for computer
    comp.select(x, y, false);
    comp.checkMove(targx, targy);
    move(x, y, targx, targy, COMP);
    search.update(x, y, targx, targy, COMP);

    // display current board
    std::cout << board;
    // update availability for both human and computer
    updateMoves();
}

// update availability for both human and computer (and board)
void Checker::updateMoves(){
    human.updateMoves();
    comp.updateMoves();
}

// move checker for board from (x, y) to (targx, targy)
void Checker::move(int x, int y, int targx, int targy, int type){
    // set current position to blank
    board.b[x][y] = 0;

    // if this is capture move
    if (abs(targy - y) == 2){
        int capY = y + 1;
        if (y - targy > 0) capY = y - 1;

        // move for human
        if (type == HUSS){
            board.b[x - 1][capY] = 0;
            // capture one computer checker
            comp.captured(x - 1, capY);
            board.numC -= 1;
        } else {
            // move for computer
            board.b[x + 1][capY] = 0;
            // capture one human checker
            human.captured(x + 1, capY);
            board.numH -= 1;
        }
    }
    // update target position for current player
    board.b[targx][targy] = type;
}

// play the game
void Checker::play(){
    // intro to checker game
    std::cout << "\nChecker Game, 6 * 6 board" << std::endl;
    std::cout << "The Board is Row-Major; \ne.g. the bottom-left W checker's position is (5 0)" << std::endl;
    std::cout << std::endl;
    std::cout << board;

    // determine the first / second one to make the move for each turn
    std::cout << "\nYou can choose to move first or second" << std::endl;
    std::cout << "Input 1 to take first move; 2 to take second move" << std::endl;
    int flag = 1;
    std::cin >> flag;
    while ((flag != 1) && (flag != 2)) {
        std::cout << "Please input correct number: 1, or 2" << std::endl;
        std::cin >> flag;
    }
    std::string move = (flag == 1) ? "first " : "second ";
    std::cout << "You choose to take " << move << "move" << std::endl;

    // determine the difficulty of the game
    std::cout << "\nYou can choose the difficulty of the game" << std::endl;
    std::cout << "The difficulty ranges from 1 (easiest) to 3 (hardest)" << std::endl;
    std::cout << "Input desired difficulty: 1, 2, or 3" << std::endl;
    std::cin >> difficulty;
    while (difficulty > 3 || difficulty < 1) {
        std::cout << "Please input correct number: 1, 2, or 3" << std::endl;
        std::cin >> difficulty;
    }
    std::cout << "You choose difficulty " << difficulty << std::endl;

    std::cout << "\n------------Start of the game------------\n" << std::endl;

    // if computer move first
    if (flag == 2) computerTurn();

    // game
    while (true){
        humanTurn();
        if (terminalState()) break;
        computerTurn();
        if (terminalState()) break;
    }

    determineWinner();
    std::cout << "------------End of Game------------" << std::endl;
}

// check the terminal state for the game
// terminal if one player does not have any checker
// or there is no legal move for both player
// return: bool
bool Checker::terminalState() const {
    return board.terminalState() || (human.avaMoves() == 0 && comp.avaMoves() == 0);
}

// determine the winner of the game
void Checker::determineWinner() const{
    if (board.numH > board.numC) std::cout << "Winner is Human" << std::endl;
    else if (board.numH == board.numC) std::cout << "There is a draw" << std::endl;
    else std::cout << "Winner is Computer" << std::endl;
}
