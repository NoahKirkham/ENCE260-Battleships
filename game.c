/**
    @file game.c
    @author Noah Kirkham, nki59
    @author Hansana Peiris, thp31
    @date 05/10/2025
    @brief Main game loop, as state machine
*/


#include "pio.h"
#include "pacer.h"
#include "navswitch.h"
#include "system.h"
#include "matrix.h"
#include "battleship.h"
#include "button.h"
#include "ir_uart.h"
#include "ir_game.h"
#include "tinygl.h"
#include "font5x7_1.h"

#include <stdbool.h>
#include <stdlib.h>

#define PACER_RATE 100

#define ARRAY_WIDTH 5
#define ARRAY_HEIGHT 7

#define SHIP_COUNT 3

#define HIT 2
#define MISS 1

#define CONFIRMATION_TIME 5 // Time in seconds

#define MESSAGE_RATE 8

int main(void)
{
    system_init();
    navswitch_init();
    matrix_init();
    button_init();
    pacer_init(PACER_RATE);
    ir_uart_init();

    button_update(); // Allows for on board restart

    srand(TCNT1);

    tinygl_init(PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);

    int matrix_interval = 0;

    int matrix_array[ARRAY_HEIGHT][ARRAY_WIDTH] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
    };

    int attack_array[ARRAY_HEIGHT][ARRAY_WIDTH] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
    };

    enum game_state_e {
        SHIP_PLACEMENT,             // Ships Are Being Placed
        WAITING_FOR_PLAYER,         // Waiting For The Other Player To Place Ships
        ATTACK,                     // Attack the opponent
        AWAITING_SHOT_CONFIRMATION, // Wait for a shot confirmation from opponent
        WAITING_FOR_ATTACK_RESULT,  // Wait while opponent attacks
        WIN,                        // Win
        LOSE,                       // Lose
        GAME_OVER                   // Reset and restart game
    };

    enum game_state_e game_state = SHIP_PLACEMENT;

    int ships_to_allocate = SHIP_COUNT; // Number Of Ships Left To Be Put On The Board
    bool allocation_in_progress = false; // Boolean For If An Allocatation Is In Progress
    current_ship_s current_ship;

    int random_number = rand() % 10;    // Number to decide the starting 
    int random_number_opponent;         // Number to decide the starting for opponent

    target_selector_s target_selector;
    int shot_confirmation;
    bool shot_rendered = false;
    int hit_counter = 0;
    int timeout_counter = 0;

    int received_shot_location;
    bool received_shot = false;
    int opponent_hit_counter = 0;

    while (1) {
        pacer_wait();
        navswitch_update();
        button_update();

        switch (game_state) {
            case SHIP_PLACEMENT:

                /** Display Game */
                display_matrix(ARRAY_WIDTH, ARRAY_HEIGHT, matrix_array, matrix_interval >= PACER_RATE / 2);

                if (ships_to_allocate <= 0) {
                    game_state = WAITING_FOR_PLAYER;
                }

                if (ships_to_allocate > 0 && !allocation_in_progress) {
                    ship_init(ships_to_allocate, &current_ship, ARRAY_WIDTH, ARRAY_HEIGHT, matrix_array);
                    allocation_in_progress = true;
                }

                /** Navswitch | Rotate and move the ship */
                if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                    rotate_ship(ships_to_allocate, &current_ship, ARRAY_WIDTH, ARRAY_HEIGHT, matrix_array);
                }
                if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
                    move_ship(ships_to_allocate, &current_ship, ARRAY_WIDTH, ARRAY_HEIGHT, matrix_array, UP);
                }
                if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
                    move_ship(ships_to_allocate, &current_ship, ARRAY_WIDTH, ARRAY_HEIGHT, matrix_array, DOWN);
                }
                if (navswitch_push_event_p(NAVSWITCH_EAST)) {
                    move_ship(ships_to_allocate, &current_ship, ARRAY_WIDTH, ARRAY_HEIGHT, matrix_array, RIGHT);
                }
                if (navswitch_push_event_p(NAVSWITCH_WEST)) {
                    move_ship(ships_to_allocate, &current_ship, ARRAY_WIDTH, ARRAY_HEIGHT, matrix_array, LEFT);
                }

                /** S3 Button | Confirm ship placement */
                if (button_push_event_p(0)) {
                    board_state_change(LED_BLINKING, LED_ON, ARRAY_WIDTH, ARRAY_HEIGHT, matrix_array); // Convert pixel states from 2 to 1
                    ships_to_allocate--;
                    allocation_in_progress = false;
                }

                break;

            case WAITING_FOR_PLAYER:

                /** Display Game */
                display_matrix(ARRAY_WIDTH, ARRAY_HEIGHT, matrix_array, matrix_interval >= PACER_RATE / 2);

                board_state_change(LED_ON, LED_BLINKING, ARRAY_WIDTH, ARRAY_HEIGHT, matrix_array); // Convert pixel states from 1 to 2
                
                if (!matrix_interval) {
                    ir_send(PLAYER_MOVE, random_number);
                }
                
                if (ir_receive(PLAYER_MOVE, &random_number_opponent)) {
                    if (random_number > random_number_opponent) {
                        board_state_change(LED_BLINKING, LED_ON, ARRAY_WIDTH, ARRAY_HEIGHT, matrix_array); // Convert pixel states from 2 to 1
                        game_state = ATTACK;
                        ir_send(PLAYER_MOVE, random_number);
                    } else if (random_number < random_number_opponent) {
                        board_state_change(LED_BLINKING, LED_ON, ARRAY_WIDTH, ARRAY_HEIGHT, matrix_array); // Convert pixel states from 2 to 1
                        game_state = WAITING_FOR_ATTACK_RESULT;
                        ir_send(PLAYER_MOVE, random_number);
                    } else {
                        random_number = rand() % 10;
                    }
                }
                
                break;

            case ATTACK:

                /** Disptarget_selectorlay Game */
                display_matrix(ARRAY_WIDTH, ARRAY_HEIGHT, attack_array, matrix_interval >= PACER_RATE / 2);

                if (allocation_in_progress) {
                    /** Navswitch | Move the target selector */
                    if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
                        move_target_selector(ARRAY_WIDTH, ARRAY_HEIGHT, attack_array, &target_selector, UP);
                    }
                    if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
                        move_target_selector(ARRAY_WIDTH, ARRAY_HEIGHT, attack_array, &target_selector, DOWN);
                    }
                    if (navswitch_push_event_p(NAVSWITCH_EAST)) {
                        move_target_selector(ARRAY_WIDTH, ARRAY_HEIGHT, attack_array, &target_selector, RIGHT);
                    }
                    if (navswitch_push_event_p(NAVSWITCH_WEST)) {
                        move_target_selector(ARRAY_WIDTH, ARRAY_HEIGHT, attack_array, &target_selector, LEFT);
                    }

                    /** S3 Button | Confirm shot */
                    if (button_push_event_p(0)) {
                        ir_send(SHOT_REQUEST, (target_selector.y * ARRAY_WIDTH) + target_selector.x);
                        allocation_in_progress = false;
                        game_state = AWAITING_SHOT_CONFIRMATION;
                    }
                } else {
                    allocation_in_progress = true;
                    init_target_selector(ARRAY_WIDTH, ARRAY_HEIGHT, attack_array, &target_selector);
                }

                break;
            
            case AWAITING_SHOT_CONFIRMATION:

                /** Display Game */
                display_matrix(ARRAY_WIDTH, ARRAY_HEIGHT, attack_array, matrix_interval >= PACER_RATE / 2);

                /** Get Shot Result From Opponent */
                if (ir_receive(SHOT_RESULT, &shot_confirmation) && !shot_rendered) {
                    attack_array[target_selector.y][target_selector.x] = shot_confirmation;

                    if (shot_confirmation == HIT) {
                        hit_counter++;
                    }

                    shot_rendered = true;
                    timeout_counter = 0;
                } else if (timeout_counter >= PACER_RATE && !shot_rendered) {
                    ir_send(SHOT_REQUEST, (target_selector.y * ARRAY_WIDTH) + target_selector.x);
                    timeout_counter = 0;
                }

                /** After Timeout Complete, Move To Next Game Stage */
                if (timeout_counter >= PACER_RATE * CONFIRMATION_TIME && shot_rendered) {
                    /** Win Condition */
                    if (hit_counter >= ((SHIP_COUNT + 1) * (SHIP_COUNT + 2) / 2) - 1) { // Sum of pixels in the ships
                        game_state = WIN;
                        tinygl_clear();
                        tinygl_text("WIN!");
                        timeout_counter = 0;
                    } else {
                        game_state = WAITING_FOR_ATTACK_RESULT;
                        shot_rendered = false;
                        timeout_counter = 0;
                    }
                }

                timeout_counter++;

                break;

            case WAITING_FOR_ATTACK_RESULT:

                /** Display Game */
                display_matrix(ARRAY_WIDTH, ARRAY_HEIGHT, matrix_array, matrix_interval >= PACER_RATE / 2);

                /** Check For Opponent's Shot */
                if (ir_receive(SHOT_REQUEST, &received_shot_location)) {
                    ir_send(SHOT_RESULT, matrix_array[(received_shot_location - (received_shot_location % ARRAY_WIDTH)) / ARRAY_WIDTH][received_shot_location % ARRAY_WIDTH] + 1);

                    if (matrix_array[(received_shot_location - (received_shot_location % ARRAY_WIDTH)) / ARRAY_WIDTH][received_shot_location % ARRAY_WIDTH]) {
                        matrix_array[(received_shot_location - (received_shot_location % ARRAY_WIDTH)) / ARRAY_WIDTH][received_shot_location % ARRAY_WIDTH] = LED_BLINKING;
                        opponent_hit_counter++;
                    }

                    received_shot = true;
                }

                /** Timeout Till Next Game Stage */
                if (received_shot) {
                    if (timeout_counter >= PACER_RATE * CONFIRMATION_TIME) {
                        if (opponent_hit_counter >= ((SHIP_COUNT + 1) * (SHIP_COUNT + 2) / 2) - 1) { // Sum of pixels in the ships
                            game_state = LOSE;
                            tinygl_clear();
                            tinygl_text("LOSE!");
                            timeout_counter = 0;
                        } else {
                            game_state = ATTACK;
                            received_shot = false;
                            timeout_counter = 0;
                        }
                    }
                    timeout_counter++;
                }

                break;

            case WIN:

                tinygl_update();

                timeout_counter++;

                if (timeout_counter >= PACER_RATE * CONFIRMATION_TIME) {
                    game_state = GAME_OVER;
                    timeout_counter = 0;
                }
                
                break;

            case LOSE:

                tinygl_update();

                timeout_counter++;

                if (timeout_counter >= PACER_RATE * CONFIRMATION_TIME) {
                    game_state = GAME_OVER;
                    timeout_counter = 0;
                }
                
                break;

            case GAME_OVER:

                /** Clean Up */
                game_state = SHIP_PLACEMENT;
                for (int y = 0; y < ARRAY_HEIGHT; y++) {
                    for (int x = 0; x < ARRAY_WIDTH; x++) {
                        matrix_array[y][x] = 0;
                        attack_array[y][x] = 0;
                    }
                }
                ships_to_allocate = SHIP_COUNT;
                timeout_counter = 0;
                allocation_in_progress = false;
                random_number = rand() % 10;
                shot_rendered = false;
                hit_counter = 0;
                received_shot = false;
                opponent_hit_counter = 0;
                tinygl_clear();

                break;

        }

        matrix_interval = (matrix_interval + 1) % PACER_RATE;

    }
}
