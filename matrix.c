/**
    @file matrix.c
    @author Noah Kirkham, nki59
    @author Hansana Peiris, thp31
    @date 05/10/2025
    @brief LED matrix control
*/


#include "pio.h"
#include "system.h"
#include <util/delay.h>

#define LED_OFF 0
#define LED_ON 1
#define LED_BLINKING 2

/** PIO LEDMAT Rows Array */
static const pio_t matrix_rows[] = {
    LEDMAT_ROW1_PIO,
    LEDMAT_ROW2_PIO,
    LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO,
    LEDMAT_ROW5_PIO,
    LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};

/** PIO LEDMAT Columns Array */
static const pio_t matrix_cols[] = {
    LEDMAT_COL1_PIO,
    LEDMAT_COL2_PIO,
    LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO,
    LEDMAT_COL5_PIO
};

/**
    Initialise LED matrix display.

    @RETURN void
*/
void matrix_init(void)
{
    pio_config_set(LEDMAT_ROW1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW5_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW6_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW7_PIO, PIO_OUTPUT_HIGH);

    pio_config_set(LEDMAT_COL1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL5_PIO, PIO_OUTPUT_HIGH);
}

/**
    Displays the parsed array on the LED matrix display.

    @PARAM size_t width
    @PARAM size_t height
    @PARAM int matrix_array[height][width]
    @PARAM int matrix_interval

    @RETURN void
*/
void display_matrix(size_t width, size_t height, int matrix_array[height][width], int matrix_interval)
{
    for (size_t x = 0; x < width; x++) {

        for (size_t i = 0; i < width; i++) {
            pio_output_high(matrix_cols[i]);
        }

        for (size_t i = 0; i < height; i++) {
            pio_output_high(matrix_rows[i]);
        }

        pio_output_low(matrix_cols[x]);

        for (size_t y = 0; y < height; y++) {
            if (matrix_array[y][x] == LED_ON) {
                pio_output_low(matrix_rows[y]);  // LED ON
            } else if (matrix_array[y][x] >= LED_BLINKING && matrix_interval) {
                pio_output_low(matrix_rows[y]);
            }
        }

        _delay_ms(1);
    }

    for (size_t i = 0; i < width; i++) {
        pio_output_high(matrix_cols[i]);
    }

    for (size_t i = 0; i < height; i++) {
        pio_output_high(matrix_rows[i]);
    }
}
