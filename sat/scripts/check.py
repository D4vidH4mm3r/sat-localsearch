from common import *


for fn in sorted(datadir.iterdir()):
    instance = Instance(fn)
    print(instance)
    output = runInstance(fn, t=3)
    instantiation = []
    lines = output.split("\n")
    for val in lines[0].split(" ")[1:-1]:
        instantiation.append(int(val) > 0)
    assert len(instantiation) == instance.numLits

    numSatisfied = 0
    numFailed = 0
    for pos, neg in zip(instance.positivesInClauses, instance.negativesInClauses):
        if (any(instantiation[lit-1] for lit in pos) or
                any((not instantiation[lit-1]) for lit in neg)):
            numSatisfied += 1
        else:
            numFailed += 1
    time = lines[-2].split(" ")[-1]
    if fn.name.startswith("uf"):
        assert numFailed == 0, "Satisfiable instances should be satisfied"
    print(numFailed, "failed")
