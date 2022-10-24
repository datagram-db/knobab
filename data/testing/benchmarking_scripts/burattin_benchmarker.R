library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(tidyr)
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

knobabdf <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/testing/results/benchmarking/burattin.csv')

knobabdf$execution_time <- knobabdf$model_data_decomposition_time + knobabdf$model_atomization_time + knobabdf$model_declare_to_ltlf + knobabdf$model_ltlf_query_time

knobabdf = knobabdf[,c('model_filename', 'atomization_conf', 'execution_time', 'model_type')]

knobabdf <- aggregate(list(execution_time = knobabdf$execution_time),
                     by=list(atomization_conf = knobabdf$atomization_conf,model_filename=knobabdf$model_filename,model_type=knobabdf$model_type),data=knobabdf,FUN=mean)

knobabdf <- as.data.frame(melt(knobabdf, id.vars = c("model_filename", "atomization_conf","execution_time", "model_type")))

burattindf <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/testing/results/benchmarking/declare_analyzer.csv')

burattindf <- aggregate(list(execution_time = burattindf$execution_time),
                        by=list(model_filename=burattindf$model_filename,atomization_conf=burattindf$atomization_conf,model_type=burattindf$model_type),data=burattindf,FUN=mean)

burattindf <- as.data.frame(melt(burattindf, id.vars = c("model_filename", "atomization_conf", "execution_time", "model_type")))

combineddf <- rbind(knobabdf, burattindf)  

scenario1df <- combineddf[combineddf$model_type == "SCENARIO_1", ]

scenario2df <- combineddf[combineddf$model_type == "SCENARIO_2", ]
# combineddf <- combineddf[order(combineddf[,1], combineddf[,3]), ]
# 
# combineddf$atomization_conf <- factor(combineddf$atomization_conf, levels=c("KnoBAB + CQ", "KnoBAB + Support", "Declare Analyzer"))

scenario2df$model_filename <- factor(scenario2df$model_filename, levels=c("q1", "q2", "q3", "q4", "q5", "q1^q2", "q1^q2^q4", "q1^q3^q4", "q1^q2^q5", "q1^q3^q5", "q1^q2^q3^q4^q5"))


ggplot(scenario1df, aes(x=model_filename, y=execution_time, fill=atomization_conf)) +
  geom_bar(position = "dodge", stat = "identity") +
  facet_wrap( ~ model_type, nrow=2, ncol=4) +
  labs(x = "Model", y = "Time (ms)") + 
  theme(legend.position="bottom") + 
  guides(fill=guide_legend(title="")) +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x)))

ggplot(scenario2df, aes(x=model_filename, y=execution_time, fill=atomization_conf)) +
  geom_bar(position = "dodge", stat = "identity") +
  facet_wrap( ~ model_type, nrow=2, ncol=4) +
  labs(x = "Model", y = "Time (ms)") + 
  theme(legend.position="bottom") + 
  guides(fill=guide_legend(title="")) +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x)))
