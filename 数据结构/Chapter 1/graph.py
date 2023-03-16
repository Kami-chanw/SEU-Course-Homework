import numpy as np

import pandas as pd

import matplotlib as mpl

import matplotlib.pyplot as plt

import seaborn as sns

import warnings
warnings.filterwarnings(action='once')


large = 22
med = 16
small = 12

params = {'axes.titlesize': large,

          'legend.fontsize': med,

          'figure.figsize': (16, 10),

          'axes.labelsize': med,

          'axes.titlesize': med,

          'xtick.labelsize': med,

          'ytick.labelsize': med,

          'figure.titlesize': large}

plt.rcParams.update(params)

plt.style.use('seaborn-whitegrid')

sns.set_style("white")


# Version

print(mpl.__version__)  # > 3.0.0

print(sns.__version__)  # > 0.9.0

midwest = pd.read_csv("data.csv")


# Decorations

plt.figure(figsize=(8, 6), dpi=80)

x = midwest["n"]
y = midwest["runTime(ms)"]

plt.plot(x, y)
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
plt.xlabel("n"); plt.ylabel("run time(ms)")
plt.title("Average Run Time of Binary Search Algorithm", fontsize=22)

plt.legend(fontsize=12)

plt.show()
