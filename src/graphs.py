import networkx as nx
import random

p_in = .55
p_out = .04


def add(probability):
    return random.random() < probability


g = nx.Graph()

community1 = list(range(1, 33))
community2 = list(range(33, 65))
community3 = list(range(65, 97))
community4 = list(range(97, 129))

for vertex1 in community1:
    for vertex2 in community1:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_in):
            g.add_edge(vertex1, vertex2)

for vertex1 in community2:
    for vertex2 in community2:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_in):
            g.add_edge(vertex1, vertex2)

for vertex1 in community3:
    for vertex2 in community3:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_in):
            g.add_edge(vertex1, vertex2)

for vertex1 in community4:
    for vertex2 in community4:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_in):
            g.add_edge(vertex1, vertex2)

outside_communities = community2 + community3 + community4

for vertex1 in community1:
    for vertex2 in outside_communities:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_out):
            g.add_edge(vertex1, vertex2)

outside_communities = community1 + community3 + community4

for vertex1 in community2:
    for vertex2 in outside_communities:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_out):
            g.add_edge(vertex1, vertex2)

outside_communities = community1 + community2 + community4

for vertex1 in community3:
    for vertex2 in outside_communities:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_out):
            g.add_edge(vertex1, vertex2)

outside_communities = community1 + community2 + community3

for vertex1 in community4:
    for vertex2 in outside_communities:
        if vertex2 == vertex1:
            continue
        elif g.has_edge(vertex1, vertex2):
            continue
        elif add(p_out):
            g.add_edge(vertex1, vertex2)


nx.write_graphml(g, "../input/random_graph.graphml")