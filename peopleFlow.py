import networkx as nx
import matplotlib.pyplot as plt

# Assuming your data is stored in a .dat file named 'data.dat'
file_path = 'main.dat'

# Read the .dat file and create a directed graph
G = nx.DiGraph()

with open(file_path, 'r') as datfile:
    for line in datfile:
        row = line.strip().split()  # Assuming space-separated values, adjust as needed
        
        person_id = int(row[0])
        
        for idx, value in enumerate(row[1:]):
            if int(value) == 1:
                G.add_edge(person_id, idx)

# Draw the graph
pos = nx.spring_layout(G)  # You can choose a different layout as well
nx.draw(G, pos, with_labels=True, node_size=700, node_color="skyblue", font_size=8, font_color="black", font_weight="bold", arrowsize=10)

# Show the graph
plt.show()
