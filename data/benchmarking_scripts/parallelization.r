library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(scales)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

knobabdf <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/testing/results/benchmarking/parallel.csv')

knobabdf = knobabdf[,c('scheduling_strategy', 'no_threads', 'model_filename', 'model_ltlf_query_time')]

knobabdf <- aggregate(list(model_ltlf_query_time = knobabdf$model_ltlf_query_time),
                     by=list(scheduling_strategy = knobabdf$scheduling_strategy, no_threads=knobabdf$no_threads,model_filename=knobabdf$model_filename),data=knobabdf,FUN=mean)

knobabdf <- as.data.frame(melt(knobabdf, id.vars = c("scheduling_strategy", "no_threads", "model_filename","model_ltlf_query_time")))
  
singledf <- knobabdf[knobabdf$no_threads == 1, ]

knobabdf <- knobabdf[knobabdf$no_threads != 1, ]

# knobabdf$speed_up <- singledf$model_ltlf_query_time / knobabdf$model_ltlf_query_time

ggplot(data=knobabdf, aes(x=no_threads, y=model_ltlf_query_time), group=model_filename) +
  geom_line(aes(color=scheduling_strategy)) +
  geom_point(aes(color=scheduling_strategy)) +
  facet_wrap( ~ model_filename, nrow=2, ncol=4) +
  labs(y = "Time (ms)") +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))) +
  theme(legend.position="bottom") +
  labs(color="") + 
  geom_hline(data = singledf, aes(yintercept = model_ltlf_query_time), color="red", linetype="dashed")

# ggplot(data=knobabdf, aes(x=no_threads, y=speed_up), group=scheduling_strategy) +
#   geom_line(aes(color=scheduling_strategy)) +
#   geom_point(aes(color=scheduling_strategy)) +
#   facet_wrap( ~ model_filename, nrow=2, ncol=4) +
#   labs(y = "Time (ms)") +
#   scale_y_log10(
#     breaks = scales::trans_breaks("log10", function(x) 10^x),
#     labels = scales::trans_format("log10", scales::math_format(10^.x))) +
#   theme(legend.position="bottom") +
#   labs(color="")


