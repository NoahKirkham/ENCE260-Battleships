/**
    @file ir_game.c
    @author Noah Kirkham, nki59
    @author Hansana Peiris, thp31
    @date 05/10/2025
    @brief Game IR control
*/


#include "ir_uart.h"
#include "ir_game.h"
#include <stdbool.h>
#include <stdint.h>

#define TYPE_BITMASK 0b11000000
#define VALUE_BITMASK 0b00111111

/**
    Sends the data and type as a single byte of information

    @PARAM int type
    @PARAM int number

    @RETURN void
*/
void ir_send(int type, int number) 
{
    uint8_t data = ((type) << 6) + number;
    ir_uart_putc(data);
}

/**
    Checks if there is something in the buffer to recieve, 
    false if not, otherwise checks if the type is correct
    then puts the data to the input pointer

    @PARAM int type
    @PARAM int *number

    @RETURN bool
*/
bool ir_receive(int type, int *number) 
{
    if (ir_uart_read_ready_p()) {

        uint8_t data = ir_uart_getc();
        uint8_t data_type = (data & TYPE_BITMASK) >> 6;
        uint8_t data_value = data & VALUE_BITMASK;

        switch (type) {
            case PLAYER_MOVE:
                if (data_type == PLAYER_MOVE) {
                    *number = data_value;
                } else {
                    return false;
                }
                break;
            case SHOT_REQUEST:
                if (data_type == SHOT_REQUEST) {
                    *number = data_value;
                } else {
                    return false;
                }
                break;
            case SHOT_RESULT:
                if (data_type == SHOT_RESULT) {
                    *number = data_value;
                } else {
                    return false;
                }
                break;
        }

        return true;
    }

    return false;
}