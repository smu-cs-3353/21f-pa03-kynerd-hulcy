import json
import networkx as nx
import matplotlib.pyplot as plt
from networkx.algorithms.community.centrality import girvan_newman

g = nx.Graph()

newdata = []

with open("/Users/kynerd/Desktop/Algorithms/21f-pa03-kynerd-hulcy/input/spotify_million_playlist_dataset_challenge/challenge_set.json") as f:

    data = json.load(f)
    playlists = data["playlists"] # extracts playlists
    for dictionary in playlists:
        if dictionary["num_samples"] == 100: # only looks at playlists with 100 songs
            newdata.append(dictionary)


playlists = []
for dictionary in newdata:
    tracks = dictionary["tracks"] # extracts tracks from playlist
    artists_in_playlist = set()
    for track in tracks:
        artists_in_playlist.add(track["artist_name"]) # add's track's artist to set
    artist_list = list(artists_in_playlist)
    playlists.append(artist_list)

playlists_alt = []
for i in range(1000): # number of playlists to be checked
    playlists_alt.append(playlists[i])

number_of_playlists = {}

for playlist in playlists_alt:
    for i in range(len(playlist)): # checks every artist
        artist1 = playlist[i]
        for j in range(i, len(playlist)): # adds edge for every artist past original artist
            artist2 = playlist[j]
            if artist1 != artist2:
                if  artist1 > artist2:
                    if (artist2, artist1) in number_of_playlists:
                        if number_of_playlists[(artist2, artist1)] > 5 and not g.has_edge(artist2, artist1): # only adds edge if artists appear together in 5 playlists
                            g.add_edge(artist2, artist1, weight=1) # starts w initial weight 1
                        else:
                            number_of_playlists[(artist2, artist1)] += 1
                            if g.has_edge(artist2, artist1):
                                g[artist2][artist1]['weight'] /= 2 # divides weight by 2 for all subsequent playlist appearances
                    else:
                        number_of_playlists[(artist2, artist1)] = 1
                else:
                    if (artist1, artist2) in number_of_playlists:
                        if number_of_playlists[(artist1, artist2)] > 5 and not g.has_edge(artist1, artist2):
                            g.add_edge(artist1, artist2, weight=1)
                        else:
                            number_of_playlists[(artist1, artist2)] += 1
                            if g.has_edge(artist1, artist2):
                                g[artist1][artist2]['weight'] /= 2
                    else:
                        number_of_playlists[(artist1, artist2)] = 1


communities = girvan_newman(g) # calculates different genre communities

node_groups = []
with open("spotify_genres.csv", 'w') as output_file:
    for com in next(communities):
        node_groups.append(list(com))
        output_file.write(str(list(com)))
        output_file.write("\n")
