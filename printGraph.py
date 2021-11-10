import networkx as nx
import matplotlib.pyplot as plt

graph = nx.read_graphml("cmake-build-debug/output.graphml")
nx.draw(graph)
plt.axes([2,2,2,2])
plt.savefig("output.png")