#include "../extern/Search.h"

// need to change this later
using namespace std;

bool debug = true;

Search::Search(const AvaMoves& human, const AvaMoves& comp, const Board& board):
human(human), comp(comp), board(board){
    this->human.updateBoard(this->board);
    this->comp.updateBoard(this->board);
    // cout << human;
    // cout << comp;

    // unnecessary?
    // updateMoves();
}

void Search::updateBoard(const Board& board){
    for (size_t i = 0; i < this->board.b.size(); i++){
        for (size_t j = 0; j < this->board.b[i].size(); j++){
            this->board.b[i][j] = board.b[i][j];
        }
    }
    // assume human & comp are updated by main play()
}

// // or loop through the human/comp to update their position?
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
    updateMoves();

    cout << "Start of searching for computer...\n" << board << human << comp;
    return iterativeDeep();
}

Result Search::iterativeDeep(int maxDepth){
    Result fmove (-1, -1, -1, -1); // result of alphabeta (first max)
    float util = -std::numeric_limits<float>::max();

    start = std::chrono::system_clock::now();

    Result cmove (-1, -1, -1, -1); // result of each iteration

    if (debug){
        for (int i = 4; i < maxDepth; i++){
            float tempUtil = alphaBeta(cmove, i);
            cout << "Depth: " << i << " utility: " << tempUtil << endl;
            // if the utility value for the returned move is larger
            // change the current result to the returned move
            if (tempUtil > util) {
                util = tempUtil;
                fmove.update(cmove);
                cout << "\tUpdated utility: " << util << fmove;
            }

            // cout << "after reset: " << board << human << comp << endl;

            // if the duration >= 14: stop searching
            // need to satisfy the duration requirement (within 15 seconds)
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            if (elapsed.count() >= 14) {
                cout << "This search goes to depth " << i << endl;
                break;
            }
        }
    } else {
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

            // cout << "after reset: " << board << human << comp << endl;

            // if the duration >= 14: stop searching
            // need to satisfy the duration requirement (within 15 seconds)
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            if (elapsed.count() >= 14) {
                cout << "This search goes to depth " << i << endl;
                break;
            }
        }
    }

    cout << "Result of this search: (estimated) utility: " << util << fmove;
    return fmove;
}

// return the action or the estimated value?
float Search::alphaBeta(Result& fmove, int depth){
    // parameters: depth
    float alpha = -6, beta = 6;
    // cout << human;
    // cout << comp;
    return maxVal(alpha, beta, fmove, depth);
}

// maxVal: for COMP player
float Search::maxVal(float alpha, float beta, Result& fmove, int depth){
    if (debug) cout << endl << "Max; Depth: " << depth << endl << board;
    updateMoves();

    // edge cases

    // parameters: alpha, beta, depth
    if (terminalState()) return utility();
    // if depth == 0: return evaluated utility
    if (depth == 0) {
        float v = eval();
        cout << "Max; Depth: " << depth << " utility: " << v << endl;
        return v;
    }
    // if times up: evaluated utility
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (elapsed.count() >= 14) return eval();


    // main function
    // cout << "Max; Depth: " << depth << " " << comp;

    float v = -6, tempv;
    Result cmove (-1, -1, -1, -1); // result of min
    bool capture = comp.avaCapture();
    for (size_t i = 0; i < comp.moves.size(); i++){
        if (!comp.moves[i]) continue;

        int posX = comp.moves[i].cur.x, posY = comp.moves[i].cur.y;

        // capture move
        if (capture){
            for (size_t j = 0; j < comp.moves[i].capture.size(); j++){
                if (comp.moves[i].capture[j]){
                    int targX = comp.moves[i].capture[j].x, targY = comp.moves[i].capture[j].y;
                    move(posX, posY, targX, targY, COMP);
                    // cout << "Move: " << endl << board << comp << human;
                    tempv = minVal(alpha, beta, cmove, depth - 1);
                    reset(posX, posY, targX, targY, COMP);
                    // cout << "Reset: " << endl << board << comp << human;
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
    cout << "Max; Depth: " << depth << " utility: " << v << endl;
    return v;
}

// minVal: for HUSS player
float Search::minVal(float alpha, float beta, Result& fmove, int depth){
    if (debug) cout << endl << "Min; Depth: " << depth << endl << board;
    updateMoves();

    // edge cases

    // parameters: alpha, beta, depth
    if (terminalState()) return utility();
    // if depth == 0: return evaluated utility
    if (depth == 0) {
        float v = eval();
        cout << "Min; Depth: " << depth << " utility: " << v << endl;
        return v;
    }
    // if times up: evaluated utility
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (elapsed.count() >= 14) return eval();


    // main function
    // need to update AvaMoves as well

    // cout << "Min; Depth: " << depth << " " << human;

    float v = 6, tempv;
    Result cmove (-1, -1, -1, -1); // result of min
    bool capture = human.avaCapture();
    for (size_t i = 0; i < human.moves.size(); i++){
        if (!human.moves[i]) continue;

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
                    cout << human.moves[i] << " " << targX << " " << targY << endl;
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
    cout << "Min; Depth: " << depth << " utility: " << v << endl;
    return v;
}

// move from (x, y) to (targx, targy)
// should also take care of the checkers
void Search::move(int x, int y, int targx, int targy, int type){
    cout << "from " << x << " " << y << " to " << targx << " " << targy << endl;
    bool fail = false;

    // update checkers
    if (type == HUSS) {
        if (!(human.select(x, y, debug) && human.checkMove(targx, targy))) fail = true;
    }
    else {
        if (!(comp.select(x, y, debug) && comp.checkMove(targx, targy))) fail = true;
    }

    if (fail){
        cout << "Fail to move" << endl;
        return;
    }

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
    // updateMoves();
}

// reset the move from (x, y) to (targx, targy) back to original
void Search::reset(int x, int y, int targx, int targy, int type){
    // cout << "Reset: ";

    bool fail = false;
    // update checkers
    if (type == HUSS){
        if (human.select(targx, targy, false, true)) human.reset(x, y);
        else fail = true;
    } else {
        if (comp.select(targx, targy, false, true)) comp.reset(x, y);
        else fail = true;
    }

    if (fail){
        cout << "Fail to reset" << endl;
        cout << "orig: " << x << " " << y << " targ: " << targx << " " << targy << endl;
        return;
    }

    board.b[targx][targy] = 0;
    // if capture move
    if (abs(targy - y) == 2){

        int capY = y + 1;
        if (y - targy > 0) capY = y - 1;

        if (type == HUSS){
            board.b[x - 1][capY] = COMP;
            comp.resetCaptured(x - 1, capY);
        } else {
            // this is moves for computer
            board.b[x + 1][capY] = HUSS;
            human.resetCaptured(x + 1, capY);
        }
    }
    board.b[x][y] = type;
    updateMoves();
    // cout << "after: " << human << comp;
}


// heuristics to estimate the expected utility
// only for computer
float Search::eval() const {
    // parameters: player type, (board)
    // should be in range [-6, 6]

    // features:
    // [0, 6] number of current player
    int fea1 = fea1 = board.numC,
    // [-30, 0] for each available checker: distance to the other end
        fea2 = -comp.distance(),
    // [0, 30] negative: for each available checker of enemy: distance to the other end
        fea3 = human.distance(),
    // [-6, 0] negative: number of enemy
        fea4 = -board.numH;

    // linear weighted sum of features
    // cout << "Evaluation: " << fea1 << " " << fea2 << " " << fea3 << " " << fea4 << endl;
    float result = float(fea1 + fea2 + fea3 + fea4) / float(36) * 6;
    return result;
}

// utility value for terminal state: range [-6, 6] = numC - numH
float Search::utility() const {
    return board.numC - board.numH;
}

bool Search::terminalState() const {
    return board.terminalState() || (human.avaMoves() == 0 && comp.avaMoves() == 0);
}
