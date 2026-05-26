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
       zPos = dfi["pox_z"]

       ax.plot(xPos,yPos,zPos)

    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("z")
    fig.suptitle("Trajek")

    plt.show()


def PrintEnergy(fig):
    pass


def Print(FilePath):
    df = pd.read_csv(FilePath)
    fig = plt.figure()

    PrintTrajek(df)
    PrintEnergy(fig)


Print("../build/apps/data.csv")


