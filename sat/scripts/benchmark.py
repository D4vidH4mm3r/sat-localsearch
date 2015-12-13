from common import *


for instance in sorted(datadir.iterdir()):
    print(instance)
    res = runInstance(instance, t=3, o=(resdir/instance.name))
