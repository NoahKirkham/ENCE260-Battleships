/**
    @file battleship.h
    @author Noah Kirkham, nki59
    @author Hansana Peiris, thp31
    @date 05/10/2025
    @brief Ship control and board management
*/


#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include "pio.h"
#include "system.h"

typedef struct {
    int x;
    int y;
    int rotation;   // 0 For Verticle, 1 For Horizontal
} current_ship_s;

typedef struct {
    int x;
    int y;
} target_selector_s;

typedef enum {
    UP,     // Move Up
    DOWN,   // Move Down
    LEFT,   // Move Left
    RIGHT   // Move Right
} movement_direction_e;

/**
    Initializes And Places A New Ship On The Game Board.

    @PARAM int ships_to_allocate
    @PARAM current_ship_s *current_ship
    @PARAM size_t width
    @PARAM size_t height
    @PARAM int matrix_array[height][width]

    @RETURN void
*/
void ship_init(int ships_to_allocate, current_ship_s *current_ship, size_t width, size_t height, int matrix_array[height][width]);

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
void rotate_ship(int ships_to_allocate, current_ship_s *current_ship, size_t width, size_t height, int matrix_array[height][width]);

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
void move_ship(int ships_to_allocate, current_ship_s *current_ship, int width, int height, int matrix_array[height][width], movement_direction_e direction);

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
void board_state_change(int initial, int final, size_t width, size_t height, int matrix_array[height][width]);

/**
    Initialises The Target Selector On The First Free Pixel
    (A Pixel That Hasn't Been Shot Thus Far)

    @PARAM size_t width
    @PARAM size_t height
    @PARAM int attack_array[width][height]
    @PARAM target_selector_s *target_selector

    @RETURN void
*/
void init_target_selector(size_t width, size_t height, int attack_array[height][width], target_selector_s *target_selector);

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
void move_target_selector(size_t width, size_t height, int attack_array[height][width], target_selector_s *target_selector, movement_direction_e direction);


#endif // BATTLESHIP_H
