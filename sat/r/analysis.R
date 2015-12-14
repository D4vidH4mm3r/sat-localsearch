res <- read.table("../res/mincost-p-initial.csv", header=TRUE)
res$instance <- factor(res$instance)
res$p <- factor(res$p)
res$class <- gsub("(u*f).*","\\1",res$instance)

require(lattice)
bwplot(reorder(p, cost)~cost|class, layout=c(3,1), data=res, scales=list(relation="free"))
