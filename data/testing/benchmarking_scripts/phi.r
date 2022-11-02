library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(scales)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

knobabdf <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/testing/results/benchmarking/phi.csv')

knobabdf$execution_time <- knobabdf$model_data_decomposition_time + knobabdf$model_atomization_time + knobabdf$model_declare_to_ltlf + knobabdf$model_ltlf_query_time

knobabdf = knobabdf[,c('model_filename', 'atomization_type', 'execution_time')]

knobabdf <- aggregate(list(execution_time = knobabdf$execution_time),
                     by=list(model_filename = knobabdf$model_filename, atomization_type=knobabdf$atomization_type),data=knobabdf,FUN=mean)

knobabdf <- as.data.frame(melt(knobabdf, id.vars = c("model_filename", "atomization_type", "execution_time")))
  
ggplot(data=knobabdf, aes(x=model_filename, y=execution_time, fill=atomization_type)) +
  geom_bar(position = "dodge", stat = "identity") +
  labs(x = "Model", y = "Time (ms)") + 
  theme(legend.position="bottom") + 
  labs(color="") +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x)))
