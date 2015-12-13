from common import *


print("Instance p cost time")
for instance in sorted(datadir.iterdir()):
    for p in (0.01,0.05,0.1,0.2,0.3,0.4,0.5):
        res = runInstance(instance, t=3, p=p)
        lines = res.split("\n")
        cost = lines[-3].split(" ")[-1]
        time = lines[-2].split(" ")[-1]
        print(instance.name, p, cost, time)
