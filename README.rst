Stupid Goddamn Tile Game
========================

Controls
########
::

    Move/jump      : Arrow keys
    Take a block   : Right shift
    Drop a block   : Space
    Pause          : p
    Quit           : Esc.

    Toggle slow-mo : t
    (useful for debugging)

Creating new levels
###################

Level directory
---------------

You need to create a subdirectory under ``maps``. The name of the directory
should be the level number. So if you want to make level 1, create the directory
``maps/1``. You should ensure there are no gaps between level
numbers-- i.e. if levels 1 and 2 already exist, the only level you can create
next is 3.

Level files
-----------

Inside the level directory, create two plain text files, ``maps`` and
``background``.

* ``maps``: Each character in this file represents a single tile on the
  screen (see the **Map Symbols** section below). A space character means
  a blank tile-- empty space. The game window is always 16 tiles in height,
  so there must be at least 16 lines in this file. Extra lines will be
  ignored. A level can be, at most, 1024 tiles long. If any lines in this
  files are longer than 1024 characters, the extra data will be ignored.
* ``background``: Each character in this file represents a tile of
  background scenery. Background tiles are half the size of map tiles, so
  there must be at least 32 lines in this file. Extra lines will be ignored.
  If the width of the background is smaller than the length of the
  level, the pattern will be repeated. If larger, the extra data will be
  ignored.

Map Symbols
###########
::

    # : Map tile (fixed)
    x : Map tile (take-able)
    S : Player start/respawn position
    F : Player finish position
    

