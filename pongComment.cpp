#include <emscripten.h>
#include <emscripten/bind.h>

enum Move {
    STATIONARY = 0,
    UP = 1,
    DOWN = 2
};

/* 
 Function we want to export
 return int
 takes float and int (takes the bll xspeed, ySpeed , xpositon, yposition and paddle yposition)
*/

int getAIMove(float ball_xspeed, float ball_yspeed,
              float ball_xpos, float ball_ypos,
              int paddle_ypos) {
    int idealPosition = ball_ypos;

    /* Track the ball ypostion as long as the ball is 
        moving away from the panel. But if the ball moves
        toward the paddle , the ball xspeed is negative, it
        figues out the number of turns until the ball will 
        hit the paddle then it calculates the ideal position 
        disregarding any bounces that might happen. 
    */
    if(ball_xspeed <= 0){
        auto turns = (ball_xpos - 50) / (-1 * ball_xspeed);
        idealPosition = ball_ypos + (ball_yspeed * turns);
    }

    /* 
    if the ideal
    position is greater than the paddles yposition we want to 
    move down. If the ideal position is less than the paddles
    yposition we want to move up. 
    */
    if(idealPosition > paddle_ypos){
        return Move::DOWN;
    }
    if(idealPosition < paddle_ypos){
        return Move::UP;
    }
    return Move::STATIONARY;
}

/* How to get getAIMove function available to JS through wasm?
emscripten/bind.h header provides an emscripten bindings macro. This
lets us advertise what functions you want available to Js: here we are
saying we want emscripten function named getAIMove and pass at the 
address the function we have created getAIMove
*/
EMSCRIPTEN_BINDINGS(pong) {
    emscripten::function("getAIMove", &getAIMove);
}