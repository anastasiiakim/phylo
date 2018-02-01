# phylo
programs that work with phylogenetic trees in the Newick format

# get_ranked_topology
is used to get ranked tree topology (clades followed by their ranks) given species tree in the Newick format.

# Compile
g++ get_ranked_topology.cpp -o topo 

Program parameters and options:

| Option        | Description                                             | 
| ------------- |:-------------------------------------------------------:| 
| -st file      | Input a tree string in the Newick format through a file | 
| -out file     | Provide an output file name                             |   


# Examples
tree.txt: ((A:0.9032443231,(B:0.4734160308,D:0.4734160308):0.4298282923):0.1284305971,(E:0.6915098761,C:0.6915098761):0.3401650441):0.4492550856;
./topo -sp tree.txt -out out_tree.txt
out.tree.txt: ABD-2-CE-3-BD-4-


