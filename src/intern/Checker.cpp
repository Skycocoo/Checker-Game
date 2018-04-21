#include "../extern/Checker.h"

// need to change this later
using namespace std;

int counter = 0;

Checker::Checker():
board(), human(board, HUSS), comp(board, COMP), search(human, comp, board){}

bool Checker::terminalState() const {
    // cout << human.avaMoves() << " " << comp.avaMoves() << endl;
    return board.terminalState() || (human.avaMoves() == 0 && comp.avaMoves() == 0);
}

void Checker::determineWinnder() const{
    if (board.numH > board.numC) cout << "Winner is Human" << endl;
    else if (board.numH == board.numC) cout << "There is a draw" << endl;
    else cout << "Winner is Computer" << endl;
}


void Checker::move(int x, int y, int targx, int targy, int type){
    // move a checker forom (x, y) to (targx, targy)
    board.b[x][y] = 0;

    // if capture move
    if (abs(targy - y) == 2){
        int capY = y + 1;
        if (y - targy > 0) capY = y - 1;

        if (type == HUSS){
            // if (y - targy > 0) board.b[x - 1][y - 1] = 0;
            // else board.b[x - 1][y + 1] = 0;
            board.b[x - 1][capY] = 0;
            comp.captured(x - 1, capY);
        } else {
            // this is moves for computer
            // if (y - targy > 0) board.b[x + 1][y - 1] = 0;
            // else board.b[x + 1][y + 1] = 0;
            board.b[x + 1][capY] = 0;
            human.captured(x + 1, capY);
        }
    }
    board.b[targx][targy] = type;
    board.updateCount();
}

void Checker::humanTurn(){
    cout << "Human turn" << endl;
    // cout << human << comp;

    if (human.avaMoves() == 0){
        cout << "Human does not have any available legal move" << endl;
        return;
    }

    cout << human;

    // if (counter == 0){
    //     human.select(4, 1);
    //     human.checkMove(2, 3);
    //     move(4, 1, 2, 3, HUSS);
    //     search.update(4, 1, 2, 3, HUSS);
    // } else if (counter == 1){
    //     human.select(4, 3);
    //     human.checkMove(2, 5);
    //     move(4, 3, 2, 5, HUSS);
    //     search.update(4, 3, 2, 5, HUSS);
    // } else if (counter == 2){
    //     human.select(5, 0);
    //     human.checkMove(4, 1);
    //     move(5, 0, 4, 1, HUSS);
    //     search.update(5, 0, 4, 1, HUSS);
    // }
    // ++counter;

    cout << "Please select the checker in \'x y\' format" << endl;
    bool select = false;
    int x = 0, y = 0;
    while (!select){
        cin >> x >> y;
        if ((select = human.select(x, y))){
            cout << "Please choose the location to move in \'x y\' format" << endl;
            bool target = false;
            int targx = 0, targy = 0;
            while (!target){
                cin >> targx >> targy;
                // make the move & update the checker
                if ((target = human.checkMove(targx, targy))) {
                    move(x, y, targx, targy, HUSS);
                    // update moves for search
                    // search's move take care of the update of AvaMoves
                    search.update(x, y, targx, targy, HUSS);
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

    if (comp.avaMoves() == 0){
        cout << "Computer does not have any available legal move" << endl;
        return;
    }

    cout << comp;

    Result result = search.search(board);
    int x = result.x, y = result.y;
    int targx = result.targX, targy = result.targY;

    comp.select(x, y, false);
    comp.checkMove(targx, targy);
    move(x, y, targx, targy, COMP);
    search.update(x, y, targx, targy, COMP);

    // update moves for search?
    // search.comp.select(result.x, result.y);
    // search.comp.checkMove(result.targX, result.targY);
    // search.update(result.x, result.y, result.targX, result.targY, COMP);

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
    // int flag = 1;
    // cin >> flag;
    // if (flag == 2) computerTurn();

    while (true){
        humanTurn();
        if (terminalState()) break;
        computerTurn();
        if (terminalState()) break;
        break;
        // if (counter >= 3) break;
    }

    determineWinnder();
    cout << "End of Game" << endl;
}
