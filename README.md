## Programming Assignment 03
####Maddie Hulcy and Jay Kynerd

Problem: _given a graph, implement the Girvan-Newman algorithm to determine communities within the graph_

We implemented the Girvan-Newman algorithm according to the pseudocode provided in their paper, “Community structure in social and biological networks".
First, we calculated edge betweenness centrality for every edge in the graph using the Brandes algorithm. Following this algorithm, we implemented a breadth-first-search to determine the distance and number of shortest paths from each vertex to all other vertices in the graph. Then, the vertices were visited in reverse order from leaves to source by popping off a stack, and dependencies and centrality betweenness were accumulated.
Once edge betweenness centrality was calculated for each edge in the graph, edges with the highest betweenness centrality were removed until distinct communities were formed. 

