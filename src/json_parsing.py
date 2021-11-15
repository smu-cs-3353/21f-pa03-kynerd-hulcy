import json
import networkx as nx
import itertools
import matplotlib.pyplot as plt

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

# print(playlists)

for playlist in playlists:
    for i in range(len(playlist)):
        artist1 = playlist[i]
        for j in range(i, len(playlist)):
            artist2 = playlist[j]
            g.add_edge(artist1, artist2)


nx.draw(g)
plt.show()