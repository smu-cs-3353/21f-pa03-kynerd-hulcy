import networkx as nx
import matplotlib.pyplot as plt
import random

p_in = random.uniform(.501, 1)
p_out = 0.5 * (1 - p_in)


def add(probability):
    return random.random() < probability


g = nx.Graph()
# print(str(p_in * 16) + " " + str(p_out * 16))

community1 = list(range(1, 33))
community2 = list(range(33, 65))
community3 = list(range(65, 97))
community4 = list(range(97, 129))

edges = {new_list: [] for new_list in range(1, 129)}
#
# for vertex in community1:

#
#     vertices = edges.get(vertex)
#
#     while len(vertices) < p_in * 16:
#         vertex_to_try = random.choice(community1)
#         while vertex_to_try == vertex:
#             vertex_to_try = random.choice(community1)
#
#         if not g.has_edge(vertex, vertex_to_try):
#             print("Adding edge: " + str(vertex) + ", " + str(vertex_to_try))
#             g.add_edge(vertex, vertex_to_try)
#             vertices.append(vertex_to_try)

num_edges = 0

for vertex1 in community1:
    for vertex2 in community1:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_in):
            num_edges += 1
            g.add_edge(vertex1, vertex2)

print("num_edges = " + str(num_edges))

for vertex1 in community2:
    for vertex2 in community2:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_in):
            num_edges += 1
            g.add_edge(vertex1, vertex2)

for vertex1 in community3:
    for vertex2 in community3:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_in):
            num_edges += 1
            g.add_edge(vertex1, vertex2)

for vertex1 in community4:
    for vertex2 in community4:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_in):
            num_edges += 1
            g.add_edge(vertex1, vertex2)

outside_communities = community2 + community3 + community4

for vertex1 in community1:
    for vertex2 in outside_communities:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_out):
            num_edges += 1
            g.add_edge(vertex1, vertex2)

outside_communities = community1 + community3 + community4

for vertex1 in community2:
    for vertex2 in outside_communities:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_out):
            num_edges += 1
            g.add_edge(vertex1, vertex2)

outside_communities = community1 + community2 + community4

for vertex1 in community3:
    for vertex2 in outside_communities:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_out):
            num_edges += 1
            g.add_edge(vertex1, vertex2)

outside_communities = community1 + community2 + community3

for vertex1 in community4:
    for vertex2 in outside_communities:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_out):
            num_edges += 1
            g.add_edge(vertex1, vertex2)


nx.write_graphml(g, "input.grapml")
# nx.draw(g)
# plt.savefig("testOutput.png")
#
# print("p_in = " + str(p_in) + ", p_out = " + str(p_out))
# print("num_edges = " + str(num_edges))
# print("num_edges / num_vertices = " + str(num_edges / 128))
