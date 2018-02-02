# phylo
programs that work with phylogenetic trees in the Newick format

**get_ranked_topology**
is used to get ranked tree topology (clades followed by their ranks) given species tree in the Newick format.

**calc_anomal.cpp**
is used to calculate number of species trees producing anomalous gene trees and number of anomalous gene trees.

# Compile
g++ get_ranked_topology.cpp -o topo 

Program parameters and options:

| Option        | Description                                               | 
| ------------- |:---------------------------------------------------------:| 
| -st file      | Input a tree string in the Newick format through the file | 
| -out file     | Provide an output file name                               |   


g++ calc_anomal.cpp -o anom

Program parameters and options:

| Option        | Description                                             | 
| ------------- |:-------------------------------------------------------:| 
| -topo file    | Input a species tree topology through the file          | 
| -sort file    | Specify the file name of sorted gene topologies         |
|               | starting from the tree with the highest probability     |   

output files:
num_anomal.txt

num_anomal_binom.txt

# Examples
tree.txt: ((A:0.9032443231,(B:0.4734160308,D:0.4734160308):0.4298282923):0.1284305971,(E:0.6915098761,C:0.6915098761):0.3401650441):0.4492550856;

./topo -sp tree.txt -out out_tree.txt

out.tree.txt: ABD-2-CE-3-BD-4-


