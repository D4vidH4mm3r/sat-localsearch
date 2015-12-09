from subprocess import Popen, PIPE
from itertools import takewhile
from pathlib import Path


datadir = Path("../data")
instance = "../data/uf20-010.cnf"

for instance in sorted(datadir.iterdir()):
    print(instance)
    positivesInClauses = []
    negativesInClauses = []
    with instance.open("r") as fd:
        for line in fd:
            if not line.startswith("c"):
                break
        specs = line.split(" ")
        numLits = int(specs[2])
        numClauses = int(specs[4]) # NOTE: there seems to be two spaces between nl and nc
        print("#literals:", numLits)
        print("#clauses:", numClauses)
        for line in fd:
            line = line.strip()
            if line.startswith("%") or line.startswith("0"):
                break
            vals = line.split(" ")
            posClause = []
            negClause = []
            for val in vals[:-1]: # skip 0 at end
                val = int(val)
                if val>0:
                    posClause.append(val)
                else:
                    negClause.append(-val)
            positivesInClauses.append(posClause)
            negativesInClauses.append(negClause)
    assert len(positivesInClauses) == len(negativesInClauses) == numClauses

    instantiation = []
    p = Popen(["./xyz", "-t", "3", str(instance)], stdout=PIPE)
    inter = p.communicate()
    res = inter[0].decode("utf-8")
    line = res.partition("\n")[0]
    for val in line.split(" ")[1:]:
        instantiation.append(int(val) > 0)
    assert len(instantiation) == numLits

    satisfied = 0
    failed = 0
    for pos, neg in zip(positivesInClauses, negativesInClauses):
        if (any(instantiation[lit-1] for lit in pos) or
                any((not instantiation[lit-1]) for lit in neg)):
            satisfied += 1
        else:
            failed += 1
    print("#satisfied:", satisfied)
    print("#failed:", failed)
    print()
