import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.colors import LightSource
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

PATH = "datasets/pend_tree_count_bagging.bb"

with open(PATH, "r") as f:
    lines = f.readlines()
    x_axis_ticks_labels = np.array([float(x) for x in lines[0].replace(" \n", "").split(" ")])
    y_axis_ticks_labels = np.array([float(x) for x in lines[1].replace(" \n", "").split(" ")])

    matrix = []
    for line in lines[2:]:
        line = line.replace(" \n", "")
        matrix.append([float(x) for x in line.split(" ")])

    matrix = np.array(matrix)

    print("x_axis_ticks_labels: ", x_axis_ticks_labels)
    print("y_axis_ticks_labels: ", y_axis_ticks_labels)
    print("matrix: ", matrix)


# plot only v1
"""plt.plot(x_axis_ticks_labels, matrix[0], label="v1")
plt.show()
exit(0)"""

# Plot a heatmap
fig, ax = plt.subplots()
im = ax.imshow(matrix, cmap=cm.coolwarm)

# disable values z on heatmap grid
for i in range(len(y_axis_ticks_labels)):
    for j in range(len(x_axis_ticks_labels)):
        text = ax.text(j, i, "",
                       ha="center", va="center", color="w")

# We want to show all ticks...
ax.set_xticks(np.arange(len(x_axis_ticks_labels)))
ax.set_yticks(np.arange(len(y_axis_ticks_labels)))
# ... and label them with the respective list entries
ax.set_xticklabels(x_axis_ticks_labels)
ax.set_yticklabels(y_axis_ticks_labels)

# Rotate the tick labels and set their alignment.
plt.setp(ax.get_xticklabels(), rotation=45, ha="right",
         rotation_mode="anchor")

# Loop over data dimensions and create text annotations.
for i in range(len(y_axis_ticks_labels)):
    for j in range(len(x_axis_ticks_labels)):
        text = ax.text(j, i, matrix[i, j],
                       ha="center", va="center", color="w")

ax.set_title("Hyperopt")
fig.tight_layout()
plt.show()