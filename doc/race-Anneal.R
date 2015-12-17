run_and_get_time <- function (instance, param="") {
    instance_path <- paste("../data", instance, sep="/")
    output <- system(paste("../src/xyz", param, instance_path, sep=" "), intern=TRUE)
    cost <- as.numeric(gsub("^c Cost: ", "", output[3]))
    time <- as.numeric(gsub("^c Time: ", "", output[4]))
    return(cost*10+time)
}
race.init <- function () {
    alpha <- c(0.55,0.75,0.95)
    spt <- c(30000,60000)
    T0 <- c(10,20,30,40,50)
    candidates <- expand.grid(alpha,spt,T0)
    instances <- Filter({function (s) grepl("^uf250", s)}, list.files("../data"))
    return(list(no.candidates=nrow(candidates),
                no.tasks=length(instances),
                candidates=candidates,
                tasks=instances))
}
race.wrapper <- function (candidate, task, data) {
    # note that candidate and task are both indices
    # candidate should be value for p
    current <- data$candidates[candidate,]
    return(run_and_get_time(data$tasks[task],
                            param=paste("-ss 1",
                                        "--alpha", current$Var1,
                                        "--spt", current$Var2,
                                        "--T0", current$Var3,
                                        "-t 5")))
}
race.info <- function (data) {
    return(list(race.name="Tuning p",
                no.candidates=data$no.candidates,
                no.tasks=data$no.tasks,
                extra="Try really honestly to succeed :S"))
}
