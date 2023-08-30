import numpy as np
from scipy.cluster.hierarchy import dendrogram, linkage
import scipy.spatial.distance as ssd
import matplotlib.pyplot as plt

distances = np.array([[0, 12, 6, 2, 3, 1],
                      [12, 0, 8, 7, 6, 8],
                      [6, 8, 0, 9, 2, 20],
                      [2, 7, 9, 0, 7, 6],
                      [3, 6, 2, 7, 0, 2],
                      [1, 8, 20, 6, 2, 0]])
distances = ssd.squareform(distances)
Z = linkage(distances, method='average')

plt.figure(figsize=(10, 5))
dendrogram(Z, labels=['A', 'B', 'C', 'D', 'E', 'F'])
plt.title('Hierarchical Clustering Dendrogram (Average Linkage)')
plt.xlabel('Sample index')
plt.ylabel('Distance')
plt.show()
