***The Breakout Game on a 8 × 8 LED matrix***

-not finished-

Hardware:
- Arduino UNO board
- breadboard
- 8 × 8 LED matrix
- 16 × 2 LCD
- MAX7219 Driver 
- joystick
- resistors
- active buzzer
- RGB LED
- potentiometers
- 10μF capacitor
- 104 ceramic capacitor
- wires
- connector cable

Appearance of the hardware:

https://ibb.co/T8chywM

Description:
The classic Breakout Game, but this time on an Arduino board!
The player has to break all the bricks with a ball which jumps on a platform. And since we are in December, you can have a Christmassy song played while you are playing.
- there is a score and 3 lives;
- you can introduce your name to keep record of who had the highscore (highscore is saved in EEPROM);
- there are levels;
- each level generates different configuration of bricks (each level adds a new bricks row)

How to play:
Using the potentiometer, you move the platform in order to catch the ball. The ball jumps and breaks a brick at a time. Game target: break all the bricks without letting the ball fall off the paddle.

_ _IN PROGRESS_ _: add rows, do something about the LCD, make the game playable without cancelling the song.
