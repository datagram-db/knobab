library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(tidyr)
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

knoababtimings <- read.csv(file = '/media/sam/0E68C35868C33CE9/Users/sambu/Documents/Repositories/CodeBases/knobab/data/testing/results/knobab_burattin_benchmark.csv')

knoababtimings$execution_time <- knoababtimings$model_data_decomposition_time + knoababtimings$model_atomization_time + knoababtimings$model_declare_to_ltlf + knoababtimings$model_ltlf_query_time

knoababtimings = knoababtimings[,c('model_filename', 'atomization_conf', 'execution_time')]

knoababtimings <- aggregate(list(execution_time = knoababtimings$execution_time),
                     by=list(atomization_conf = knoababtimings$atomization_conf,model_filename=knoababtimings$model_filename),data=knoababtimings,FUN=mean)

knobabdf <- as.data.frame(melt(knoababtimings, id.vars = c("model_filename", "atomization_conf","execution_time")))

burattintimings <- read.csv(file = '/media/sam/0E68C35868C33CE9/Users/sambu/Documents/Repositories/CodeBases/knobab/data/testing/results/burattin_benchmark.csv')

burattintimings <- aggregate(list(execution_time = burattintimings$execution_time),
                        by=list(model_filename=burattintimings$model_filename,atomization_conf=burattintimings$atomization_conf),data=burattintimings,FUN=mean)

burattindf <- as.data.frame(melt(burattintimings, id.vars = c("model_filename", "atomization_conf", "execution_time")))

combineddf <- rbind(knobabdf, burattindf)  

combineddf <- combineddf[order(combineddf[,1], combineddf[,3]), ]

combineddf$atomization_conf <- factor(combineddf$atomization_conf, levels=c("KnoBAB + CQ", "KnoBAB + Support", "Declare Analyzer"))

ggplot(combineddf, aes(x=model_filename, y=execution_time, fill=atomization_conf)) +
  geom_bar(position = "dodge", stat = "identity") +
  labs(x = "Model", y = "Time (ms)") + 
  theme(legend.position="bottom") + 
  guides(fill=guide_legend(title="")) +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x)))
