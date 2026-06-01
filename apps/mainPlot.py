import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
import pandas as pd

mpl.use("TkAgg")

def PrintTrajek(dataFrame):
    fig = plt.figure()
    ax = fig.add_subplot(projection="3d")

    idx = dataFrame["index"].unique()
    
    for i in idx:
       dfi = dataFrame.where(i == dataFrame["index"])
       xPos = dfi["pos_x"]
       yPos = dfi["pos_y"]
       zPos = dfi["pos_z"]

       ax.plot(xPos,yPos,zPos)

    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("z")
    fig.suptitle("Trajek")

    plt.show()

def PlotData(x_col, y_col, xlabels, ylabels, title = None):
    plt.figure(figsize=(10, 6))

    for (i, lbl) in zip(y_col, ylabels):
        plt.plot(x_col, i, label = lbl )
    plt.xlabel(xlabels)
    plt.ylabel("Value")
    plt.title(title or f"{', '.join(ylabels)} over {xlabels}")
    plt.legend()
    plt.grid(True)
    plt.show()

def Plot3D(firsts, secounds, thirds, labels, title = None):
    pass


def Print(FilePath):
    df = pd.read_csv(FilePath)

    PrintTrajek(df)
    PlotData(df["Time"], [df["Ekin"]], "Time / [s]", ["Ekin"], "Kinetic Energy")
    PlotData(df["Time"], [df["EPot"]], "Time / [s]", ["Epot"], "Potential Energy")
    PlotData(df["Time"], [df["Ekin"]+df["EPot"]], "Time / [s]", ["Eges"], "Energy Plot")



if __name__ == "__main__":
    Print("../build/apps/data.csv")
