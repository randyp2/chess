# Board Setup - Bitboards


Each bit board represents the current state of the chess board using a unit64

Logical mapping

(MSB)
a8 -> 63, . . . . . . . .  .  .  h8 -> 56
.                                   . 
.                                   . 
.                                   . 
.                                   . 
.
a2 -> 15                         h2 -> 8
a1 -> 7, b1 -> 6, c1 -> 5, ... , h1 -> 0 (LSB)

To display piece positions:
    - Bitboard for every piece

    White: Pawn, knight, bishop, rook, queen, king
    Black: Pawn, knight, bishop, rook, queen, king

Total bitboards: 12

## Fen Strings

**example:**
Fen string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

**KEY**
**Lower case letters:** black pieces 
    - r = rook, k = king, n = knight
**Upper case letters:** white pieces
**Numbers:** Number of blank spaces in between pieces 
**/:** Next rank
**w | b:** Located at the end of fen string seperated by space denotes whose turn it is
