from common import *
import csv
import sys


print(sys.argv[1:])
fn = (resdir / "mincost-p-initial.csv")
new = not fn.exists()
with fn.open("a", newline="") as fd:
    csvwriter = csv.writer(fd, delimiter=" ", quotechar="\"")
    if new:
        csvwriter.writerow(["instance", "p", "cost", "time"])
    for instance in sorted(datadir.iterdir()):
        for p in sys.argv[1:]:
            p = float(p)
            res = runInstance(instance, t=3, p=p)
            lines = res.split("\n")
            cost = int(lines[-3].split(" ")[-1])
            time = float(lines[-2].split(" ")[-1])
            csvwriter.writerow([instance.name, p, cost, time])
