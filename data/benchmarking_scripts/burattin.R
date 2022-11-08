library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(tidyr)
library(latex2exp)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

knobabdf <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/results/burattin.csv')

knobabdf$execution_time <- knobabdf$model_data_decomposition_time + knobabdf$model_atomization_time + knobabdf$model_declare_to_ltlf + knobabdf$model_ltlf_query_time

knobabdf = knobabdf[,c('model_filename', 'atomization_conf', 'execution_time', 'model_type')]

knobabdf <- aggregate(list(execution_time = knobabdf$execution_time),
                     by=list(atomization_conf = knobabdf$atomization_conf,model_filename=knobabdf$model_filename,model_type=knobabdf$model_type),data=knobabdf,FUN=mean)

knobabdf[knobabdf == "scripts/intersection_pipeline.yaml"] <- "KnoBAB + CQ"
knobabdf[knobabdf == "scripts/support_pipeline.yaml"] <- "KnoBAB + Support"

burattindf <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/results/declare_analyzer.csv')

burattindf <- aggregate(list(execution_time = burattindf$execution_time),
                        by=list(model_filename=burattindf$model_filename,atomization_conf=burattindf$atomization_conf,model_type=burattindf$model_type),data=burattindf,FUN=mean)

combineddf <- rbind(knobabdf, burattindf)  

scenario1df <- combineddf[combineddf$model_type == "SCENARIO_1", ]
scenario2df <- combineddf[combineddf$model_type == "SCENARIO_2", ]

scenario1df$atomization_conf <- factor(scenario1df$atomization_conf, levels=c("KnoBAB + CQ", "KnoBAB + Support", "Declare Analyzer"))
scenario2df$atomization_conf <- factor(scenario2df$atomization_conf, levels=c("KnoBAB + CQ", "KnoBAB + Support", "Declare Analyzer"))

ggplot(scenario1df, aes(x=reorder(model_filename, execution_time), y=execution_time, fill=atomization_conf)) +
  geom_bar(position = "dodge", stat = "identity") +
  labs(x = "Model", y = "Time (ms)") + 
  theme(legend.position="bottom") + 
  scale_x_discrete(labels=c("data/bpic_2012/models/scenario_1/5.powerdecl" = TeX(r'($M_1$)'), 
                            "data/bpic_2012/models/scenario_1/10.powerdecl" = TeX(r'($M_2$)'), 
                            "data/bpic_2012/models/scenario_1/15.powerdecl" = TeX(r'($M_3$)'), 
                            "data/bpic_2012/models/scenario_1/20.powerdecl" = TeX(r'($M_4$)'))) +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))) + 
  guides(fill=guide_legend(title="Atomization Type"))

yticks <- c(1, 10, 100, 10000)

# scenario2df$mean_t <- trans(scenario2df$execution_time)

scenario2df$model_filename <- factor(scenario2df$model_filename, levels=c("data/bpic_2012/models/scenario_2/q1.powerdecl",
                                                                          "data/bpic_2012/models/scenario_2/q2.powerdecl",
                                                                          "data/bpic_2012/models/scenario_2/q3.powerdecl",
                                                                          "data/bpic_2012/models/scenario_2/q4.powerdecl",
                                                                          "data/bpic_2012/models/scenario_2/q5.powerdecl",
                                                                          "data/bpic_2012/models/scenario_2/q1^q2.powerdecl",
                                                                          "data/bpic_2012/models/scenario_2/q1^q2^q4.powerdecl",
                                                                          "data/bpic_2012/models/scenario_2/q1^q3^q4.powerdecl",
                                                                          "data/bpic_2012/models/scenario_2/q1^q2^q5.powerdecl",
                                                                          "data/bpic_2012/models/scenario_2/q1^q3^q5.powerdecl",
                                                                          "data/bpic_2012/models/scenario_2/q1^q2^q3^q4^q5.powerdecl"))

ggplot(scenario2df, aes(x=model_filename, y=execution_time, fill=atomization_conf)) +
  geom_col(position = "dodge") +
  guides(fill=guide_legend(title="")) +
  labs(x = TeX(r'($M$)'), y = "Execution Time (ms)") +
  theme(legend.position="bottom") +
  scale_x_discrete(labels=c("data/bpic_2012/models/scenario_2/q1.powerdecl" = TeX(r'($M_1$)'), 
                            "data/bpic_2012/models/scenario_2/q2.powerdecl" = TeX(r'($M_2$)'), 
                            "data/bpic_2012/models/scenario_2/q3.powerdecl" = TeX(r'($M_3$)'), 
                            "data/bpic_2012/models/scenario_2/q4.powerdecl" = TeX(r'($M_4$)'),
                            "data/bpic_2012/models/scenario_2/q5.powerdecl" = TeX(r'($M_5$)'),
                            "data/bpic_2012/models/scenario_2/q1^q2.powerdecl" = TeX(r'($M_6$)'),
                            "data/bpic_2012/models/scenario_2/q1^q2^q4.powerdecl" = TeX(r'($M_7$)'),
                            "data/bpic_2012/models/scenario_2/q1^q3^q4.powerdecl" = TeX(r'($M_8$)'),
                            "data/bpic_2012/models/scenario_2/q1^q2^q5.powerdecl" = TeX(r'($M_9$)'),
                            "data/bpic_2012/models/scenario_2/q1^q3^q5.powerdecl" = TeX(r'($M_{10}$)'),
                            "data/bpic_2012/models/scenario_2/q1^q2^q3^q4^q5.powerdecl" = TeX(r'($M_{11}$)'))) +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))) +
  guides(fill=guide_legend(title="Atomization Type"))
