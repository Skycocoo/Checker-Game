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
board(), human(board, HUSS), comp(board, COMP), search(human, comp, board, this), humanSelect(false){
    GLuint font;
    textured = setTextured("font.png", font);
    text = Text(&textured, font);

    GLuint bg;
    textured = setTextured("background.png", bg);
    background = Object(&textured, bg);
    background.setShape(glm::vec3(screenWidth, screenHeight, 0));
    background.setScale(2 * screenHeight);

    GLuint ti;
    textured = setTextured("title.png", ti);
    title = Object(&textured, ti);
    title.setShape(glm::vec3(screenWidth, screenHeight, 0));
    title.setScale(2 * screenHeight);

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
    search.render();

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

    if (!humanSelect){
        text.renderLeft("Player", 1, 2, 3.5, -3.5);
    }

    if (humanSelect && (human.cur != -1)){
        if (human.moves[human.cur].isCapture()){
            for (size_t i = 0; i< human.moves[human.cur].capture.size(); i++){
                if (human.moves[human.cur].capture[i]){
                    glm::vec3 off((2 * human.moves[human.cur].capture[i].y + 1) * halfTile, (2 * human.moves[human.cur].capture[i].x + 1) * halfTile, 0);
                    checkerH2.setPos(zero.x + off.x, zero.y - off.y);
                    checkerH2.update();
                    checkerH2.render();
                }
            }
        } else {
            for (size_t i = 0; i< human.moves[human.cur].regular.size(); i++){
                if (human.moves[human.cur].regular[i]){
                    glm::vec3 off((2 * human.moves[human.cur].regular[i].y + 1) * halfTile, (2 * human.moves[human.cur].regular[i].x + 1) * halfTile, 0);
                    checkerH2.setPos(zero.x + off.x, zero.y - off.y);
                    checkerH2.update();
                    checkerH2.render();
                }
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


bool Checker::humanAva() const{
    return human.avaMoves() == 0;
}


bool Checker::humanTurn(int x, int y, bool& done){

    // human turn
    if (terminalState()){
        done = true;
        return false;
    }

    convertMouse(x, y);

    // select checker
    // humanSelect = false & want to select the checker
    if (!humanSelect){
        humanSelect = human.select(x, y);
    } else {
        // humanSelect = true & want to reselect
        int originalCur = human.cur;
        bool select = human.select(x, y);
        // std::cout << std::boolalpha << "select?" << select << std::endl;
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
                human.cur = -1;
                // std::cout << board;
                updateMoves();
                return true;

            } else {
                humanSelect = false;
            }
        }
    }
    return false;

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

    humanSelect = false;

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
