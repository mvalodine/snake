# Snake Game in C

This is a Snake game implementation in C, featuring four progressively difficult levels. The game is designed for the RBX430-1 board and utilizes the LCD screen for display and button inputs for control.

Features:
- Four Levels of Difficulty:
- Level 1: Cyclical play area, 10 foods worth 1 point each, slow speed, and a 30-second time limit.
- Level 2: Perimeter walls with 90-degree turns, 10 foods worth 2 points each, and medium speed.
- Level 3: Electric perimeter walls, 1 food at a time worth 3 points, and faster speed.
- Level 4: Electric perimeter walls, 1 food at a time worth 4 points, and the fastest speed.
- Snake Movement: The snake moves automatically, and the player uses buttons to change direction.
- Obstacles: As the game progresses, obstacles (rocks) appear on the screen to increase difficulty.
- Scoring: The snake grows longer with each food eaten, and the player scores points for each food consumed.
- Game End: The game ends if the snake hits an obstacle, itself, or the time runs out.
- King Snake: Completing all four levels crowns the player as the "King Snake" with a maximum score of 100 points.

Requirements:
- Hardware: RBX430-1 board (MSP430)
- Software: Code Composer Studio (Version 5.2.0.00090)
- Libraries: RBX430-1.h, RBX430_lcd.h, snake.h

Installation:
- Clone or download the repository.
- Open the project in Code Composer Studio.
- Build the project and load it onto the RBX430-1 board.
- Connect the board to an LCD screen for visual output.

How to Play:
- Use the buttons on the RBX430-1 board to change the direction of the snake.
- The snake moves automatically, and you must guide it to eat food and avoid obstacles.
- The game ends when the snake hits an obstacle or itself, or when the time runs out.

Controls:
- Button 1: Move Up
- Button 2: Move Down
- Button 3: Move Left
- Button 4: Move Right

Author:
The original program and framework were written by Professor Paul Roper from Brigham Young University. The game was edited and completed by Michelle Valodine, who adapted the code to work and implemented the game as part of a programming challenge.
