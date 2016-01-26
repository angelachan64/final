# BUGS

These are of list of known problems we were not able to fix. Some are not necessarily problems, but things we would have liked to include

- When creating a new game, and selecting a port, if the port is already in use, there will be an error message displayed. However, it exits the program, and forces the user to restart. It is reccomended that the user simply increment the port number by 1.
- When joinging a game, the program DOES NOT show the list of ports that are being used, so the user will have to know a port number that is in use before-hand in order to get the game that he/she wants. Because we recommend that a user start at port 5000 and increment by 1 if a port number is already in use, the user can simply start at 5000 and and keeping incrementing by one until he/she finds a game.