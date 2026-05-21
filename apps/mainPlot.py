import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
import pandas as pd

mpl.use("TkAgg")


data = np.loadtxt("../build/apps/data.csv", delimiter=",")

def PrintTrajek(fileName):
    df = pd.read_csv("")
    indexes = df["index"].unique()
    EntitysDfs = []
    plt.figure() #muss 3d sein
    for i in indexes:
        EntitysDfs.append(df.where(df["index"] == i))


entitys = np.unique(data[:,0])

plt.figure(figsize=(10, 6))

for ent in entitys:
    subset = data[data[:, 0] == ent]
    xPos = subset[:, 1]
    yPos = subset[:, 2]
    plt.plot(xPos, yPos)

plt.grid(True)
plt.show()
