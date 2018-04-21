// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#include "../extern/Checker.h"
#include <string>

int difficulty = 1;

Checker::Checker():
board(), human(board, HUSS), comp(board, COMP), search(human, comp, board){}

bool Checker::terminalState() const {
    return board.terminalState() || (human.avaMoves() == 0 && comp.avaMoves() == 0);
}

void Checker::determineWinnder() const{
    if (board.numH > board.numC) std::cout << "Winner is Human" << std::endl;
    else if (board.numH == board.numC) std::cout << "There is a draw" << std::endl;
    else std::cout << "Winner is Computer" << std::endl;
}


void Checker::move(int x, int y, int targx, int targy, int type){
    // move a checker forom (x, y) to (targx, targy)
    board.b[x][y] = 0;

    // if capture move
    if (abs(targy - y) == 2){
        int capY = y + 1;
        if (y - targy > 0) capY = y - 1;

        if (type == HUSS){
            board.b[x - 1][capY] = 0;
            comp.captured(x - 1, capY);
        } else {
            // this is moves for computer
            board.b[x + 1][capY] = 0;
            human.captured(x + 1, capY);
        }
    }
    board.b[targx][targy] = type;
    board.updateCount();
}

void Checker::humanTurn(){
    std::cout << "------------Human turn------------" << std::endl;

    if (human.avaMoves() == 0){
        std::cout << "Human does not have any available legal move" << std::endl;
        return;
    }

    std::cout << human;

    std::cout << "Please select the checker in \'x y\' format" << std::endl;
    bool select = false;
    int x = 0, y = 0;
    while (!select){
        std::cin >> x >> y;
        if ((select = human.select(x, y))){
            std::cout << "Please choose the location to move in \'x y\' format" << std::endl;
            bool target = false;
            int targx = 0, targy = 0;
            while (!target){
                std::cin >> targx >> targy;
                // make the move & update the checker
                if ((target = human.checkMove(targx, targy))) {
                    move(x, y, targx, targy, HUSS);
                    // update moves for search
                    // search's move take care of the update of AvaMoves
                    search.update(x, y, targx, targy, HUSS);
                }
                else std::cout << "Not a legal target location; please input correct locaion" << std::endl;
            }
        } else std::cout << "Not a legal checker to be moved; please correct location" << std::endl;
    }

    std::cout << board;
    updateMoves();

}

void Checker::computerTurn(){
    std::cout << "------------Computer turn------------" << std::endl;

    if (comp.avaMoves() == 0){
        std::cout << "Computer does not have any available legal move" << std::endl;
        return;
    }

    std::cout << comp;

    Result result = search.search(board);
    int x = result.x, y = result.y;
    int targx = result.targX, targy = result.targY;

    comp.select(x, y, false);
    comp.checkMove(targx, targy);
    move(x, y, targx, targy, COMP);
    search.update(x, y, targx, targy, COMP);

    std::cout << board;
    updateMoves();
}

void Checker::updateMoves(){
    // remember to update moves for each turn
    human.updateMoves();
    comp.updateMoves();
    board.updateCount();
}

void Checker::play(){
    // determine the first / second one to make the move for each turn

    std::cout << "\nChecker Game, 6 * 6 board" << std::endl;
    std::cout << "The Board is Row-Major; \ne.g. the bottom-left W checker's position is (5 0)" << std::endl;
    std::cout << std::endl;
    std::cout << board;

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

    if (flag == 2) computerTurn();
    while (true){
        humanTurn();
        if (terminalState()) break;
        computerTurn();
        if (terminalState()) break;
    }

    determineWinnder();
    std::cout << "------------End of Game------------" << std::endl;
}
