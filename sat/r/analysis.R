res <- read.table("../res/mincost-p-initial.csv", header=TRUE)
res$instance <- factor(res$instance)
res$p <- factor(res$p)
res$class <- gsub("(u*f).*","\\1",res$instance)

require(lattice)
bwplot(reorder(p, cost)~cost|class, data=res, scales=list(relation="free"))
bwplot(reorder(p, time)~time|class, data=(res[res$class=="uf",]), scales=list(relation="free"))
