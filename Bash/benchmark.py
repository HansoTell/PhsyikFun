import matplotlib.pyplot as plt
import pandas as pd
import matplotlib as mpl
import numpy as np
from scipy.optimize import curve_fit

mpl.use("TkAgg")

df = pd.read_csv("benchmark.csv", delimiter=",")
num_steps = 10001.062

def lin(n, a, c): return a*n+c
def nlogn(n, a, c): return a*n*np.log(n)+c
def quad(n, a, b, c): return a*n**2+b*n+c
def power(n, a, p, c): return a*n**p+c

models = {
    "a·n + c":          (lin,   [1e-6, 0]),
    "a·n·log(n) + c":   (nlogn, [1e-6, 0]),
    "a·n² + b·n + c":   (quad,  [1e-9, 1e-6, 0]),
    "a·n^p + c":        (power, [1e-6, 1.5, 0]),
}

n_fit = np.linspace(df["n"].min(), df["n"].max(), 400)

plt.figure()
plt.scatter(df["n"], df["Runtime"], label = "Messdaten")
for name, (f, p0) in models.items():
    try:
        popt,_ = curve_fit(f, df["n"], df["Runtime"], p0=p0, maxfev=20000)
    except RuntimeError:
        print(f"{name}: Fit nicht konvergiert")
        continue
    res = df["Runtime"] - f(df["n"], *popt)
    ss_res = np.sum(res**2)
    r2 = 1 - ss_res / np.sum((df["Runtime"] - df["Runtime"].mean())**2)
    k = len(popt)
    aic = len(df["n"]) * np.log(ss_res / len(df["n"])) + 2*k
    print(f"{name:18s} R²={r2:.5f} AIC={aic:.1f} params={popt}")
    plt.plot(n_fit, f(n_fit, *popt), label=f"{name} (R²={r2:.4f})")

plt.legend()
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
plt.scatter(dfCOpy, means, label="SystemDestructorWhole")
plt.scatter(dfCOpy, meansDiffrent, label="SumOfTimes")
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

plt.figure()
StepsPerSecound = num_steps / df["Runtime"]
plt.scatter(df["n"], StepsPerSecound)
plt.title("Steps Per Secound")
plt.grid(True)
plt.xlabel("Entitys")
plt.ylabel("Steps per Secound")

plt.figure()
plt.loglog(df["n"], df["Runtime"], "o")
plt.title("LogLog Runtime Plot")
plt.grid(True)
plt.xlabel("Log(Entitys)")
plt.ylabel("Log(Runtime)")
slope, intercept = np.polyfit(np.log(df["n"]), np.log(df["Runtime"]), 1)
print("Exponent circa", slope)

plt.show()
