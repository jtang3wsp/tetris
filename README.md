# Tetris
Modern Tetris made in C++ with SFML

- Soft drop/hard drop
- Clockwise and counterclockwise rotation functions (using SRS wall kick data)
- Hold
- 5 upcoming pieces view
- Ghost piece
- 7-piece bag randomizer

# IMPORTANT
GRID IS DISPLAYED IN ROW-MAJOR ORDER. TRADITIONAL X AND Y COORDINATES ARE SWAPPED
- (+X = RIGHT, -X = LEFT)
- (+Y = DOWN, -Y = UP)

WALL KICK DATA WAS MODIFIED ACCORDINGLY

Fix/Add
- SMOOTHER CONTROLS
- Better gravity/lock timers
- Sometimes a ghost mino will overwrite one of its original Tetromino's minos (?)
