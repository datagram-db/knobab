library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(scales)
library(ggbreak) 
library(patchwork)
library(latex2exp)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

df <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/results/phi.csv')

df$execution_time <- df$model_data_decomposition_time + df$model_atomization_time + df$model_declare_to_ltlf + df$model_ltlf_query_time

df = df[,c('model_filename', 'atomization_type', 'execution_time')]

df <- aggregate(list(execution_time = df$execution_time),
                     by=list(model_filename = df$model_filename, atomization_type=df$atomization_type),data=df,FUN=mean)

df[df == "AtomizeEverythingIfAnyDataPredicate"] <- "Atomize Everything"
df[df == "AtomizeOnlyOnDataPredicates"] <- "Atomize Only on Data Predicate"

ggplot(data=df, aes(x=reorder(model_filename, execution_time), y=execution_time, fill=atomization_type)) +
  geom_bar(position = "dodge", stat = "identity") +
  labs(x = TeX(r'($M$)'), y = "Execution Time (ms)") + 
  theme(legend.position="bottom") + 
  labs(color="") +
  scale_x_discrete(labels=c("data/bpic_2012/models/scenario_1/5.powerdecl" = TeX(r'($M_1$)'), 
                            "data/bpic_2012/models/scenario_1/10.powerdecl" = TeX(r'($M_2$)'), 
                            "data/bpic_2012/models/scenario_1/15.powerdecl" = TeX(r'($M_3$)'), 
                            "data/bpic_2012/models/scenario_1/20.powerdecl" = TeX(r'($M_4$)'))) +
  guides(fill=guide_legend(title="Atomization Type"))
  
