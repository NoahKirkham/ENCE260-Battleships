/**
    @file battleship.c
    @author Noah Kirkham, nki59
    @author Hansana Peiris, thp31
    @date 05/10/2025
    @brief Ship control and board management
*/


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "battleship.h"

#define LED_OFF 0
#define LED_ON 1
#define LED_BLINKING 2

#define SHIP_VERTICAL 0
#define SHIP_HORIZONTAL 1

/**
    Initializes And Places A New Ship On The Game Board.

    @PARAM int ships_to_allocate
    @PARAM current_ship_s *current_ship
    @PARAM size_t width
    @PARAM size_t height
    @PARAM int matrix_array[height][width]

    @RETURN void
*/
void ship_init(int ships_to_allocate, current_ship_s *current_ship, size_t width, size_t height, int matrix_array[height][width])
{
    int ship_length = ships_to_allocate + 1; // Length is detemined as ships to allocate + 1
    bool placement_avaliable; // Bool for checking placement

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (y + ship_length > height) {
                continue;
            }

            placement_avaliable = true;

            for (size_t ship_y = y; ship_y < ship_length + y; ship_y++) {
                if (matrix_array[ship_y][x]) {
                    placement_avaliable = false;
                    break;
                }
            }

            if (placement_avaliable) {
                current_ship->x = x;
                current_ship->y = y;
                current_ship->rotation = SHIP_VERTICAL;
                for (size_t ship_y = y; ship_y < ship_length + y; ship_y++) {
                    matrix_array[ship_y][x] = LED_BLINKING;
                }
                return; // Ship placed successfully
            }
        }
    }
}

/**
    Checks if there is a position available for 
    the ship to move into

    @PARAM int x
    @PARAM int y
    @PARAM int ships_to_allocate
    @PARAM int rotation
    @PARAM size_t width
    @PARAM size_t height
    @PARAM int matrix_array[height][width]

    @RETURN bool
*/
bool is_valid_ship_position(int x, int y, int ships_to_allocate, int rotation, size_t width, size_t height, int matrix_array[height][width])
{
    int ship_length = ships_to_allocate + 1;

    size_t check_x = x;
    size_t check_y = y;

    for (int i = 0; i < ship_length; i++) {

        if (rotation == SHIP_HORIZONTAL) {
            check_x = x + i;
        } else if (rotation == SHIP_VERTICAL) {
            check_y = y + i;
        }

        if (check_x >= width || check_y >= height || matrix_array[check_y][check_x] == LED_ON) {
            return false;
        }
    }
    return true;
}

/**
    Rotates the current ship between horizontal and vertical
    orientations if space allows.

    @PARAM int ships_to_allocate
    @PARAM current_ship_s *current_ship
    @PARAM size_t width
    @PARAM size_t height
    @PARAM int matrix_array[height][width]

    @RETURN void
*/
void rotate_ship(int ships_to_allocate, current_ship_s *current_ship, size_t width, size_t height, int matrix_array[height][width])
{
    int ship_length = ships_to_allocate + 1;
    int placement_avaliable;
    if (current_ship->rotation == SHIP_HORIZONTAL) { // Current Ship Is Horizontal

        if ((size_t)(current_ship->y + ship_length) > height) {
            return;
        }

        placement_avaliable = true;

        for (size_t ship_y = current_ship->y + 1; ship_y < (size_t)(ship_length + current_ship->y); ship_y++) {
            if (matrix_array[ship_y][current_ship->x]) {
                placement_avaliable = false;
                break;
            }
        }

        if (placement_avaliable) {
            current_ship->rotation = SHIP_VERTICAL;
            for (size_t ship_x = current_ship->x; ship_x < (size_t)(current_ship->x + ship_length); ship_x++) {
                matrix_array[current_ship->y][ship_x] = LED_OFF;
            }
            for (size_t ship_y = current_ship->y; ship_y < (size_t)(ship_length + current_ship->y); ship_y++) {
                matrix_array[ship_y][current_ship->x] = LED_BLINKING;
            }
            return;
        }
    } else { // Current Ship Is Vertical
        if ((size_t)(current_ship->x + ship_length) > width) {
            return;
        }

        placement_avaliable = true;

        for (size_t ship_x = current_ship->x + 1; ship_x < (size_t)(current_ship->x + ship_length); ship_x++)
        {
            if (matrix_array[current_ship->y][ship_x]) {
                placement_avaliable = false;
                break;
            }
        }

        if (placement_avaliable) {
            current_ship->rotation = SHIP_HORIZONTAL;
            for (size_t ship_y = current_ship->y; ship_y < (size_t)(current_ship->y + ship_length); ship_y++) {
                matrix_array[ship_y][current_ship->x] = LED_OFF;
            }
            for (size_t ship_x = current_ship->x; ship_x < (size_t)(ship_length + current_ship->x); ship_x++) {
                matrix_array[current_ship->y][ship_x] = LED_BLINKING;
            }
            return;
        }
    }
}

/**
    Updates The Position Of The Currently Selected Ship On
    The 7x5 Game Matrix. It First Checks Whether The Ship Can
    Move In The Given Direction Without Exceeding The Grid
    Boundaries Or Colliding With Another Ship. If The Move Is
    Valid, Tt Updates Both The Ship's Stored Coordinates And
    The Matrix Representation.

    @PARAM int ships_to_allocate
    @PARAM current_ship_s *current_ship
    @PARAM size_t width
    @PARAM size_t height
    @PARAM int matrix_array[height][width]
    @PARAM movement_direction_e direction

    @RETURN void
*/
void move_ship(int ships_to_allocate, current_ship_s *current_ship, int width, int height, int matrix_array[height][width], movement_direction_e direction)
{
    int ship_length = ships_to_allocate + 1;
    int8_t pos_y = current_ship->y;
    int8_t pos_x = current_ship->x;

    switch (direction) {

        case UP: // Move Up
            if (current_ship->rotation == SHIP_HORIZONTAL) { 
                for (int step = 1; pos_y - step >= 0; step++) {
                    if (is_valid_ship_position(pos_x, pos_y - step, ships_to_allocate, current_ship->rotation, width, height, matrix_array)) {
                        
                        // Clear old
                        for (size_t ship_x = pos_x; ship_x < (size_t)(pos_x + ship_length); ship_x++) {
                            matrix_array[pos_y][ship_x] = LED_OFF;
                        }

                        // Update position
                        current_ship->y = pos_y - step;

                        // Draw new ship (row)
                        for (size_t ship_x = current_ship->x; ship_x < (size_t)(current_ship->x + ship_length); ship_x++) {
                            matrix_array[current_ship->y][ship_x] = LED_BLINKING;
                        }
                        break;
                    }
                }
            } else { // Current Ship Is Vertical
                for (int step = 1; pos_y - step >= 0; step++) {
                    if (is_valid_ship_position(pos_x, pos_y - step, ships_to_allocate, current_ship->rotation, width, height, matrix_array)) {
                        // Clear old 
                        for (size_t ship_y = pos_y; ship_y < (size_t)(pos_y + ship_length); ship_y++) {
                            matrix_array[ship_y][pos_x] = LED_OFF;
                        }

                        // Update ship position
                        current_ship->y = pos_y - step;

                        // Draw new ship (col)
                        for (size_t ship_y = current_ship->y; ship_y < (size_t)(current_ship->y + ship_length); ship_y++) {
                            matrix_array[ship_y][pos_x] = LED_BLINKING;
                        }
                        break;
                    }
                }
            }
            break;


        case DOWN: // Move Down

            if (current_ship->rotation == SHIP_HORIZONTAL) { 
                for (int step = 1; pos_y + step < height; step++) {
                    if (is_valid_ship_position(pos_x, pos_y + step, ships_to_allocate, current_ship->rotation, width, height, matrix_array)) {
                        
                        // Clearing old
                        for (size_t ship_x = pos_x; ship_x < (size_t)(pos_x + ship_length); ship_x++) {
                            matrix_array[pos_y][ship_x] = LED_OFF;
                        }
                        
                        // Updating ship positon
                        current_ship->y = pos_y + step;

                        // Draw new ship (row)
                        for (size_t ship_x = pos_x; ship_x < (size_t)(pos_x + ship_length); ship_x++) {
                            matrix_array[current_ship->y][ship_x] = LED_BLINKING;
                        }
                        break;
                    }
                }
            } else { // Current Ship Is Vertical
                for (int step = 1; pos_y + step + ship_length - 1 < height; step++) {
                    if (is_valid_ship_position(pos_x, pos_y + step, ships_to_allocate, current_ship->rotation, width, height, matrix_array)) {
                        // Clear old
                        for (size_t ship_y = pos_y; ship_y < (size_t)(pos_y + ship_length); ship_y++) {
                            matrix_array[ship_y][pos_x] = LED_OFF;
                        }

                        // Update ship position
                        current_ship->y = pos_y + step;

                        // Draw new ship (col)
                        for (size_t ship_y = current_ship->y; ship_y < (size_t)(current_ship->y + ship_length); ship_y++) {
                            matrix_array[ship_y][pos_x] = LED_BLINKING;
                        }
                        break;
                    }
                }
            }
            break;

        case LEFT: // Move Left
            if (current_ship->rotation == SHIP_HORIZONTAL) { 
                for (int step = 1; pos_x - step >= 0; step++) {
                    if (is_valid_ship_position(pos_x - step, pos_y, ships_to_allocate, current_ship->rotation, width, height, matrix_array)) {
                        
                        // Clear old
                        for (size_t ship_x = pos_x; ship_x < (size_t)(pos_x + ship_length); ship_x++) {
                            matrix_array[pos_y][ship_x] = LED_OFF;
                        }

                        // Update ship position
                        current_ship->x = pos_x - step;

                        // Draw new ship (row)
                        for (size_t ship_x = current_ship->x; ship_x < (size_t)(current_ship->x + ship_length); ship_x++) {
                            matrix_array[pos_y][ship_x] = LED_BLINKING;
                        }
                        break;      
                    }
                }
            } else { // Current Ship Is Vertical
                for (int step = 1; pos_x - step >= 0; step++) {
                    if (is_valid_ship_position(pos_x - step, pos_y, ships_to_allocate, current_ship->rotation, width, height, matrix_array)) {
                        
                        // Clear old
                        for (size_t ship_y = pos_y; ship_y < (size_t)(pos_y + ship_length); ship_y++) {
                            matrix_array[ship_y][pos_x] = LED_OFF;
                        }

                        // Update ship position
                        current_ship->x = pos_x - step;

                        // Draw new ship (col)
                        for (size_t ship_y = pos_y; ship_y < (size_t)(pos_y + ship_length); ship_y++) {
                            matrix_array[ship_y][current_ship->x] = LED_BLINKING;
                        }
                        break;
                    }
                }
            }
            break;


        case RIGHT: // Move Right
            if (current_ship->rotation == SHIP_HORIZONTAL) { 
                for (int step = 1; pos_x + step + ship_length - 1 < width; step++) {
                    if (is_valid_ship_position(pos_x + step, pos_y, ships_to_allocate, current_ship->rotation, width, height, matrix_array)) {
                        
                        // Clear old 
                        for (size_t ship_x = pos_x; ship_x < (size_t)(pos_x + ship_length); ship_x++) {
                            matrix_array[pos_y][ship_x] = LED_OFF;
                        }
                        // Update ship position
                        current_ship->x = pos_x + step;

                        // Draw new ship (row)
                        for (size_t ship_x = current_ship->x; ship_x < (size_t)(current_ship->x + ship_length); ship_x++) {
                            matrix_array[pos_y][ship_x] = LED_BLINKING;
                        }
                        break;
                    }
                }
            } else { // Current Ship Is Vertical
                for (int step = 1; pos_x + step < width; step++) {
                    if (is_valid_ship_position(pos_x + step, pos_y, ships_to_allocate, current_ship->rotation, width, height, matrix_array)) {
                        
                        // Clear old 
                        for (size_t ship_y = pos_y; ship_y < (size_t)(pos_y + ship_length); ship_y++) {
                            matrix_array[ship_y][pos_x] = LED_OFF;
                        }
                        // Update ship position
                        current_ship->x = pos_x + step;

                        // Draw new ship (col)
                        for (size_t ship_y = pos_y; ship_y < (size_t)(pos_y + ship_length); ship_y++) {
                            matrix_array[ship_y][current_ship->x] = LED_BLINKING;
                        }
                        break;
                    }
                }
            }
            break;

    }
}


/**
    Converts All Pixels That Are Current State Initial
    To The Final State.

    @PARAM int initial
    @PARAM int final
    @PARAM size_t width
    @PARAM size_t height
    @PARAM int matrix_array[height][width]

    @RETURN void
*/
void board_state_change(int initial, int final, size_t width, size_t height, int matrix_array[height][width])
{
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (matrix_array[y][x] == initial) {
                matrix_array[y][x] = final;
            }
        }
    }
}

/**
    Initialises The Target Selector On The First Free Pixel
    (A Pixel That Hasn't Been Shot Thus Far)

    @PARAM size_t width
    @PARAM size_t height
    @PARAM int attack_array[width][height]
    @PARAM target_selector_s *target_selector

    @RETURN void
*/
void init_target_selector(size_t width, size_t height, int attack_array[height][width], target_selector_s *target_selector)
{
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (!attack_array[y][x]) {
                attack_array[y][x] = LED_BLINKING;
                target_selector->x = x;
                target_selector->y = y;
                return;
            }
        }
    }
}

/**
    Moves The Target Selector To The First Free Pixel In The
    Chosen Direction (A Pixel That Hasn't Been Shot Thus Far)

    @PARAM size_t width
    @PARAM size_t height
    @PARAM int attack_array[width][height]
    @PARAM target_selector_s *target_selector
    @PARAM movement_direction_e direction

    @RETURN void
*/
void move_target_selector(size_t width, size_t height, int attack_array[height][width], target_selector_s *target_selector, movement_direction_e direction)
{
    switch (direction) {

        case UP:
            for (int y = target_selector->y - 1; y >= 0; y--) {
                int x = target_selector->x;
                size_t rowCounter = 0;
                do {
                    if (!attack_array[y][x - rowCounter] && (int)(x - rowCounter) >= 0) {
                        attack_array[target_selector->y][target_selector->x] = LED_OFF;
                        target_selector->x = x - rowCounter;
                        target_selector->y = y;
                        attack_array[target_selector->y][target_selector->x] = LED_BLINKING;
                        return;
                    } else if (!attack_array[y][x + rowCounter] && (size_t)(x + rowCounter) < width) {
                        attack_array[target_selector->y][target_selector->x] = LED_OFF;
                        target_selector->x = x + rowCounter;
                        target_selector->y = y;
                        attack_array[target_selector->y][target_selector->x] = LED_BLINKING;
                        return;
                    }
                    rowCounter++;
                } while (attack_array[y][x] && rowCounter < width);
            }

            break;

        case DOWN:
            for (size_t y = target_selector->y + 1; y < height; y++) {
                int x = target_selector->x;
                size_t rowCounter = 0;
                do {
                    if (!attack_array[y][x - rowCounter] && (int)(x - rowCounter) >= 0) {
                        attack_array[target_selector->y][target_selector->x] = LED_OFF;
                        target_selector->x = x - rowCounter;
                        target_selector->y = y;
                        attack_array[target_selector->y][target_selector->x] = LED_BLINKING;
                        return;
                    } else if (!attack_array[y][x + rowCounter] && (size_t)(x + rowCounter) < width) {
                        attack_array[target_selector->y][target_selector->x] = LED_OFF;
                        target_selector->x = x + rowCounter;
                        target_selector->y = y;
                        attack_array[target_selector->y][target_selector->x] = LED_BLINKING;
                        return;
                    }
                    rowCounter++;
                } while (attack_array[y][x] && rowCounter < width);
            }

            break;

        case LEFT:
            for (int x = target_selector->x - 1; x >= 0; x--) {
                if (!attack_array[target_selector->y][x]) {
                    attack_array[target_selector->y][target_selector->x] = LED_OFF;
                    target_selector->x = x;
                    attack_array[target_selector->y][x] = LED_BLINKING;
                    return;
                }
            }

            break;

        case RIGHT:
            for (size_t x = target_selector->x + 1; x < width; x++) {
                if (!attack_array[target_selector->y][x]) {
                    attack_array[target_selector->y][target_selector->x] = LED_OFF;
                    target_selector->x = x;
                    attack_array[target_selector->y][x] = LED_BLINKING;
                    return;
                }
            }

            break;

    }
}
