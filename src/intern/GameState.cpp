// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#include "GameState.h"
#include "setUp.h"


enum GameMode {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};
extern GameMode mode;
extern SDL_Window* displayWindow;
extern float screenWidth;
extern float screenHeight;

extern ShaderProgram textured;

// constructor
GameState::GameState(): order(0), diff(0){
    // load font sprite sheet for text display
    GLuint font;
    textured = setTextured("font.png", font);
    text = Text(&textured, font);

    // load title image for title display
    GLuint ti;
    textured = setTextured("title.png", ti);
    title = Object(&textured, ti);
    // scale the image to match the size of display window
    title.setShape(glm::vec3(screenWidth, screenHeight, 0));
    title.setScale(2 * screenHeight);
}


// set the player to move first / second
void GameState::setOrder(int o){
    this->order = o;

    if (this->order != 0 && this->diff != 0){
        // if both order & difficulty are selected:
        // change the mode from menu level to game level
        mode = STATE_GAME_LEVEL;
    }
}

// set the difficulty
void GameState::setDiff(int d){
    this->diff = d;
    c.setDiff(d);
    if (this->order != 0 && this->diff != 0){
        // if both order & difficulty are selected:
        // change the mode from menu level to game level
        mode = STATE_GAME_LEVEL;

    }
}


// process the mouse input
// in game level
void GameState::mouse(uint& button, int mouseX, int mouseY){
    // if the computer moves first
    if (order == 2){
        c.update();
        c.computerTurn();

        // display
        glClear(GL_COLOR_BUFFER_BIT);
        c.render();
        SDL_GL_SwapWindow(displayWindow);

        order = 0;
    }


    bool done = false;
    bool humanMove = false;
    if (c.humanAva()){
        // if human does not have available move
        // let the computer move
        humanMove = true;
    } else if (button & SDL_BUTTON(SDL_BUTTON_LEFT)){
        // if the mouse is pressed
        // need to process the mouse input
        humanMove = c.humanTurn(mouseX, mouseY, done);
        button = 0;
    }

    c.update();
    // display
    glClear(GL_COLOR_BUFFER_BIT);
    c.render();
    SDL_GL_SwapWindow(displayWindow);

    // if human moves: computer need to move
    if (humanMove){
        // move the computer
        c.computerTurn();

        c.update();
        // display
        glClear(GL_COLOR_BUFFER_BIT);
        c.render();
        SDL_GL_SwapWindow(displayWindow);
    }

    // if there is terinal state after computer moves
    if (c.terminalState()){
        done = true;
    }

    // if the game is end:
    // change mode to end level
    if (done){
        mode = STATE_GAME_OVER;
    }
}


// update the shader to render
void GameState::update(){
    switch (mode){
        case STATE_MAIN_MENU:
            title.update();
            break;
        case STATE_GAME_LEVEL:
            title.update();
            c.update();
            break;
        case STATE_GAME_OVER:
            title.update();
            break;
    }
}

// render thr game
void GameState::render(){
    switch (mode){
        case STATE_MAIN_MENU:
            displayMainMenu();
            break;
        case STATE_GAME_LEVEL:
            // the rendering of game level is taken care of GameState::mouse()
            // displayLevel();
            break;
        case STATE_GAME_OVER:
            displayOver();
            break;
    }
}

// render for menu level
void GameState::displayMainMenu(){
    title.render();
    text.render("Checker Game", 1, 2, 0, 3.5);

    text.render("press 1: move first", 0.5, 1, 0, 2);
    text.render("press 2: move second", 0.5, 1, 0, 1);
    text.render("press a: easy mode", 0.5, 1, 0, -0.5);
    text.render("press b: medium mode", 0.5, 1, 0, -1.5);
    text.render("press c: hard mode (15 seconds)", 0.5, 1, 0, -2.5);

    text.render("press Q to quit", 0.5, 1, 0, -4);
}

// // display
// void GameState::displayLevel(){
//     c.render();
//
// }

// render for end level
void GameState::displayOver(){
    title.render();
    text.render("Game Over", 1, 2, 0, 1.5);

    // 1: human win; 2: draw; 3: computer win;
    int result = c.determineWinner();
    if (result == 1) text.render("Player wins", 1, 2, 0, 0);
    else if (result == 2) text.render("End with a tie", 1, 2, 0, 0);
    else if (result == 3) text.render("Computer wins", 1, 2, 0, 0);


}
