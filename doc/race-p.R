run_and_get_time <- function (instance, param="") {
    instance_path <- paste("../data", instance, sep="/")
    output <- system(paste("../src/xyz", param, instance_path, sep=" "), intern=TRUE)
    return(as.numeric(gsub("^c Time: ", "", output[4])))
}
race.init <- function () {
    candidates <- 0.45+c(0:20)/100
    instances <- Filter({function (s) grepl("^uf250", s)}, list.files("../data"))
    return(list(no.candidates=length(candidates),
                no.tasks=length(instances),
                candidates=candidates,
                tasks=instances))
}
race.wrapper <- function (candidate, task, data) {
    # note that candidate and task are both indices
    # candidate should be value for p
    return(run_and_get_time(data$tasks[task],
                            param=paste("-p", data$candidates[candidate])))
}
race.info <- function (data) {
    return(list(race.name="Tuning p",
                no.candidates=data$no.candidates,
                no.tasks=data$no.tasks,
                extra="Try really honestly to succeed :S"))
}
