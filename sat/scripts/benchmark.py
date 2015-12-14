from common import *
import csv
import sys


if not resdir.exists():
    resdir.mkdir()
    
fn = (resdir / "tune-p-unsatisfiable.csv")
new = not fn.exists()
with fn.open("a", newline="") as fd:
    csvwriter = csv.writer(fd, delimiter=" ", quotechar="\"")
    if new:
        csvwriter.writerow(["instance", "p", "cost", "time"])
    for instance in sorted(datadir.iterdir()):
        if not instance.name.startswith("uuf250"):
            print("Skipping", instance.name, "for now")
            continue
        print(instance.name)
        for p in sys.argv[1:]:
            print(p)
            p = float(p)
            res = runInstance(instance, p=p, g=1)
            lines = res.split("\n")
            cost = int(lines[-3].split(" ")[-1])
            time = float(lines[-2].split(" ")[-1])
            csvwriter.writerow([instance.name, p, cost, time])
