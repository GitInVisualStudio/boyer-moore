import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as tkr


def format(data):
    p8 = map(lambda x: float(x[2]), filter(lambda x: x[1] == "8", data))
    p8 = list(p8)
    p16 = map(lambda x: float(x[2]), filter(lambda x: x[1] == "16", data))
    p16 = list(p16)
    p32 = map(lambda x: float(x[2]), filter(lambda x: x[1] == "32", data))
    p32 = list(p32)
    p64 = map(lambda x: float(x[2]), filter(lambda x: x[1] == "64", data))
    p64 = list(p32)
    index = map(lambda x: float(x[0]), filter(lambda x: x[1] == "32", data))
    index = list(index)
    return p8, p16, p32, p64, index


def sizeof_fmt(x, pos):
    if x < 0:
        return ""
    for x_unit in ['bytes', 'kB', 'MB', 'GB', 'TB']:
        if x < 1024.0:
            return "%3.1f %s" % (x, x_unit)
        x /= 1024.0


def make_plot(name, data):
    p8, p16, p32, p64, index = format(data)

    df = pd.DataFrame({
        'Pattern Size: 8': p8,
        'Pattern Size: 16': p16,
        'Pattern Size: 32': p32,
        'Pattern Size: 64': p64
    }, index=index)

    ax = df.plot()
    ax.xaxis.set_major_formatter(tkr.FuncFormatter(sizeof_fmt))
    plt.title(name)
    plt.xlabel("Search Text Size")
    plt.ylabel("Time in ms")
    plt.savefig(name + ".png")


def combined(first, second):
    bp8, bp16, bp32, bp64, _ = format(first)
    np8, np16, np32, np64, index = format(second)
    import numpy as np
    baverage = np.array([bp8, bp16, bp32, bp64]).T
    naverage = np.array([np8, np16, np32, np64]).T
    df = pd.DataFrame({
        'Boyer Moore average': [x.mean() for x in baverage],
        'Naive average': [x.mean() for x in naverage],
    }, index=index)

    ax = df.plot()
    ax.xaxis.set_major_formatter(tkr.FuncFormatter(sizeof_fmt))
    plt.title("Boyer Moore")
    plt.xlabel("Search Text Size")
    plt.ylabel("Time in ms")
    plt.savefig("combined-average.png")


boyer_moore = []
with open("boyer_moore.csv", "r") as file:
    boyer_moore = [x.split(';') for x in file.readlines()]

naive = []
with open("naive.csv", "r") as file:
    naive = [x.split(';') for x in file.readlines()]
combined(boyer_moore, naive)
make_plot("Boyer-Moore", boyer_moore)
make_plot("Naive", naive)
