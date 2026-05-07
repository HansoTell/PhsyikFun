import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl

mpl.use("TkAgg")


data = np.loadtxt("../build/apps/data.csv", delimiter=",")

entitys = np.unique(data[:,0])

plt.figure(figsize=(10, 6))

for ent in entitys:
    subset = data[data[:, 0] == ent]
    xPos = subset[:, 1]
    yPos = subset[:, 2]
    plt.plot(xPos, yPos)

plt.grid(True)
plt.show()
