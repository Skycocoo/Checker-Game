#include "../extern/Search.h"

// need to change this later
using namespace std;

bool debug = false;
extern int difficulty;

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
    // cout << "Updating" << endl;
    human.updateMoves();
    comp.updateMoves();
    board.updateCount();
}


// get the only move from comp
Result Search::getMove() const {
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
                    cmove.update(posX, posY, targX, targY);
                    break;
                }
            }

        } else {
            // regular move
            for (size_t j = 0; j < comp.moves[i].regular.size(); j++){
                if (comp.moves[i].regular[j]){
                    int targX = comp.moves[i].regular[j].x, targY = comp.moves[i].regular[j].y;
                    cmove.update(posX, posY, targX, targY);
                    break;
                }
            }
        }
    }
    // cout << "only one move availavle for computer" << cmove;
    return cmove;
}


Result Search::search(const Board& board){
    updateBoard(board);
    updateMoves();

    cout << "Start of searching for computer...\n" << board << human << comp;
    if (comp.avaMoves() > 1) return iterativeDeep();
    else {
        cout << "Only one move available for computer" << endl;
        return getMove();
    }
}

Result Search::iterativeDeep(int maxDepth){
    int depth = maxDepth;
    if (difficulty == 1) depth = 5;
    else if (difficulty == 2) depth = 15;

    Result fmove (-1, -1, -1, -1); // result of alphabeta (first max)
    float util = -std::numeric_limits<float>::max();
    int max = 0, min = 0, node = 0;

    start = std::chrono::system_clock::now();

    Result cmove (-1, -1, -1, -1); // result of each iteration

    if (debug){
        for (int i = 3; i < 4; i++){
            int numMax = 0, numMin = 0, numNode = 0;
            float tempUtil = alphaBeta(cmove, i, numMax, numMin, numNode);
            // if the utility value for the returned move is larger
            // change the current result to the returned move

            // if the duration >= 14: stop searching
            // need to satisfy the duration requirement (within 15 seconds)
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            if (elapsed.count() >= 15) {
                cout << "This search goes to depth " << i-1 << endl;
                break;
            }

            if (tempUtil > -6){
                util = tempUtil;
                fmove.update(cmove);
                max = numMax, min = numMin, node = numNode;
                cout << "Depth: " << i << " utility: " << tempUtil << fmove;
                // cout << "number of max pruning: " << max << " min pruning: " << min << " nodes: " << node << endl;
            }

            // cout << "\tUpdated utility: " << util << fmove;
            // cout << "after reset: " << board << human << comp << endl;

        }
    } else {
        for (int i = 1; i < depth; i++){
            int numMax = 0, numMin = 0, numNode = 0;
            float tempUtil = alphaBeta(cmove, i, numMax, numMin, numNode);
            // if the utility value for the returned move is larger
            // change the current result to the returned move

            // if the duration >= 14: stop searching
            // need to satisfy the duration requirement (within 15 seconds)
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            if (elapsed.count() >= 15) {
                cout << "This search goes to depth " << i-1 << endl;
                break;
            }

            if (tempUtil > -6){
                util = tempUtil;
                fmove.update(cmove);
                max = numMax, min = numMin, node = numNode;
                cout << "Depth: " << i << " utility: " << tempUtil << fmove;
                // cout << "number of max pruning: " << max << " min pruning: " << min << " nodes: " << node << endl;
            }

            // cout << "\tUpdated utility: " << util << fmove;
            // cout << "after reset: " << board << human << comp << endl;


        }
    }

    cout << "\nResult of this search: utility: " << util << "\n\tnumber of max pruning: " << max << " min pruning: " << min << " nodes: " << node << endl << fmove;

    // if comp is doomed to fail
    if (util == -6) return getMove();
    else return fmove;
}

// return the action or the estimated value?
float Search::alphaBeta(Result& fmove, int depth, int& numMax, int& numMin, int& numNode){
    // parameters: depth
    float alpha = -6, beta = 6;
    int curDepth = 0;
    // cout << human;
    // cout << comp;
    return maxVal(alpha, beta, fmove, curDepth, depth, numMax, numMin, numNode);
}

// maxVal: for COMP player
float Search::maxVal(float alpha, float beta, Result& fmove, int curDepth, int depth, int& numMax, int& numMin, int& numNode){
    updateMoves();
    if (debug) cout << endl << "Max; Depth: " << curDepth << endl << board << comp << human;

    ++numNode;

    // edge cases

    // parameters: alpha, beta, depth
    if (terminalState()) return utility();
    // if depth == 0: return evaluated utility
    if (curDepth == depth) {
        float v = eval();
        if (debug) cout << "Max; Depth: " << curDepth << " utility: " << v << endl;
        return v;
    }
    // if times up: evaluated utility
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (elapsed.count() >= 15) return eval();

    // main function

    float v = -6, tempv;
    Result cmove (-1, -1, -1, -1); // result of min

    if (comp.avaMoves() == 0) return minVal(alpha, beta, cmove, curDepth + 1, depth, numMax, numMin, numNode);

    bool capture = comp.avaCapture();
    for (size_t i = 0; i < comp.moves.size(); i++){
        if (!comp.moves[i]) continue;

        int posX = comp.moves[i].cur.x, posY = comp.moves[i].cur.y;

        // capture move
        if (capture){
            for (size_t j = 0; j < comp.moves[i].capture.size(); j++){
                if (comp.moves[i].capture[j]){
                    int targX = comp.moves[i].capture[j].x, targY = comp.moves[i].capture[j].y;
                    int cap = move(i, posX, posY, targX, targY, COMP);
                    // cout << "Move: " << endl << board << comp << human;
                    tempv = minVal(alpha, beta, cmove, curDepth + 1, depth, numMax, numMin, numNode);
                    reset(i, cap, posX, posY, targX, targY, COMP);
                    // cout << "Reset: " << endl << board << comp << human;
                    if (tempv > v){
                        v = tempv;
                        fmove.update(posX, posY, targX, targY);
                    }
                    if (v >= beta){
                        ++numMax;
                        return v;
                    }
                    if (v > alpha) alpha = v;
                }
            }

        } else {
            // regular move
            for (size_t j = 0; j < comp.moves[i].regular.size(); j++){
                if (comp.moves[i].regular[j]){
                    if (debug) cout << comp.moves[i] << endl;

                    int targX = comp.moves[i].regular[j].x, targY = comp.moves[i].regular[j].y;
                    move(i, posX, posY, targX, targY, COMP);
                    tempv = minVal(alpha, beta, cmove, curDepth + 1, depth, numMax, numMin, numNode);
                    reset(i, -1, posX, posY, targX, targY, COMP);
                    if (tempv > v){
                        v = tempv;
                        fmove.update(posX, posY, targX, targY);
                    }
                    if (v >= beta){
                        ++numMax;
                        return v;
                    }
                    if (v > alpha) alpha = v;
                }
            }
        }
    }

    if (debug) cout << "Max; Depth: " << curDepth << " utility: " << v << endl;
    // if (debug && (v == -6 || v == 6)) cout << "moves zero? " << comp.avaMoves() << board << endl;
    return v;
}

// minVal: for HUSS player
float Search::minVal(float alpha, float beta, Result& fmove, int curDepth, int depth, int& numMax, int& numMin, int& numNode){
    updateMoves();
    if (debug) cout << endl << "Min; Depth: " << curDepth << endl << board << human << comp;

    ++numNode;

    // edge cases

    // parameters: alpha, beta, depth
    if (terminalState()) return utility();
    // if depth == 0: return evaluated utility
    if (curDepth == depth) {
        float v = eval();
        if (debug) cout << "Min; Depth: " << curDepth << " utility: " << v << endl;
        return v;
    }
    // if times up: evaluated utility
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (elapsed.count() >= 15) return eval();


    // main function
    // need to update AvaMoves as well

    float v = 6, tempv;
    Result cmove (-1, -1, -1, -1); // result of min

    if (human.avaMoves() == 0) return maxVal(alpha, beta, cmove, curDepth + 1, depth, numMax, numMin, numNode);

    bool capture = human.avaCapture();
    for (size_t i = 0; i < human.moves.size(); i++){
        if (!human.moves[i]) continue;
        int posX = human.moves[i].cur.x, posY = human.moves[i].cur.y;

        // capture move
        if (capture){
            for (size_t j = 0; j < human.moves[i].capture.size(); j++){
                if (human.moves[i].capture[j]){
                    int targX = human.moves[i].capture[j].x, targY = human.moves[i].capture[j].y;
                    int cap = move(i, posX, posY, targX, targY, HUSS);
                    // cout << "Move: " << endl << board;
                    tempv = maxVal(alpha, beta, cmove, curDepth + 1, depth, numMax, numMin, numNode);
                    reset(i, cap, posX, posY, targX, targY, HUSS);
                    // cout << "Reset: " << endl << board;
                    if (tempv < v){
                        v = tempv;
                        fmove.update(posX, posY, targX, targY);
                    }
                    if (v <= alpha){
                        ++numMin;
                        return v;
                    }
                    if (v < beta) beta = v;
                }
            }

        } else {
            // regular move
            for (size_t j = 0; j < human.moves[i].regular.size(); j++){
                if (human.moves[i].regular[j]){
                    if (debug) cout << human.moves[i] << endl;

                    int targX = human.moves[i].regular[j].x, targY = human.moves[i].regular[j].y;
                    move(i, posX, posY, targX, targY, HUSS);
                    tempv = maxVal(alpha, beta, cmove, curDepth + 1, depth, numMax, numMin, numNode);
                    reset(i, -1, posX, posY, targX, targY, HUSS);

                    if (tempv < v){
                        v = tempv;
                        fmove.update(posX, posY, targX, targY);
                    }
                    if (v <= alpha){
                        ++numMin;
                        return v;
                    }
                    if (v < beta) beta = v;
                }
            }
        }
    }
    if (debug) cout << "Min; Depth: " << curDepth << " utility: " << v << endl;
    // if (debug && (v == -6 || v == 6)) cout << human.avaMoves() << endl;
    return v;
}

void Search::update(int x, int y, int targX, int targY, int type){
    // update checkers
    if (type == HUSS) {
        human.select(x, y, debug);
        // cout << "after select" << endl;
        human.checkMove(targX, targY);
        // cout << "after check" << endl;
    }
    else {
        comp.select(x, y, debug);
        comp.checkMove(targX, targY);
    }

    // move a checker forom (x, y) to (targX, targY)
    board.b[x][y] = 0;

    // if capture move
    if (abs(targY - y) == 2){
        int capY = y + 1;
        if (y - targY > 0) capY = y - 1;

        if (type == HUSS){
            // if (y - targY > 0) board.b[x - 1][y - 1] = 0;
            // else board.b[x - 1][y + 1] = 0;
            if (debug) cout << x - 1 << " " << capY << " captured" << endl;
            board.b[x - 1][capY] = 0;
            comp.captured(x - 1, capY);
        } else {
            // this is moves for computer
            // if (y - targY > 0) board.b[x + 1][y - 1] = 0;
            // else board.b[x + 1][y + 1] = 0;
            board.b[x + 1][capY] = 0;
            human.captured(x + 1, capY);
        }
    }
    board.b[targX][targY] = type;
    // board.updateCount();
    updateMoves();
}

// move from (x, y) to (targX, targY)
// should also take care of the checkers
int Search::move(int index, int x, int y, int targX, int targY, int type){
    if (debug) cout << "from " << x << " " << y << " to " << targX << " " << targY << endl;

    if (type == HUSS) human.superMove(index, targX, targY);
    else comp.superMove(index, targX, targY);

    int cap = -1;

    // bool fail = false;
    //
    // // update checkers
    // if (type == HUSS) {
    //     human.superMove(index, targX, targY);
    //     // if (!(human.select(x, y, debug) && human.checkMove(targX, targY))) fail = true;
    // }
    // else {
    //     comp.superMove(index, targX, targY);
    //     // if (!(comp.select(x, y, debug) && comp.checkMove(targX, targY))) fail = true;
    // }
    //
    // if (fail){
    //     cout << "Fail to move" << endl;
    //     return;
    // }

    // move a checker forom (x, y) to (targX, targY)
    board.b[x][y] = 0;

    // if capture move
    if (abs(targY - y) == 2){
        int capY = y + 1;
        if (y - targY > 0) capY = y - 1;

        if (type == HUSS){
            // if (y - targY > 0) board.b[x - 1][y - 1] = 0;
            // else board.b[x - 1][y + 1] = 0;
            board.b[x - 1][capY] = 0;
            cap = comp.captured(x - 1, capY);
        } else {
            // this is moves for computer
            // if (y - targY > 0) board.b[x + 1][y - 1] = 0;
            // else board.b[x + 1][y + 1] = 0;
            board.b[x + 1][capY] = 0;
            cap = human.captured(x + 1, capY);
        }
    }
    board.b[targX][targY] = type;
    board.updateCount();
    // updateMoves();

    return cap;
}

// reset the move from (x, y) to (targX, targY) back to original
void Search::reset(int index, int indexCap, int x, int y, int targX, int targY, int type){
    // if (debug) cout << "reset: orig " << x << " " << y << " to " << targX << " " << targY << endl;

    if (type == HUSS) {
        // if (debug) cout << human.moves[index];
        human.reset(index, x, y);
    }
    else {
        // if (debug) cout << comp.moves[index];
        comp.reset(index, x, y);
    }

    board.b[targX][targY] = 0;
    // if capture move
    if (abs(targY - y) == 2){

        int capY = y + 1;
        if (y - targY > 0) capY = y - 1;

        if (type == HUSS){
            board.b[x - 1][capY] = COMP;
            // comp.resetCaptured(x - 1, capY);
            comp.resetCaptured(indexCap);
        } else {
            // this is moves for computer
            board.b[x + 1][capY] = HUSS;
            // human.resetCaptured(x + 1, capY);
            human.resetCaptured(indexCap);
        }
    }
    board.b[x][y] = type;
    updateMoves();
    // if (debug) cout << "after: " << human << comp;
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

// utility value for terminal state: range [-6, 6]: -6: comp loses, 6: comp wins
float Search::utility() const {
    // if one of them wins
    if (board.terminalState()){
        if (board.numC > 0) return 6;
        else return -6;
    } else {
        // if there is no legal move available for both of them
        return board.numC - board.numH;
    }
    // cout << "terminal? " << boolalpha << board.terminalState() << board << endl;

    // return board.numC - board.numH;
}

bool Search::terminalState() const {
    return board.terminalState() || (human.avaMoves() == 0 && comp.avaMoves() == 0);
}
