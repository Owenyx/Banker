    Overview:
The program will take an input file "input.txt" that specifies:
N - the number of processes
M - the number of resource types
NxM MAX matrix that contains the MAX amount of each resource (column) that a process (row) will need
AVAILABLE vector that contain M columns, specifying how much of each resource type is available

The input file is formatted as follows:
N M
MAX[0][0] MAX[0][1] ... MAX[0][M]
MAX[1][0] MAX[1][1] ... MAX[1][M]
...
MAX[N][0] MAX[N][1] ... MAX[N][M]
AVAIL[0] AVAIL[1] ... AVAIL[M]

After processing input, the program asks the user which process they would like to request resources for.
It then asks for the request vector of that process, formatted as:
REQ[0] REQ[1] ... REQ[M]

After recieving user input, the program uses the Banker's algorithm to determine if the request should be granted,
displaying a error message if not and a success message if so.
After either message, the program dumps all resource matrices and vectors - MAX, AVAIL, ALLOC, and NEED in that order

Processes never give up alloated resources.

    Execution:
To run the program in Ubuntu, navigate to the directory containing the program and execute:
    make
    ./main
Then just follow the program's instructions and the steps detailed in the overview.
input.txt must be present in the same directory for the program to work.

To terminate the program, enter a negative process value when prompted "What process?"