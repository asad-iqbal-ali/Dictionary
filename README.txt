Asad Ali
MPCS51100
HW3

Compilation instructions:
p1: "gcc p1.c chtbl.c list.c -lrt"
p2: "gcc p2.c rbtree.c -lrt"

Both programs take the following commands:
add [word] [definition]
find [word]
find [word1] [word2]
print
delete [word]
read [filename]
test [filename]
exit

I used a link to the realtime library because I wanted to see how fast each program could read a file (that's what the command "test" does - it then prints out the time in nanoseconds), but "clock()" does not have a high enough resolution to do this for such a small dictionary so I had to use some time.h functions. 

Performance:

I found that the hash table could read a file in ~500,000 nsecs, while the tree took ~750,000. This makes sense, since the tree is O(lgn) for insertions while the table is O(1), but I would have assumed that the cost of rehashing would add up. It's possible that this would be the case for larger sample sizes.

The tree's balancing was also hard to evaluate on only 200 entries, since lg(200) < 8, so the room for error was fairly large. I found that the tree consistently ended up with height 10, which I thought was a huge problem until I realized that that's just a couple extra leaf nodes inserted in less-than-perfect spots. Again, I'd like to test the balancing on much larger sample sizes if possible.

p1:

p1 is less portable than p2, since a number of hash table functions are written into p1.c instead of chtbl.c. This was because I hadn't really gotten the hang of figuring out where to put things before I started, and by the time I finished I didn't really have time to move the functions around. Also, frankly I was scared to touch anything now that it actually worked.
 I used the provided list and chtbl .h and .c files, with alterations to account for the fact that the key and data were separate values for entries in the table. My program uses the division method for f, and three different g functions (listed in p1.c). It switches between g functions on each rehash. Since the f function is hard-wired into the implementation, it was much easier to change the g function.

My rehashing function starts by doubling the size of the table, and then runs through the first half of the new table(since there would not be any entries in the second half unless they've already been rehashed), removing the head of the list at each entry and reinserting it into the table for as many items as the list originally had. To do this, I had to edit the insertion function to append new items to the end of the lists instead of at the head, so that I wouldn't end up repeatedly pulling and rentering the same item in the event that it ended up in the same bucket upon rehashing. I figured this was the most straightforward method of rehashing. I ran a rehash every time the load factor went above 1.

The print and find2 functions create an array of pointers to list elements, then use qsort to put them in alphabetical order. It's not ideal, but since the hash table doesn't preserve order I didn't know what else to do.

The program outputs a log file called "hashlog.txt" with the specified data. 

p2:

I used a method 2 tree, and tried to make this one more portable, so all general tree functions are written in rbtree.c.

Again, I used a lot of the provided functions, with the addition of a few lines to account for the key and data being separate entities for each node. I did notice, however, that the provided functions for rotations did not preserve colors, update parentage (for the parent and child sub-trees), or account for changed addresses. The first two were easy enough to fix, but the last one required a bit of finagling(sp?) whenever I rotated around a node. So for example, if I have pointers to a node, n, and it's right child, c, and I use left_rotate(n), then once that returns n will be pointing to c and c will be pointing to n. I tried to account for this wherever applicable (it really only comes up in insertion case 4 and deletion case 3, since insertion case 5 and deletion case 1 return without further alterations once they rotate nodes), but I may have been overly cautious so there's probably a few unnecessary pointer adjustments.

The program outputs to a log file called "treelog.txt" with the tree depth listed on each insertion.

