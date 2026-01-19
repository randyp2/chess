# 1/15/2026 - Board Setup
**Start Time: 1/11/26 **
**Endt Time: Present **

## Goals 
- Make this chess program work logically
- Valid moves only able to be played
 - Learn how to use bit manipulation to generate moves
 - Learn how to store generate moves
 - Test for ms runtimes to generate possible moves

### Encoded Moves:
Resource: https://www.chessprogramming.org/Encoding_Moves
- Moves can be encoded in 16 bits

** 0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0**
   |    MOVE_FLAG |        TO_SQUARE            |          FROM_SQUARE        |


**Position**
    - Only knows the state of the Board
    - Doesn't know the legal moves

**Move**
    - Class that defines what a move is using the encoded moves

**Move Generation**
Resource: https://www.chessprogramming.org/Move_Generation
    - Knows what moves are legal and pseudo legal

Pseudo-legal moves:
In Pseudo-legal move generation pieces obey their normal rules of movement, but they're not checked beforehand to see if they'll leave the king in check. It is left up to the move-making function to test the move, or it is even possible to let the king remain in check and only test for the capture of the king on the next move.
TL;DRL: Only consider how the piece moves itself without considering king safety.

Legal: 
In Legal move generation, as the name implies, only legal moves are generated, which means extra time must be spent to make sure the king isn't going to be left or placed in check after each move. Pins are the main difficulty, particularly when en passant is involved.
TL;DRL: Keep king safety in mind




*PAWNS*
 1) One square move
 2) Double pawn push - only at Start
 3) Diagonal Captures
 4) Enpassant
 5) Promotions on last rank

 **1) One Pawn Move**

