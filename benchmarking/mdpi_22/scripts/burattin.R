library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(tidyr)
library(ggforce)
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

knobabdf <- read.csv(file = '/home/sam/Documents/Repositories/Codebases/knobab/benchmarking/mdpi_22/data/results/burattin.csv')

knobabdf$execution_time <- knobabdf$model_data_decomposition_time + knobabdf$model_atomization_time + knobabdf$model_declare_to_ltlf + knobabdf$model_ltlf_query_time

knobabdf = knobabdf[,c('model_filename', 'atomization_conf', 'execution_time', 'model_type')]

knobabdf <- aggregate(list(execution_time = knobabdf$execution_time),
                     by=list(atomization_conf = knobabdf$atomization_conf,model_filename=knobabdf$model_filename,model_type=knobabdf$model_type),data=knobabdf,FUN=mean)

knobabdf <- as.data.frame(melt(knobabdf, id.vars = c("model_filename", "atomization_conf","execution_time", "model_type")))

burattindf <- read.csv(file = '/home/sam/Documents/Repositories/Codebases/knobab/benchmarking/mdpi_22/data/results/declare_analyzer.csv')

burattindf <- aggregate(list(execution_time = burattindf$execution_time),
                        by=list(model_filename=burattindf$model_filename,atomization_conf=burattindf$atomization_conf,model_type=burattindf$model_type),data=burattindf,FUN=mean)

burattindf <- as.data.frame(melt(burattindf, id.vars = c("model_filename", "atomization_conf", "execution_time", "model_type")))

combineddf <- rbind(knobabdf, burattindf)
combineddf$atomization_conf <- factor(combineddf$atomization_conf, levels = c("KnoBAB + CQ", "KnoBAB + Support", "Declare Analyzer"))

scenario1df <- combineddf[combineddf$model_type == "SCENARIO_1", ]

ggplot(scenario1df, aes(x=model_filename, y=execution_time, fill=atomization_conf)) +
  geom_bar(position = "dodge", stat = "identity") +
  labs(x = "Model", y = "Execution Time (ms)") +
  theme(legend.position="bottom") +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))) +
  scale_x_discrete(labels=c('M_1' = expression(M[1]), 'M_2' = expression(M[2]),  'M_3' = expression(M[3]), 'M_4' = expression(M[4]))) + 
  scale_fill_discrete(name = "Atomization Type")

scenario2df <- combineddf[combineddf$model_type == "SCENARIO_2", ]
scenario2df$model_filename <- factor(scenario2df$model_filename, levels=c("q1", "q2", "q3", "q4", "q5", "q1^q2", "q1^q2^q4", "q1^q3^q4", "q1^q2^q5", "q1^q3^q5", "q1^q2^q3^q4^q5"))

trans <- function(x){pmin(x,50) + 0.0005*pmax(x-50,0)}
yticks <- c(0, 25, 50, 9000, 12000)
scenario2df$execution_time <- trans(scenario2df$execution_time)

ggplot(scenario2df, aes(x=model_filename, y=execution_time, fill=atomization_conf)) +
  geom_bar(position = "dodge", stat = "identity") +
  labs(x = "Model", y = "Execution Time (ms)") +
  theme(legend.position="bottom") +
  scale_y_continuous(limits=c(0,NA), breaks=trans(yticks), labels=yticks) +
  scale_x_discrete(labels=c('q1' = expression(M[1]), 'q2' = expression(M[2]),  'q3' = expression(M[3]), 'q4' = expression(M[4]), 'q5' = expression(M[5]), 'q1^q2' = expression(M[6]), 'q1^q2^q4' = expression(M[7]), 'q1^q3^q4' = expression(M[8]), 'q1^q2^q5' = expression(M[9]), 'q1^q3^q5' = expression(M[10]), 'q1^q2^q3^q4^q5' = expression(M[11]))) +
  geom_rect(aes(xmin=0, xmax=12, ymin=max(subset(scenario2df, atomization_conf != 'Declare Analyzer')$execution_time, na.rm = TRUE) + 5, ymax=max(subset(scenario2df, atomization_conf != 'Declare Analyzer')$execution_time, na.rm = TRUE) + 6), fill="white") +
  scale_fill_discrete(name = "Atomization Type")
