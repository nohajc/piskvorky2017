# Build

The project uses cmake, so it can be built using e.g. CLion or simply in terminal:

`mkdir build; cd build; cmake ..; make`

The resulting binary will be in `bin`.

Tested on Linux. Might work on OS X. Windows support incomplete.

# Dependencies

## Linux
* gtk3
* boost

# About

This is an implementation of generalized *NxN* tic-tac-toe where the winner must get *K* crosses/naughts in a row.

Logic is implemented in C++, UI in HTML, CSS and TIScript (using Sciter engine - https://sciter.com/).

## Design

The game is represented by a C++ class `Game`. `Game` consists of two `Player`s and a `Grid` (game board).

Method `Game::play()` is the game loop. In the game loop, we perform moves. Current player can propose coordinates where he wants to place his mark. The `Game` class is then responsible for validating and actually performing the move.

`Player` is an abstract class with virtual methods, so we can derive a `HumanPlayer` which registers handler on the UI click event and listens for a click in a particular game board cell, or `MinMaxPlayer` who can create a game tree of possible moves for several rounds ahead (or even precompute all games for 3x3 board). Such AI player then always chooses the best move (out of the discovered ones). We also implement `TestPlayer` who just returns coordinates of a predefined game (given as a vector of cells) which is useful for testing. We simulate a game an check for an expected outcome.

`Grid` is a wrapper for 2D vector of `Cell`s. A `Cell` can be empty or it can hold cross/naught. When we check for victory of a `Player` (after each move), we have to search for a row of crosses/naughts in every direction. It is therefore useful to implement horizontal, vertical and diagonal iterators on `Grid`. We do that with the help of **Boost Iterator Adaptor**.

`Frame` is derived from `sciter::windows` and it is the glue between game logic and UI. It exposes some of the C++ methods to TIScript, so that we can configure the game parameters using a HTML form and then call game initialization from the `sumbit` event handler.

All the C++ sources are in `src`. The `res` directory contains HTML layout, CSS stylesheet and UI script. The script implements main menu, generates the visible representation of game board and manages events.

## Notes

The AI we implement using the well known MinMax algorithms has some limitations. The time and memory complexity of precomputing all possible games would be *O((N * N)!)*, so we limit the number of game tree nodes by a constant. In consequence, the accuracy of predicting the best move decreases with increasing *N*.

What sometimes happens (especially if *K* is a lot smaller than *N*) is that the `MinMaxPlayer` prefers moves that help him but he does not play defensively at all. This is caused by the incomplete prediction and non-locality of moves. If the human counterpart places his marks in the lower right corner, chances are the game tree will not reach that area of the board, because AI generates all possible next moves from left-top to right-bottom every time.

We tried to address that problem by generating the game tree nodes in BFS order which should better approximate the idea of looking at *M* moves ahead instead of going in DFS order at the very end of only one possible string of moves, potentially depleting the maximum number of allowed nodes in memory before exploring the neighboring cells from the current move.

This works in some cases but it would probably help to search neighboring cells (relative to the last move) first, instead of always going from left-top to right-bottom (as mentioned above).
