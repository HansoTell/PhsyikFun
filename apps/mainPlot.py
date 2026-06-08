import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
import pandas as pd

mpl.use("TkAgg")

def PrintTrajek(dataFrame, options):
    for OptionsPack in options:
        if "Trajektorie" in OptionsPack:
            fig = plt.figure()
            ax = fig.add_subplot(projection="3d")

            idx = dataFrame["index"].unique()
            
            for i in idx:
               dfi = dataFrame[i == dataFrame["index"]]
               xPos = dfi["pos_x"]
               yPos = dfi["pos_y"]
               zPos = dfi["pos_z"]

               ax.plot(xPos,yPos,zPos)

            ax.set_xlabel("x")
            ax.set_ylabel("y")
            ax.set_zlabel("z")
            fig.suptitle("Trajektorie")

# wie könnte man einstellen, dass entweder einzelnd für entitys oder ob für alle entitys bündelt
def PrintEnergie(df:pd.DataFrame, options):
    idx = df["index"].unique()

    for OptionsPack in options:
        if "Eges" in OptionsPack or "EKin" in OptionsPack or "EPot" in OptionsPack:
            plt.figure()
            for i in idx:
                dfi = df[i == df["index"]]
                time = dfi["Time"]
                ekin = dfi["Ekin"]
                epot = dfi["EPot"]
                if "Eges" in OptionsPack:
                    eges = ekin + epot 
                    plt.plot(time, eges, label = f"Eges of Entity = {i}")
                if "EKin" in OptionsPack:
                    plt.plot(time, ekin, label = f"EKin of Entity = {i}")
                if "EPot" in OptionsPack:
                    plt.plot(time, epot, label = f"EPotof Entity = {i}")
            plt.xlabel("Time")
            plt.ylabel("E / [J]")
            plt.title(", ".join(OptionsPack))
            plt.legend()
            plt.grid(True)

def Plot(FilePath, options):
    df = pd.read_csv(FilePath, delimiter=",", dtype="float")
    PrintTrajek(df, options)
    PrintEnergie(df, options)

    plt.show()


if __name__ == "__main__":
    Plot("../build/apps/data.csv", [["Trajektorie"], ["Eges"], ["EKin", "EPot"]])
