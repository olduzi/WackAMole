# WackAMole
A short arduino project using LED lights and buttons to play WackAMole

For my implementation of Wack a Mole, I created a Mole class and made four mole objects, one for each LED on my breadboard. A random mole will "pop out" and the light will turn on until the play pressed the appropriate button, or until the mole's lifespan goes out. Every 25 seconds of the game play, the lifespan of the moles decrease but the amount of points earned for hitting the mole increases. If the player doesn't hit the mole in time, they recieve a strike and lose points. If the player hits the wrong button the player loses points, but doesn't receive a strike. Once the player receives 5 strikes, the game ends, the lights flash, and the buzzer rings.
