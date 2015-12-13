from subprocess import Popen, PIPE
from pathlib import Path


top = Path("..")
datadir = top / "data"
srcdir = top / "src"
resdir = top / "res"
main = str(srcdir / "xyz")

class Instance():
    def __init__(I, fn):
        I.positivesInClauses = []
        I.negativesInClauses = []
        I.name = fn.name
        with fn.open("r") as fd:
            for line in fd:
                if not line.startswith("c"):
                    break
            specs = line.split(" ")
            I.numLits = int(specs[2])
            # NOTE: there seems to be two spaces between nl and nc
            I.numClauses = int(specs[4])
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
                I.positivesInClauses.append(posClause)
                I.negativesInClauses.append(negClause)
        assert len(I.positivesInClauses) == len(I.negativesInClauses) == I.numClauses

    def __str__(I):
        return "{} ({} literals, {} clauses)".format(I.name, I.numLits, I.numClauses)

def runInstance(fn, *args, **kwargs):
    arglist = [main, str(fn)] + list(args)
    for k, v in kwargs.items():
        arglist.append("-" + k)
        arglist.append(str(v))
    p = Popen(arglist, stdout=PIPE)
    inter = p.communicate()
    return inter[0].decode("utf-8")
