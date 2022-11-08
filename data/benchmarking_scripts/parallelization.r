library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(scales)
library(latex2exp)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

df <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/results/parallel.csv')

df = df[,c('scheduling_strategy', 'no_threads', 'model_filename', 'model_ltlf_query_time')]

df <- aggregate(list(model_ltlf_query_time = df$model_ltlf_query_time),
                     by=list(scheduling_strategy = df$scheduling_strategy, no_threads=df$no_threads,model_filename=df$model_filename),data=df,FUN=mean)

df[df == "AltPrecedence"] <- "Alternate Precedence"
df[df == "AltResponse"] <- "Alternate Response"
df[df == "NegSuccession"] <- "Not Succession"
df[df == "ChainPrecedence"] <- "Chain Precedence"
df[df == "ChainResponse"] <- "Chain Response"
df[df == "RespExistence"] <- "Responded Existence"

df[df == "BLOCK_STATIC_SCHEDULE"] <- "BLOCKED STATIC"
df[df == "CYCLIC_STATIC_SCHEDULE"] <- "BLOCK-CYCLIC STATIC"
df[df == "GUIDED_SCHEDULE"] <- "GUIDED SCHEDULE"
df[df == "MONOTONIC_DYNAMIC_SCHEDULE"] <- "MONOTONIC:DYNAMIC"
df[df == "TASK_SIZE_PREDICTION_BLOCK_STATIC"] <- "TASK SIZE PREDICTION BLOCKED STATIC"
df[df == "TASK_SIZE_PREDICTION_UNBALANCED_DYNAMIC"] <- "TASK SIZE PREDICTION UNBALANCED DYNAMIC"

singledf <- df[df$no_threads == 1, ]
df <- df[df$no_threads != 1, ]

# df$speed_up <- singledf$model_ltlf_query_time / df$model_ltlf_query_time

ggplot(data=df, aes(x=no_threads, y=model_ltlf_query_time), group=model_filename) +
  geom_line(aes(color=scheduling_strategy)) +
  geom_point(aes(color=scheduling_strategy)) +
  facet_wrap( ~ model_filename, nrow=2, ncol=4) +
  labs(x = TeX(r'($ |\omega| $)'), y = "Execution Time (ms)") +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))) +
  theme(legend.position="bottom") +
  labs(color="") + 
  geom_hline(data = singledf, aes(yintercept = model_ltlf_query_time), color="red", linetype="dashed")

# ggplot(data=df, aes(x=no_threads, y=speed_up), group=scheduling_strategy) +
#   geom_line(aes(color=scheduling_strategy)) +
#   geom_point(aes(color=scheduling_strategy)) +
#   facet_wrap( ~ model_filename, nrow=2, ncol=4) +
#   labs(y = "Time (ms)") +
#   scale_y_log10(
#     breaks = scales::trans_breaks("log10", function(x) 10^x),
#     labels = scales::trans_format("log10", scales::math_format(10^.x))) +
#   theme(legend.position="bottom") +
#   labs(color="")


