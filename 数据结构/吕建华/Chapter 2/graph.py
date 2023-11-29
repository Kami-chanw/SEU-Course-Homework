from tkinter import PROJECTING
import numpy as np
from scipy import interpolate
import pandas as pd

import matplotlib as mpl

import matplotlib.pyplot as plt

import seaborn as sns

import warnings

warnings.filterwarnings(action="once")


large = 22
med = 16
small = 12

params = {
    "axes.titlesize": large,
    "legend.fontsize": med,
    "figure.figsize": (16, 10),
    "axes.labelsize": med,
    "axes.titlesize": med,
    "xtick.labelsize": med,
    "ytick.labelsize": med,
    "figure.titlesize": large,
}

plt.rcParams.update(params)

plt.style.use("seaborn-whitegrid")

sns.set_style("white")


# Version

print(mpl.__version__)  # > 3.0.0

print(sns.__version__)  # > 0.9.0
data = pd.read_csv("data2.csv")

x = list(set(data["row"]))
y = list(set(data["col"]))
z = np.asarray(data["count"])
z = z.reshape((len(y), len(x)))

plt.figure(figsize=(8, 6), dpi=80)
axe = plt.axes(projection="3d")

ix = np.linspace(0, len(x), len(x) * 2)  # x for interpolation
iy = np.linspace(0, len(y), len(y) * 2)
X, Y = np.meshgrid(ix, iy)
f = interpolate.interp2d(x, y, z, kind="cubic")
Z = f(ix, iy)
axe.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap="autumn", antialiased=True)
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
axe.set_xlabel("row")
axe.set_ylabel("col")
axe.set_zlabel("count")
axe.invert_xaxis()
plt.show()
