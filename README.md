
*@author Noah Kirkham, nki59
@author Hansana Peiris, thp31
@date 13/10/2025*

*----- Battleships -----*

To play, start the UCFK4.

Overview: 
The objective is to sink all of your opponent's ships by guessing their position on the board. The starting player is chosen at random. The first player to sink all ships wins.

You have three ships to place:
- Aircraft Carrier  (4 pixels long)
- Battleship        (3 pixels long)
- Destroyer         (2 pixels long)

*-- Quick Play*

Controls For All Game states
- Navswitch Up      (Moves up)
- Navswitch Down    (Moves down)
- Navswitch Left    (Moves left)
- Navswitch Right   (Moves right)
- Navswitch Press   (Rotates)
- S3                (Action Button)

Attack mode
- Flashing = Hit
- Solid = Miss/Not Hit Yet

*-- Ship Placement*

The first game state, the state which is automatically booted into, is the ship placement state. In this state you are to place your ships on the 5x7 game board.

The ships will automatically spawn in vertical, in the first position that is free, it will check left to right, then down the board till a legal position is found. The current ship is distinguishable by its blinking lights, whereas already placed ships will be a solid light.

You are able to move the ship with the black 5 way navswitch, moving the navswitch position to up, down, left, right, will move the ship in the respected direction if there is space available. Pressing the navswitch with rotate the ship around the top left pixel, the ship will not rotate if there is no space. To confirm the placement of your current ship, press the white S3 button on the left of the LED matrix display.

Controls in this game state:
- Navswitch Up      (Moves ship up)
- Navswitch Down    (Moves ship down)
- Navswitch Left    (Moves ship left)
- Navswitch Right   (Moves ship right)
- Navswitch Press   (Rotates ship)
- S3                (Confirms ship placement)

*-- Waiting For Player To Their Place Ships*

Once all three ships have had their placements confirmed, if the other player is still placing their ships, your ships will start to blink to show this. No steps are needed from you, once the other player confirms their ships, the game will proceed.

*-- Attack*

The game will decide a player 1 and player 2, player one will start off attacking, while the other player will be waiting for the oppostions attack (Refer to the next section).

When attacking, your board state will change to attack mode, in this mode you will see your target selector and previous shots you have made.

Your target selector will first appear in the first position from the top left, if no position in the first row are available then it will check the next and so on. Your target selector will be blinking. You are able to move your target selector with the navswitch, up, down, left, and right will move it in their respected directions.

Left and right will only move the target selector within the same row into the next free position available, whereas the up and down will first try the position immediately above/below, and then the pixel on the left then right, moving outwards from the initial position until a legal space is found.

Your previous shots will also appear in this screen: 
- Solid pixels indicate missed shots. 
- Blinking pixels indicate hits. 
- Blank pixels have not had shots fired yet.

Controls in this game state:
- Navswitch Up      (Moves target selector up)
- Navswitch Down    (Moves target selector down)
- Navswitch Left    (Moves target selector left)
- Navswitch Right   (Moves target selector right)
- S3                (Confirms shot)

*-- Waiting For Player Attack*

In this game state, your board will display your ships. Pixels that are solid are positions where your ship has not been hit, and blinking pixels are where your ship has been hit. No actions are required by the player in this state.

*-- Win*

This state is triggered when you sink all of your opponents ships. The game displays "WIN!" scrolling across the screen for 5 seconds, before transitioning to the 'Game Over' state.

*-- Lose*

This state is triggered when you're opponent sinks all of your ships. The game displays "LOSE!" scrolling across the screen for 5 seconds, before transitioning to the 'Game Over' state.

*-- Game over*

After either a win or loss, the game resets all variables. The ship placement and attack matricies are cleared. Then the game is reset by transitioning back to the 'Ship Placement' state, allowing players to begin a new match.


AI Statement:
Some generative AI - ChatGPT was used to explain how different functions can be used together to achieve the desired outcome, to get an idea of how the code should be structured. 