# Space Game

![Game preview](assets/preview.gif)

A space defense game written in C using NCURSES. Inspired by Space Impact, on the Nokia 3310 et seq.

## Directions
* <kbd>←</kbd><kbd>↑</kbd><kbd>↓</kbd><kbd>→</kbd> : Move ship
* <kbd>space</kbd> : Shoot
* <kbd>p</kbd> : Pause
* <kbd>q</kbd> : Quit

## Installation
Dependencies: `ncurses`

**Mac**
1. Open Terminal: <kbd>cmd</kbd>+<kbd>space</kbd>, search "Terminal"
2. Run `git clone https://github.com/eugenekkim8/space_game.git`
3. Run `cd space_game`
4. Run `make`
5. Run `bin/release/main` to launch.
6. To uninstall, run `cd -` and then `rm -r space_game` (accepting any overrides).

**Windows**

A lot harder, unfortunately. Consider installing Cygwin with packages `git`, `gcc`, `make`, and `ncurses` and following the Mac instructions above.

This game has not been tested on Windows. 
