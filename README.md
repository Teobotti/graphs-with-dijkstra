# CALCULATE MINIMUM PATHS

This program manages a graph ranking based on "the sum of minimum paths from node 0 and all other nodes of the graph". It takes several graphs in input and gives back the top k graphs.

## How it works:
Firstly, the program takes in input two values:
  - d: the number of nodes in each graph
  - k: the top k graphs to include in the chart
  
 Then takes in input the graph in matrix format:
 
 Aggiungigrafo (command that tells the program to store a new graph)
 
 0 ,  3 ,  4
 
 1 ,  2 ,  5
 
 5 ,  2 ,  1
 
 At the end, the command TopK asks for the best K graphs.
 
 ## Insights
 To proper calculate minimum paths, the best algorithm is Dijkstra for sure. Some improvements were made to allow the algorithm to perform better. It has been tested with millions of graphs, calculating the chart in less than 1.5 seconds. 
 
The algorithm calculates for each graph the sum of minimum paths. Then decides if to insert the graph in the chart. The data structure for the chart is a Max Heap in order to find immediatly (Time complexity: O(1)) the maximum value, if the new graph found is grater than the max, then is discarded.

Other improvemets are to read the input not with the classic function atoi() and strtok(), which are slow functions. Reading the entire line with fgets() and then analyze and calculate the values one by one has revealed a very much faster method.

In dijkstra algorithm, a Min Heap structure is used to store the temporary values of the paths, this also improves overall time complexity of the program. 
