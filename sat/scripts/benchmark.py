from common import *
from argparse import ArgumentParser
import csv


argparse = ArgumentParser(description="Run a series of tests")
argparse.add_argument("output", help="Name of (csv) file to write to")
argparse.add_argument("instanceclass", help="Prefix of name for test instances to use")
argparse.add_argument("values", help="The parameter values to test", nargs="+", type=float)
args = argparse.parse_args()

if not resdir.exists():
    resdir.mkdir()
    
fn = (resdir / (args.output + ".csv"))
new = not fn.exists()
with fn.open("a", newline="") as fd:
    csvwriter = csv.writer(fd, delimiter=" ", quotechar="\"")
    if new:
        csvwriter.writerow(["instance", "p", "cost", "time"])
    for instance in sorted(datadir.iterdir()):
        if not instance.name.startswith(args.instanceclass):
            continue
        print(instance.name)
        for p in args.values:
            print(p)
            res = runInstance(instance, p=p)
            lines = res.split("\n")
            cost = int(lines[-3].split(" ")[-1])
            time = float(lines[-2].split(" ")[-1])
            csvwriter.writerow([instance.name, p, cost, time])
