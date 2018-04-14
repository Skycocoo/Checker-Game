#include "../extern/Checker.h"

// need to change this later
using namespace std;

Checker::Checker(): board(), human(board, HUSS), comp(board, COMP), search(human, comp, board){}

bool Checker::terminalState() const {
    // check if the game is ended

    // NEED: whether there is any legal move for both players
    return board.terminalState() || (human.avaMoves() == 0 && comp.avaMoves() == 0);

}

void Checker::move(int x, int y, int targx, int targy, int type){
    // move a checker forom (x, y) to (targx, targy)
    board.b[x][y] = 0;

    // if capture move
    if (abs(targy - y) == 2){
        if (type == HUSS){
            if (y - targy > 0) board.b[x - 1][y - 1] = 0;
            else board.b[x - 1][y + 1] = 0;
        } else {
            // this is moves for computer
            if (y - targy > 0) board.b[x + 1][y - 1] = 0;
            else board.b[x + 1][y + 1] = 0;
        }

        board.updateCount();
    }
    board.b[targx][targy] = type;
}

void Checker::humanTurn(){
    cout << "Human turn" << endl << human;

    cout << "Please select the checker in \'x y\' format" << endl;
    bool select = false;
    int x = 0, y = 0;
    while (!select){
        cin >> x >> y;
        if ((select = human.select(x, y))){
            // search.human.select(x, y);
            cout << "Please choose the location to move in \'x y\' format" << endl;
            bool target = false;
            int targx = 0, targy = 0;
            while (!target){
                cin >> targx >> targy;
                // make the move & update the checker
                if ((target = human.checkMove(targx, targy))) {
                    move(x, y, targx, targy, HUSS);
                    // search.human.checkMove(targx, targy);
                    search.move(x, y, targx, targy, HUSS);
                }
                else cout << "Not a legal target location; please input correct locaion" << endl;
            }
        } else cout << "Not a legal checker to be moved; please correct location" << endl;
    }

    cout << board;
    updateMoves();

}

void Checker::computerTurn(){
    cout << "Computer turn" << endl;
    cout << comp;

    Result result = search.search(board);
    comp.select(result.x, result.y);
    comp.checkMove(result.targX, result.targY);
    move(result.x, result.y, result.targX, result.targY, COMP);

    // search.comp.select(result.x, result.y);
    // search.comp.checkMove(result.targX, result.targY);
    search.move(result.x, result.y, result.targX, result.targY, COMP);

    cout << board;
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

    // while: input for human player & alpha-beta for computer
    // call checkState() for every white iteration

    // determine the winner / draw

    cout << "\nChecker Game, 6 * 6 board" << endl;
    cout << "The Board is Row-Major; \ne.g. the bottom-left W checker's position is (5, 0)" << endl;
    cout << endl;
    cout << board;

    // cout << "input 1 to take first move; 2 to take second move" << endl;
    int flag = 1;
    // cin >> flag;
    // if (flag == 2) computerTurn();
    while (!terminalState()){
        humanTurn();
        computerTurn();
    }


}
