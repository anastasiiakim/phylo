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



| Tables        | Are           | Cool  |
| ------------- |:-------------:| -----:|
| col 3 is      | right-aligned | $1600 |
| col 2 is      | centered      |   $12 |
| zebra stripes | are neat      |    $1 |
