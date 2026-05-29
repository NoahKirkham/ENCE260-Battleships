/**
    @file ir_game.h
    @author Noah Kirkham, nki59
    @author Hansana Peiris, thp31
    @date 09/10/2025
    @brief Game IR control
*/


#ifndef IR_GAME_H
#define IR_GAME_H

#define PLAYER_MOVE 0b01
#define SHOT_REQUEST 0b10
#define SHOT_RESULT 0b11

/**
    Sends the data and type as a single byte of information

    @PARAM int type
    @PARAM int number

    @RETURN void
*/
void ir_send(int type, int number);


/**
    Checks if there is something in the buffer to recieve, 
    false if not, otherwise checks if the type is correct
    then puts the data to the input pointer

    @PARAM int type
    @PARAM int *number

    @RETURN bool
*/
bool ir_receive(int type, int *number);


#endif // IR_GAME_H