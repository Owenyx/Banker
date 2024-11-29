# Banker
one file contains
N M
N row M column matrix of MAX
Row of AVAILABLE

ex. in file:
5 3
3 9 4
2 5 7
4 2 1
0 0 1
4 4 2
12 9 13

program will read in AVAILABLE and MAX

program will ask:
What process? 2
Request vector? 1 0 2

When getting request vector, you know to read M numbers

Check that:
Request > need - "Request denied - Exceeded maximum claim"
Request > available - "Request denied - Not enough available resources"
Unsafe - "Request denied - Will leave system in unsafe state"
Success - "Request granted"

Output ex.

What process? 1
Request vector? 1 0 2
Request granted
dump NEED, ALLOC, AVAILABLE, and MAX
What process? 2
Request vector? 2 0 5
...
End if:
What process? -1 (or any negative number)
still dump NEED, ALLOC, AVAILABLE, and MAX
<terminate>


Start program with nothing allocated and ofc need = max
start process and resources indices at 0
Test the example on the slides
So start with 10 5 7 try and allocate
0 1 0
3 0 2
3 0 2
2 1 1
0 0 2


read N and M
shape MAX, NEED, AVAILABLE, ALLOC vectors
pass banker MAX, AVAILABLE, ALLOC

processes never release resources