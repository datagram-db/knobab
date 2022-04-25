library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(tidyr)
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

timings <- read.csv(file = 'C:/Users/Sam/Documents/Repositories/Codebases/knobab/data/testing/results/knobab_burattin_benchmark.csv')
timings$execution_time <- timings$model_data_decomposition_time + timings$model_atomization_time + timings$model_declare_to_ltlf + timings$model_ltlf_query_time
timings = timings[,c('model_size', 'model_filename', 'atomization_conf', 'execution_time')]

timings <- aggregate(list(execution_time = timings$execution_time),
                     by=list(atomization_conf = timings$atomization_conf, model_size=timings$model_size,model_filename=timings$model_filename),data=timings,FUN=mean)

knobabdf <- as.data.frame(melt(timings, id.vars = c("model_size", "model_filename", "atomization_conf","execution_time")))

burattintimings <- read.csv(file = 'C:/Users/Sam/Documents/Repositories/Codebases/knobab/data/testing/results/burattin_benchmark.csv')

burattintimings <- aggregate(list(execution_time = burattintimings$execution_time),
                        by=list(model_size=burattintimings$model_size,atomization_conf=burattintimings$atomization_conf),data=burattintimings,FUN=mean)

burattindf <- as.data.frame(melt(burattintimings, id.vars = c("model_size", "atomization_conf", "execution_time")))

knobabdf$model_filename <- NULL

rbind(knobabdf, burattindf)  %>%
ggplot( aes(x=model_size, y=execution_time, fill=atomization_conf)) +
  geom_bar(position = "dodge", stat = "identity") +
  labs(x = "Model Size", y = "Time (ms)") + 
  theme(legend.position="bottom") + 
  guides(fill=guide_legend(title="")) +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))
  )
  