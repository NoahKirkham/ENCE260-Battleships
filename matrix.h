/**
    @file matrix.h
    @author Noah Kirkham, nki59
    @author Hansana Peiris, thp31
    @date 05/10/2025
    @brief LED matrix control
*/

#ifndef MATRIX_H
#define MATRIX_H

#include "pio.h"
#include "system.h"

#define LED_OFF 0
#define LED_ON 1
#define LED_BLINKING 2

/**
    Initialise LED matrix display.

    @RETURN void
*/
void matrix_init(void);

/**
    Displays the parsed array on the LED matrix display.

    @PARAM size_t width
    @PARAM size_t height
    @PARAM int matrix_array[height][width]
    @PARAM int matrix_interval

    @RETURN void
*/
void display_matrix(size_t width, size_t height, int matrix_array[height][width], int matrix_interval);

#endif // MATRIX_H
