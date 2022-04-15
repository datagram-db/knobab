data <- read.csv("/home/giacomo/projects/knobab/parall.csv")
data$total <- data$model_data_decomposition_time + data$model_atomization_time + data$model_declare_to_ltlf + data$model_ltlf_query_time
data <- aggregate(data[, 23], list(threads=data$no_threads, model=data$model_filename, size=data$model_size), mean)
baseline <- data[data$threads==1, ]
speedup <- data[data$threads>1, ]
names(baseline) <- c("threads", "model", "size", "base")
names(speedup) <- c("threads", "model", "size", "speedup")
baseline$threads <- NULL
speedup <- merge(baseline, speedup, all=FALSE)
speedup$speedup <- speedup$base / speedup$speedup
speedup$base <- NULL