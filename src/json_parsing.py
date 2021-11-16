import json
import networkx as nx
import itertools
import matplotlib.pyplot as plt
from networkx.algorithms.community.centrality import girvan_newman

g = nx.Graph()

newdata = []

with open("/Users/kynerd/Desktop/Algorithms/21f-pa03-kynerd-hulcy/input/spotify_million_playlist_dataset_challenge/challenge_set.json") as f:

    data = json.load(f)
    playlists = data["playlists"]
    for dictionary in playlists:
        if dictionary["num_samples"] == 100:
            newdata.append(dictionary)


playlists = []
for dictionary in newdata:
    tracks = dictionary["tracks"]
    artists_in_playlist = set()
    for track in tracks:
        artists_in_playlist.add(track["artist_name"])
    artist_list = list(artists_in_playlist)
    playlists.append(artist_list)

playlists_alt = []
for i in range(40): # number of playlists to be checked
    playlists_alt.append(playlists[i])

number_of_playlists = {}

for playlist in playlists_alt:
    for i in range(len(playlist)):
        artist1 = playlist[i]
        for j in range(i, len(playlist)):
            artist2 = playlist[j]
            if artist1 != artist2:
                if artist1 > artist2:
                    if (artist2, artist1) in number_of_playlists:
                        if number_of_playlists[(artist2, artist1)] > 3 and not g.has_edge(artist2, artist1):
                            g.add_edge(artist2, artist1, weight=3)
                        else:
                            number_of_playlists[(artist2, artist1)] += 1
                    else:
                        number_of_playlists[(artist2, artist1)] = 1
                else:
                    if (artist1, artist2) in number_of_playlists:
                        if number_of_playlists[(artist1, artist2)] > 3 and not g.has_edge(artist1, artist2):
                            g.add_edge(artist1, artist2, weight=3)
                        else:
                            number_of_playlists[(artist1, artist2)] += 1
                    else:
                        number_of_playlists[(artist1, artist2)] = 1
            # if g.has_edge(artist1, artist2):
            #     g[artist1][artist2]['weight'] += 1
            # elif artist1 != artist2:
            #     g.add_edge(artist1, artist2, weight=1)


# print(g.number_of_edges())
# for edge in g.edges:
#     if g.get_edge_data(*edge)["weight"] < 2:
#         g.remove_edge(*edge)
#
# print(g.number_of_edges())

print("nodes: " + str(g.number_of_nodes()) + ", " + "edges: " + str(g.number_of_edges()))
# nx.draw(g, node_size=2)
# plt.show()
# nx.write_graphml(g, "../output/spotify_data.graphml")

communities = girvan_newman(g)

node_groups = []
with open("communities_output.txt", 'w') as output_file:
    for com in next(communities):
        node_groups.append(list(com))
        output_file.write(str(list(com)))
        output_file.write("\n")
    # output_file.write(str(node_groups))

print(node_groups)
