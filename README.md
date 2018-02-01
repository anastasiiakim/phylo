# phylo
programs that work with phylogenetic trees in the Newick format

# get_ranked_topology
is used to get ranked tree topology (clades followed by their ranks) given species tree in the Newick format.

# Compile
g++ get_ranked_topology.cpp -o topo 

Program parameters and options:
| Option | Description |
| --- | --- |
| -st file|  Input a tree string in the Newick format through a file|
| -out file| Provide an output file name |

# Examples
./topo -sp tree.txt -out out_tree.txt
Attempt | #1 | #2 | #3 | #4 | #5 | #6 | #7 | #8 | #9 | #10 | #11
--- | --- | --- | --- |--- |--- |--- |--- |--- |--- |--- |---
Seconds | 301 | 283 | 290 | 286 | 289 | 285 | 287 | 287 | 272 | 276 | 269
