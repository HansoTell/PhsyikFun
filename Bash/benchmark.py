import matplotlib.pyplot as plt
import pandas as pd
import matplotlib as mpl

mpl.use("TkAgg")

df = pd.read_csv("benchmark.csv", delimiter=",")

plt.figure()
plt.scatter(df["n"], df["Runtime"])
plt.title("Runtime")
plt.xlabel("Entitys")
plt.ylabel("Runtime/[s]")
plt.grid(True)

plt.figure()
means = []
meansDiffrent = []
dfCOpy = df
dfCOpy = dfCOpy["n"].drop_duplicates()
for i in (10, 20, 50, 100, 200, 500):
    dfi = df[i == df["n"]]
    means.append(dfi["Runtime"].mean())
    meansDiffrent.append(dfi["SystemStep"].mean() + dfi["SystemPrint"].mean())
plt.plot(dfCOpy, means, label="SystemDestructorWhole")
plt.plot(dfCOpy, meansDiffrent, label="SumOfTimes")
plt.title("Runtime Time Mean")
plt.legend()
plt.grid(True)
plt.xlabel("Entitys")
plt.ylabel("Time/[s]")

plt.figure()
plt.scatter(df["n"], df["SystemStep"], label="StepTime")
plt.scatter(df["n"], df["SystemPrint"], label="PrintTime")
plt.title("System Methods Time")
plt.legend()
plt.grid(True)
plt.xlabel("Entitys")
plt.ylabel("Time/[s]")

plt.figure()
plt.scatter(df["n"], df["CoreStep"], label="StepTime")
plt.scatter(df["n"], df["CoreCollision"], label="CollisionTime")
plt.scatter(df["n"], df["CoreRest"], label="RestTime")
plt.title("Core Methods Time")
plt.legend()
plt.grid(True)
plt.xlabel("Entitys")
plt.ylabel("Time/[s]")

plt.figure()
meansCoreStep = []
meansCoreCollision= []
meansCoreRest = []
for i in (10, 20, 50, 100, 200, 500):
    dfi = df[i == df["n"]]
    meansCoreStep.append(dfi["CoreStep"].mean())
    meansCoreCollision.append(dfi["CoreCollision"].mean())
    meansCoreRest.append(dfi["CoreRest"].mean())
plt.plot(dfCOpy, meansCoreStep, label="MeanStepTime")
plt.plot(dfCOpy, meansCoreCollision, label="MeanCollisionTime")
plt.plot(dfCOpy, meansCoreRest, label="MeanRestTime")

plt.legend()
plt.title("Core Means")
plt.grid(True)
plt.xlabel("Entitys")
plt.ylabel("Time/[s]")


plt.show()
