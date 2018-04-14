#include "../extern/Search.h"

// need to change this later
using namespace std;

Search::Search(const AvaMoves& human, const AvaMoves& comp, const Board& board):
human(human), comp(comp), board(board){
    this->human.updateBoard(board);
    this->comp.updateBoard(board);
    // unnecessary?
    updateMoves();
}

void Search::updateBoard(const Board& board){
    for (size_t i = 0; i < this->board.b.size(); i++){
        for (size_t j = 0; j < this->board.b[i].size(); j++){
            this->board.b[i][j] = board.b[i][j];
        }
    }
    // assume human & comp are updated by main play()
}

// // loop through the human/comp to update their position?
// void updateSearch(){
//
// }

void Search::updateMoves(){
    // remember to update moves for each turn
    human.updateMoves();
    comp.updateMoves();
    board.updateCount();
}

Result Search::search(const Board& board){
    updateBoard(board);
    cout << "Start of searching for computer...\n" << board;
    return iterativeDeep();
}

Result Search::iterativeDeep(int maxDepth){
    Result fmove (-1, -1, -1, -1); // result of alphabeta (first max)
    float util = -std::numeric_limits<float>::max();

    start = std::chrono::system_clock::now();

    Result cmove (-1, -1, -1, -1);
    for (int i = 1; i < maxDepth; i++){
        float tempUtil = alphaBeta(cmove, i);
        cout << "Depth: " << i << " utility: " << tempUtil << endl;
        // if the utility value for the returned move is larger
        // change the current result to the returned move
        if (tempUtil > util) {
            util = tempUtil;
            fmove.update(cmove);
            cout << "\tUpdated utility: " << util << fmove;
        }

        // if the duration >= 14: stop searching
        // need to satisfy the duration requirement (within 15 seconds)
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        if (elapsed.count() >= 14) {
            cout << "This search goes to depth " << i << endl;
            break;
        }
    }

    cout << "Result of this search: (estimated) utility: " << util << fmove;
    return fmove;
}

// return the action or the estimated value?
float Search::alphaBeta(Result& fmove, int depth){
    // parameters: depth
    float alpha = -6, beta = 6;
    return maxVal(alpha, beta, fmove, depth);
}

// maxVal: for COMP player
float Search::maxVal(float alpha, float beta, Result& fmove, int depth){
    cout << endl << "Max; Depth: " << depth << endl << board;
    updateMoves();

    // edge cases

    // parameters: alpha, beta, depth
    if (terminalState()) return utility();
    // if depth == 0: return evaluated utility
    if (depth == 0) return eval(COMP);
    // if times up: evaluated utility
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (elapsed.count() >= 14) return eval(COMP);


    // main function

    cout << comp;

    float v = -6, tempv;
    Result cmove (-1, -1, -1, -1); // result of min
    bool capture = comp.avaCapture();
    for (size_t i = 0; i < comp.moves.size(); i++){
        int posX = comp.moves[i].cur.x, posY = comp.moves[i].cur.y;

        // capture move
        if (capture){
            for (size_t j = 0; j < comp.moves[i].capture.size(); j++){
                if (comp.moves[i].capture[j]){
                    int targX = comp.moves[i].capture[j].x, targY = comp.moves[i].capture[j].y;
                    move(posX, posY, targX, targY, COMP);
                    // cout << "Move: " << endl << board;
                    tempv = minVal(alpha, beta, cmove, depth - 1);
                    reset(posX, posY, targX, targY, COMP);
                    // cout << "Reset: " << endl << board;
                    if (tempv > v){
                        v = tempv;
                        fmove.update(posX, posY, targX, targY);
                    }
                    if (v >= beta) return v;
                    if (v > alpha) alpha = v;
                }
            }

        } else {
            // regular move
            for (size_t j = 0; j < comp.moves[i].regular.size(); j++){
                if (comp.moves[i].regular[j]){
                    int targX = comp.moves[i].regular[j].x, targY = comp.moves[i].regular[j].y;
                    move(posX, posY, targX, targY, COMP);
                    tempv = minVal(alpha, beta, cmove, depth - 1);
                    reset(posX, posY, targX, targY, COMP);
                    if (tempv > v){
                        v = tempv;
                        fmove.update(posX, posY, targX, targY);
                    }
                    if (v >= beta) return v;
                    if (v > alpha) alpha = v;
                }
            }
        }
    }
    return v;
}

// minVal: for HUSS player
float Search::minVal(float alpha, float beta, Result& fmove, int depth){
    cout << endl << "Min; Depth: " << depth << endl << board;
    updateMoves();

    // edge cases

    // parameters: alpha, beta, depth
    if (terminalState()) return utility();
    // if depth == 0: return evaluated utility
    if (depth == 0) return eval(HUSS);
    // if times up: evaluated utility
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (elapsed.count() >= 14) return eval(HUSS);


    // main function

    float v = 6, tempv;
    Result cmove (-1, -1, -1, -1); // result of min
    bool capture = human.avaCapture();
    for (size_t i = 0; i < human.moves.size(); i++){
        int posX = human.moves[i].cur.x, posY = human.moves[i].cur.y;

        // capture move
        if (capture){
            for (size_t j = 0; j < human.moves[i].capture.size(); j++){
                if (human.moves[i].capture[j]){
                    int targX = human.moves[i].capture[j].x, targY = human.moves[i].capture[j].y;
                    move(posX, posY, targX, targY, HUSS);
                    // cout << "Move: " << endl << board;
                    tempv = maxVal(alpha, beta, cmove, depth - 1);
                    reset(posX, posY, targX, targY, HUSS);
                    // cout << "Reset: " << endl << board;

                    if (tempv < v){
                        v = tempv;
                        fmove.update(posX, posY, targX, targY);
                    }
                    if (v <= alpha) return v;
                    if (v < beta) beta = v;
                }
            }

        } else {
            // regular move
            for (size_t j = 0; j < human.moves[i].regular.size(); j++){
                if (human.moves[i].regular[j]){
                    int targX = human.moves[i].regular[j].x, targY = human.moves[i].regular[j].y;
                    move(posX, posY, targX, targY, HUSS);
                    tempv = maxVal(alpha, beta, cmove, depth - 1);
                    reset(posX, posY, targX, targY, HUSS);

                    if (tempv < v){
                        v = tempv;
                        fmove.update(posX, posY, targX, targY);
                    }
                    if (v <= alpha) return v;
                    if (v < beta) beta = v;
                }
            }
        }
    }
    return v;
}

// move from (x, y) to (targx, targy)
// should also take care of the checkers
void Search::move(int x, int y, int targx, int targy, int type){
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

    // update checkers
    if (type == HUSS){
        human.select(x, y, false);
        human.checkMove(targx, targy);
    } else {
        comp.select(x, y, false);
        comp.checkMove(targx, targy);
    }

}

// reset the move from (x, y) to (targx, targy) back to original
void Search::reset(int x, int y, int targx, int targy, int type){
    board.b[targx][targy] = 0;
    // if capture move
    if (abs(targy - y) == 2){
        if (type == HUSS){
            if (y - targy > 0) board.b[x - 1][y - 1] = COMP;
            else board.b[x - 1][y + 1] = COMP;
        } else {
            // this is moves for computer
            if (y - targy > 0) board.b[x + 1][y - 1] = HUSS;
            else board.b[x + 1][y + 1] = HUSS;
        }
        board.updateCount();
    }
    board.b[x][y] = type;

    // update checkers
    if (type == HUSS){
        human.select(targx, targy, false);
        human.reset(x, y);
    } else {
        comp.select(targx, targy, false);
        comp.reset(x, y);
    }

}


// heuristics to estimate the expected utility
float Search::eval(int type) const {
    // parameters: player type, (board)
    // should be in range [-6, 6]

    // features:
    // [0, 6] number of avaliable checkers for current player
    int fea1 = human.avaMoves();
    if (type == COMP) fea1 = comp.avaMoves();

    // [0, 18] for each avalible checker: proximity to the other end
    int fea2 = human.proximity();
    if (type == COMP) fea2 = comp.proximity();

    // [-18, 0] negative: for each avalible checker of enemy: proximity to the other end
    int fea3 = -comp.proximity();
    if (type == COMP) fea3 = -human.proximity();

    // [-6, 0] negative: number of enemy
    int fea4 = -board.numC;
    if (type == COMP) fea4 = -board.numH;

    // linear weighted sum of features
    float result = float(fea1 + fea2 + fea3 + fea4) / float(24) * 6;
    // cout << result << " ";
    return result;
}

// utility value for terminal state: range [-6, 6] = numC - numH
float Search::utility() const {
    return board.numC - board.numH;
}

bool Search::terminalState() const {
    return board.terminalState() || (human.avaMoves() == 0 && comp.avaMoves() == 0);
}
