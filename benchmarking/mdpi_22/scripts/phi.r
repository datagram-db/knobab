library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(scales)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

knobabdf <- read.csv(file = '/home/sam/Documents/Repositories/Codebases/knobab/benchmarking/mdpi_22/data/results/phi.csv')

knobabdf$execution_time <- knobabdf$model_data_decomposition_time + knobabdf$model_atomization_time + knobabdf$model_declare_to_ltlf + knobabdf$model_ltlf_query_time

knobabdf = knobabdf[,c('model_filename', 'atomization_type', 'execution_time')]

knobabdf <- aggregate(list(execution_time = knobabdf$execution_time),
                     by=list(model_filename = knobabdf$model_filename, atomization_type=knobabdf$atomization_type),data=knobabdf,FUN=mean)

knobabdf <- as.data.frame(melt(knobabdf, id.vars = c("model_filename", "atomization_type", "execution_time")))

ggplot(data=knobabdf, aes(x=model_filename, y=execution_time, fill=atomization_type)) +
  geom_bar(position = "dodge", stat = "identity") +
  labs(x = "Model", y = "Execution Time (ms)") + 
  theme(legend.position="bottom") + 
  labs(color="Atomization Type") +
  scale_x_discrete(labels=c('M_1' = expression(M[1]), 'M_2' = expression(M[2]),  'M_3' = expression(M[3]), 'M_4' = expression(M[4]))) +
  scale_fill_discrete(name = "Atomization Type",labels = c('AtomizaeEverythingIfDataPredicate' = 'Atomize Everything', 'AtomizeOnlyOnDataPredicates' = "Atomize Only on Data Predicated"))

