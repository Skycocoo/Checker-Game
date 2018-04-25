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

GameState::GameState(): order(0), diff(0){
    GLuint font;
    textured = setTextured("font.png", font);
    text = Text(&textured, font);

    GLuint ti;
    textured = setTextured("title.png", ti);
    title = Object(&textured, ti);
    title.setShape(glm::vec3(screenWidth, screenHeight, 0));
    title.setScale(2 * screenHeight);
}


void GameState::setOrder(int o){
    this->order = o;

    if (this->order != 0 && this->diff != 0){
        mode = STATE_GAME_LEVEL;
    }
}

void GameState::setDiff(int d){
    this->diff = d;
    c.setDiff(d);
    if (this->order != 0 && this->diff != 0){

        mode = STATE_GAME_LEVEL;

    }
}




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


void GameState::render(){
    switch (mode){
        case STATE_MAIN_MENU:
            displayMainMenu();
            break;
        case STATE_GAME_LEVEL:
            displayLevel();
            break;
        case STATE_GAME_OVER:
            displayOver();
            break;
    }

}


void GameState::displayMainMenu(){
    title.render();
    text.render("Checker Game", 1, 2, 0, 3.5);

    text.render("press 1: move first", 0.5, 1, 0, 2);
    text.render("press 2: move second", 0.5, 1, 0, 1);
    text.render("press a: easy mode", 0.5, 1, 0, -0.5);
    text.render("press b: medium mode", 0.5, 1, 0, -1.5);
    text.render("press c: hard mode", 0.5, 1, 0, -2.5);

    text.render("press Q to quit", 0.5, 1, 0, -4);
}


void GameState::mouse(uint& button, int mouseX, int mouseY){

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
        humanMove = true;
    } else if (button & SDL_BUTTON(SDL_BUTTON_LEFT)){
        humanMove = c.humanTurn(mouseX, mouseY, done);
        button = 0;
    }

    c.update();
    // display
    glClear(GL_COLOR_BUFFER_BIT);
    c.render();
    SDL_GL_SwapWindow(displayWindow);

    if (humanMove){
        c.computerTurn();

        c.update();
        // display
        glClear(GL_COLOR_BUFFER_BIT);
        c.render();
        SDL_GL_SwapWindow(displayWindow);
    }

    if (c.terminalState()){
        done = true;
    }

    if (done){
        mode = STATE_GAME_OVER;
    }
}

void GameState::displayLevel(){
    c.render();

}

void GameState::displayOver(){
    title.render();
    text.render("Game Over", 1, 2, 0, 1.5);

    // 1: human win; 2: draw; 3: computer win;
    int result = c.determineWinner();
    if (result == 1) text.render("Player wins", 1, 2, 0, 0);
    else if (result == 2) text.render("End with a tie", 1, 2, 0, 0);
    else if (result == 3) text.render("Computer wins", 1, 2, 0, 0);


}
