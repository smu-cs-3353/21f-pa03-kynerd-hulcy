import random
import csv

genres = []
with open("../input/spotify_genres.csv", 'r') as spotify_genres: # reads file into lists for checking genre
    reader = csv.reader(spotify_genres)
    for genre in reader:
        genres.append(list(genre))

print("Enter your favorite artist: ")
artist = str(input())

random_artist = ''
for genre in genres: # searches through genres to find given artist's genre and returns random artist of same genre
    if artist in genre:
        random_artist = genre[random.randint(0, len(genre))-1]
        while artist == random_artist:
            random_artist = genre[random.randint(0, len(genre))-1]
        break

if random_artist != '':
    print("Listen to: " + random_artist)
else:
    print("no related artist found")