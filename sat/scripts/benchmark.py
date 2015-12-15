from common import *
from argparse import ArgumentParser
from itertools import product
import csv


argparse = ArgumentParser(description="Run a series of tests")
argparse.add_argument("output", help="Name of (csv) file to write to")
argparse.add_argument("instanceclass", help="Prefix of name for test instances to use")
argparse.add_argument("-p", "--param", help="Parameters for search", action="append", nargs="+")
args = argparse.parse_args()

if not resdir.exists():
    resdir.mkdir()
    
fn = (resdir / (args.output + ".csv"))
new = not fn.exists()
paramNames = [p[0] for p in args.param]
paramVals = [p[1:] for p in args.param]
with fn.open("a", newline="") as fd:
    csvwriter = csv.writer(fd, delimiter=" ", quotechar="\"")
    if new:
        csvwriter.writerow(["instance"] + paramNames + ["cost", "time"])
    for instance in sorted(datadir.iterdir()):
        if not instance.name.startswith(args.instanceclass):
            continue
        print(instance.name)
        for comb in product(*paramVals):
            kw = {name: val for name, val in zip(paramNames, comb)}
            print(kw)
            res = runInstance(instance, **kw)
            lines = res.split("\n")
            cost = int(lines[-3].split(" ")[-1])
            time = float(lines[-2].split(" ")[-1])
            csvwriter.writerow([instance.name] + list(comb) + [cost, time])
