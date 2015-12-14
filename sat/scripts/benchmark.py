from common import *
import csv


with (resdir / "mincost-p-initial.csv").open("a") as fd:
    csvwriter = csv.writer(fd, delimiter=" ", quotechar="\"")
    for instance in sorted(datadir.iterdir()):
        for p in (0.6,0.7,0.8,0.9,1.0):
            res = runInstance(instance, t=3, p=p)
            lines = res.split("\n")
            cost = int(lines[-3].split(" ")[-1])
            time = float(lines[-2].split(" ")[-1])
            csvwriter.writerow([instance.name, p, cost, time])
