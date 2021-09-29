# B-Tree with cache
Implementation of B-Tree with cache.
The goal of this project is to implement a B-tree and to measure an enhanced structure where an amount of cache can be used. 
The basic operations are:
* I x - Construct a tree of order x.
* A x - Add the value x to the tree, x is a signed integer. The values added are unique, i.e. there will be no two insertions of the same value (except the case when it was deleted in between).
* ? x - Check if the value x is in the tree. If it is present print "x +", otherwise print "x -".
* P   - Print all values that are in the tree in increasing order.
* L t - Load a tree of order t. In the next line there is a tree in a format given as follows. A pair of brackets ( ) signify a node. A value signify a record (key in the tree). <br>Hence ( ( ( 1 2 ) 3 ( 4 ) 5 ( 6 7 ) ) 8 ( ( 9 ) 10 ( 11 ) ) ) can be interpreted as the following tree:
<pre>
                                        (         .           8       .    )
                                        (  . 3    .  5   .  )   ( . 10  .  )
                                        ( 1 2 ) ( 4 ) ( 6 7 )   ( 9 ) ( 11 )
</pre>
* S - Save the tree in the format described above.
* R x - Remove the element x from the tree. The element may be present or not. Hence the operation might not change the elements in the tree, but may force reorganization.
* \#   - Ignore the line.
* X - End the program.
* C x q<sub>1</sub>, q<sub>2</sub>, ... - Calculate an impact of a hypothetical cache.<br>
Assume that we are given a set of quarries q<sub>1</sub>, ..., q<sub>n</sub> (the list of quarries ends by a new line character). The program can store the result of the quarry in a cache of size x - measured in the number of entries (keys) that can be stored. 
Hence, if the next quarried entry was already visited, the program does not have to visit B-tree, but may simply read the data from cache. Due to the fact that the size of the cache is much smaller than the size of B-tree, only chosen entries can be stored. The implemented policy is First-In-First-Out. I.e. the first entry that was read to the cache is removed from it when some other entry is read from B-tree and the result has to be added to the cache. 
