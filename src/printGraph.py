import networkx as nx
import matplotlib.pyplot as plt

graph = nx.read_graphml("../output/output.graphml")
# graph = nx.read_graphml("input/football.graphml")
# plt.clf()
plt.figure(2,figsize=(10,10))

nx.draw(graph, node_size=15)
# c1 = nx.algorithms.clique.cliques_containing_node(graph, list(range(1, 33)))
# print(c1)
# c2 = nx.algorithms.clique.cliques_containing_node(graph, list(range(33, 65)))
# c3 = nx.algorithms.clique.cliques_containing_node(graph, list(range(65, 97)))
# c4 = nx.algorithms.clique.cliques_containing_node(graph, list(range(97, 129)))
# pos = {c1:(0,0), c2:(100,0), c3:(0,100), c4:(100,100)}
# nx.draw()
# plt.savefig("input/football_before.png")
plt.savefig("../output/output.png")