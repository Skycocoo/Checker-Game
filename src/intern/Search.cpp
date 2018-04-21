// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#include "../extern/Search.h"

// difficulty of the game -> complexity of the search
extern int difficulty;

// constructor
Search::Search(const AvaMoves& human, const AvaMoves& comp, const Board& board):
human(human), comp(comp), board(board){
    this->human.updateBoard(this->board);
    this->comp.updateBoard(this->board);
}

// update availability for both human and computer
void Search::updateMoves(){
    human.updateMoves();
    comp.updateMoves();
}


// get the only move for computer
Result Search::getMove() const {
    bool capture = comp.avaCapture();

    for (size_t i = 0; i < comp.moves.size(); i++){
        if (!comp.moves[i]) continue;

        int posX = comp.moves[i].cur.x, posY = comp.moves[i].cur.y;
        // if the move is capture move
        if (capture){
            for (size_t j = 0; j < comp.moves[i].capture.size(); j++){
                if (comp.moves[i].capture[j]){
                    int targX = comp.moves[i].capture[j].x, targY = comp.moves[i].capture[j].y;
                    return Result(posX, posY, targX, targY);
                }
            }
        } else {
            // if the move is regular move
            for (size_t j = 0; j < comp.moves[i].regular.size(); j++){
                if (comp.moves[i].regular[j]){
                    int targX = comp.moves[i].regular[j].x, targY = comp.moves[i].regular[j].y;
                    return Result(posX, posY, targX, targY);
                }
            }
        }
    }

    return Result(-1, -1, -1, -1);
}

// start the search for this turn
Result Search::search(const Board& board){
    // update availability for both human and computer
    updateMoves();

    // std::cout << "Start of searching for computer...\n" << board << human << comp;
    std::cout << "Start of searching for computer...\n" << board;
    // if more than one move is available
    if (comp.avaMoves() > 1) return iterativeDeep();
    else {
        // if only one move is available for computer
        std::cout << "Only one move available for computer" << std::endl;
        return getMove();
    }
}

// use iterative deepening search to set timed cutoff
Result Search::iterativeDeep(int maxDepth){
    // set up max depth based on the difficulty
    int depth = maxDepth;
    if (difficulty == 1) depth = 5;
    else if (difficulty == 2) depth = 15;

    // result of the search
    Result fmove (-1, -1, -1, -1);
    // result of each iteration
    Result cmove (-1, -1, -1, -1);

    // initialize utility to be the minimum possible value
    float util = -6;
    // initialize numbers
    int max = 0, min = 0, node = 0;

    // record starting time of search
    start = std::chrono::system_clock::now();

    for (int i = 1; i < depth; i++){
        int numMax = 0, numMin = 0, numNode = 0;
        // retrieve utility for each alpha-beta search
        float tempUtil = alphaBeta(cmove, i, numMax, numMin, numNode);

        // if the duration >= 15: stop searching
        // need to satisfy the duration requirement (within 15 seconds)
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        if (elapsed.count() >= 15) {
            std::cout << "This search goes to depth " << i-1 << std::endl;
            break;
        }

        // if the result is not failure
        // record the optimal result as far as it can
        if (tempUtil > -6){
            util = tempUtil;
            fmove.update(cmove);
            max = numMax, min = numMin, node = numNode;
            // std::cout << "Depth: " << i << " utility: " << tempUtil << fmove;
        }
    }

    std::cout << "Result of this search:\n\tutility: " << util << " number of max pruning: " << max << " min pruning: " << min << " nodes: " << node << std::endl << fmove;

    // if comp is doomed to fail: choose random move
    if (util == -6) return getMove();
    else return fmove;
}

// alpha-beta search based on cutoff depth
// update result for each iteration
// update number of pruning & nodes
// return: float: estimated utility
float Search::alphaBeta(Result& fmove, int depth, int& numMax, int& numMin, int& numNode){
    float alpha = -6, beta = 6;
    int curDepth = 0;
    return maxVal(alpha, beta, fmove, curDepth, depth, numMax, numMin, numNode);
}

// maxVal: for COMP player
float Search::maxVal(float alpha, float beta, Result& fmove, int curDepth, int depth, int& numMax, int& numMin, int& numNode){
    // update availability for both human and computer
    updateMoves();

    // update number of nodes generated
    ++numNode;

    // edge cases
    // if terminal state: return corresponding utility
    if (terminalState()) return utility();
    // if depth == 0: return evaluated utility
    if (curDepth == depth) return eval();
    // if times up: evaluated utility
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (elapsed.count() >= 15) return eval();


    // main function
    // initialize temporary utility
    float v = -6, tempv;
    // result of min
    Result cmove (-1, -1, -1, -1);

    // if there is no available move: do nothing
    if (comp.avaMoves() == 0) return minVal(alpha, beta, cmove, curDepth + 1, depth, numMax, numMin, numNode);

    // loop through possible moves by index
    bool capture = comp.avaCapture();
    for (size_t i = 0; i < comp.moves.size(); i++){
        // ignore captured moves by enemy
        if (!comp.moves[i]) continue;

        // record position of current checker
        int posX = comp.moves[i].cur.x, posY = comp.moves[i].cur.y;

        // if there is capture move
        if (capture){
            for (size_t j = 0; j < comp.moves[i].capture.size(); j++){
                if (comp.moves[i].capture[j]){
                    // record target position
                    int targX = comp.moves[i].capture[j].x, targY = comp.moves[i].capture[j].y;
                    // execute the move
                    // record possible captured position for reset
                    int cap = move(i, posX, posY, targX, targY, COMP);
                    // recurse into the human move
                    tempv = minVal(alpha, beta, cmove, curDepth + 1, depth, numMax, numMin, numNode);
                    // reset the move for another iteration
                    reset(i, cap, posX, posY, targX, targY, COMP);

                    // if there is better utility
                    if (tempv > v){
                        v = tempv;
                        fmove.update(posX, posY, targX, targY);
                    }
                    // if the move can be pruned
                    if (v >= beta){
                        ++numMax;
                        return v;
                    }
                    // if alpha can be updated
                    if (v > alpha) alpha = v;
                }
            }

        } else {
            // if there is regular move
            for (size_t j = 0; j < comp.moves[i].regular.size(); j++){
                if (comp.moves[i].regular[j]){
                    // record target position
                    int targX = comp.moves[i].regular[j].x, targY = comp.moves[i].regular[j].y;
                    // execute the move
                    move(i, posX, posY, targX, targY, COMP);
                    // recurse into the human move
                    tempv = minVal(alpha, beta, cmove, curDepth + 1, depth, numMax, numMin, numNode);
                    // reset the move for another iteration
                    reset(i, -1, posX, posY, targX, targY, COMP);

                    // if there is better utility
                    if (tempv > v){
                        v = tempv;
                        fmove.update(posX, posY, targX, targY);
                    }
                    // if the move can be pruned
                    if (v >= beta){
                        ++numMax;
                        return v;
                    }
                    // if alpha can be updated
                    if (v > alpha) alpha = v;
                }
            }
        }
    }
    // return the utility value for computer
    return v;
}

// minVal: for HUSS player
float Search::minVal(float alpha, float beta, Result& fmove, int curDepth, int depth, int& numMax, int& numMin, int& numNode){
    // update availability for both human and computer
    updateMoves();

    // update number of nodes generated
    ++numNode;

    // edge cases
    // if terminal state: return corresponding utility
    if (terminalState()) return utility();
    // if depth == 0: return evaluated utility
    if (curDepth == depth) return eval();
    // if times up: evaluated utility
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (elapsed.count() >= 15) return eval();


    // main function
    // initialize temporary utility
    float v = 6, tempv;
    // result of max
    Result cmove (-1, -1, -1, -1);

    // if there is no available move: do nothing
    if (human.avaMoves() == 0) return maxVal(alpha, beta, cmove, curDepth + 1, depth, numMax, numMin, numNode);

    // loop through possible moves by index
    bool capture = human.avaCapture();
    for (size_t i = 0; i < human.moves.size(); i++){
        // ignore captured moves by enemy
        if (!human.moves[i]) continue;

        // record position of current checker
        int posX = human.moves[i].cur.x, posY = human.moves[i].cur.y;

        // if there is capture move
        if (capture){
            for (size_t j = 0; j < human.moves[i].capture.size(); j++){
                if (human.moves[i].capture[j]){
                    // record target position
                    int targX = human.moves[i].capture[j].x, targY = human.moves[i].capture[j].y;
                    // execute the move
                    // record possible captured position for reset
                    int cap = move(i, posX, posY, targX, targY, HUSS);
                    // recurse into the computer move
                    tempv = maxVal(alpha, beta, cmove, curDepth + 1, depth, numMax, numMin, numNode);
                    // reset the move for another iteration
                    reset(i, cap, posX, posY, targX, targY, HUSS);

                    // if there is better utility for min
                    if (tempv < v){
                        v = tempv;
                        fmove.update(posX, posY, targX, targY);
                    }
                    // if the move can be pruned
                    if (v <= alpha){
                        ++numMin;
                        return v;
                    }
                    // if beta can be updated
                    if (v < beta) beta = v;
                }
            }

        } else {
            // if there is regular move
            for (size_t j = 0; j < human.moves[i].regular.size(); j++){
                if (human.moves[i].regular[j]){
                    // record target position
                    int targX = human.moves[i].regular[j].x, targY = human.moves[i].regular[j].y;
                    // execute the move
                    move(i, posX, posY, targX, targY, HUSS);
                    // recurse into the computer move
                    tempv = maxVal(alpha, beta, cmove, curDepth + 1, depth, numMax, numMin, numNode);
                    // reset the move for another iteration
                    reset(i, -1, posX, posY, targX, targY, HUSS);

                    // if there is better utility for min
                    if (tempv < v){
                        v = tempv;
                        fmove.update(posX, posY, targX, targY);
                    }
                    // if the move can be pruned
                    if (v <= alpha){
                        ++numMin;
                        return v;
                    }
                    // if beta can be updated
                    if (v < beta) beta = v;
                }
            }
        }
    }
    // return the utility value for computer
    return v;
}

// update moves from the actual game
void Search::update(int x, int y, int targX, int targY, int type){
    // update checkers
    if (type == HUSS) {
        human.select(x, y, false);
        human.checkMove(targX, targY);
    } else {
        comp.select(x, y, false);
        comp.checkMove(targX, targY);
    }

    // move a checker forom (x, y) to (targX, targY)
    board.b[x][y] = 0;

    // if capture move
    if (abs(targY - y) == 2){
        int capY = y + 1;
        if (y - targY > 0) capY = y - 1;

        if (type == HUSS){
            board.b[x - 1][capY] = 0;
            comp.captured(x - 1, capY);
            board.numC -= 1;
        } else {
            board.b[x + 1][capY] = 0;
            human.captured(x + 1, capY);
            board.numH -= 1;
        }
    }
    board.b[targX][targY] = type;
    updateMoves();
}

// move from (x, y) to (targX, targY), based on index
// return: int: the possible position of captured checker index
int Search::move(int index, int x, int y, int targX, int targY, int type){
    if (type == HUSS) human.superMove(index, targX, targY);
    else comp.superMove(index, targX, targY);

    int cap = -1;

    // move a checker forom (x, y) to (targX, targY)
    board.b[x][y] = 0;

    // if capture move
    if (abs(targY - y) == 2){
        int capY = y + 1;
        if (y - targY > 0) capY = y - 1;

        if (type == HUSS){
            board.b[x - 1][capY] = 0;
            cap = comp.captured(x - 1, capY);
            board.numC -= 1;
        } else {
            board.b[x + 1][capY] = 0;
            cap = human.captured(x + 1, capY);
            board.numH -= 1;
        }
    }
    board.b[targX][targY] = type;
    return cap;
}

// reset the move from (x, y) to (targX, targY) back to original, based on index
void Search::reset(int index, int indexCap, int x, int y, int targX, int targY, int type){
    if (type == HUSS) human.reset(index, x, y);
    else comp.reset(index, x, y);

    board.b[targX][targY] = 0;

    // if capture move
    if (abs(targY - y) == 2){
        int capY = y + 1;
        if (y - targY > 0) capY = y - 1;

        if (type == HUSS){
            board.b[x - 1][capY] = COMP;
            comp.resetCaptured(indexCap);
            board.numC += 1;
        } else {
            board.b[x + 1][capY] = HUSS;
            human.resetCaptured(indexCap);
            board.numH += 1;
        }
    }
    board.b[x][y] = type;
    updateMoves();
}


// heuristics to estimate the expected utility
// only for computer
float Search::eval() const {
    // parameters: player type, (board)
    // should be in range [-6, 6]

    // features:
    // [0, 6] number of computer player
    int fea1 = fea1 = board.numC,
    // [-30, 0] for each available checker: distance to the other end
        fea2 = -comp.distance(),
    // [0, 30] negative: for each available checker of enemy: distance to the other end
        fea3 = human.distance(),
    // [-6, 0] negative: number of enemy
        fea4 = -board.numH;

    // linear weighted sum of features
    float result = float(fea1 + fea2 + fea3 + fea4) / float(36) * 6;
    return result;
}

// utility value for terminal state: range [-6, 6]: -6: comp loses, 6: comp wins
float Search::utility() const {
    // if one of them wins
    if (board.terminalState()){
        if (board.numC > 0) return 6;
        else return -6;
    }
    // if there is no legal move available for both of them
    else return board.numC - board.numH;
}

// check the terminal state for the game
// terminal if one player does not have any checker
// or there is no legal move for both player
// return: bool
bool Search::terminalState() const {
    return board.terminalState() || (human.avaMoves() == 0 && comp.avaMoves() == 0);
}
