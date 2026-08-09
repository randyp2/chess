# Magic bitboards
Fast technique for fast lookups regarding possible attack moves for 
sliding pieces.

> [!CAUTION]
> The slow way to do this is to iterate through every single attack direction
> for a sliding piece until you come across a blocker or an edge. This can be
> extremely slow especially if you do this everytime for a moved piece
> considering the board and attack squares change everytime a piece is moved.

**SOLUTION**
Introduce some sort of hashtable that hashes the current position of the blockers
relative to the current position of the sliding piece. 

The return value is the possible attack squares that the sliding piece can go to

---
**Implmenetation**
Iterate through every single square and generate a mask considering there are no 
blockers.

Then for each square that can be attacked, generate all possible combinations 
of blockers. For each combination create an attack table (bitboard) and store them 
as the value to the hashed occupied bitboard.

