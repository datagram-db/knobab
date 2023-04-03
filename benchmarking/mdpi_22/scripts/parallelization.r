library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(scales)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

knobabdf <- read.csv(file = '/home/sam/Documents/Repositories/Codebases/knobab/benchmarking/mdpi_22/data/results/parallelization.csv')

knobabdf = knobabdf[,c('scheduling_strategy', 'no_threads', 'model_filename', 'model_ltlf_query_time')]

knobabdf <- aggregate(list(model_ltlf_query_time = knobabdf$model_ltlf_query_time),
                     by=list(scheduling_strategy = knobabdf$scheduling_strategy, no_threads=knobabdf$no_threads,model_filename=knobabdf$model_filename),data=knobabdf,FUN=mean)

knobabdf <- as.data.frame(melt(knobabdf, id.vars = c("scheduling_strategy", "no_threads", "model_filename","model_ltlf_query_time")))

knobabdf$model_filename <- gsub("\\bAltPrecedence\\b", "Alternate Precedence", knobabdf$model_filename)
knobabdf$model_filename <- gsub("\\bAltResponse\\b", "Alternate Response", knobabdf$model_filename)
knobabdf$model_filename <- gsub("\\bChainPrecedence\\b", "Chain Precedence", knobabdf$model_filename)
knobabdf$model_filename <- gsub("\\bChainResponse\\b", "Chain Response", knobabdf$model_filename)
knobabdf$model_filename <- gsub("\\bNegSuccession\\b", "Not Succession", knobabdf$model_filename)
knobabdf$model_filename <- gsub("\\bRespExistence\\b", "Responded Existence", knobabdf$model_filename)

singledf <- knobabdf[knobabdf$no_threads == 1, ]

knobabdf <- knobabdf[knobabdf$no_threads != 1, ]

knobabdf$scheduling_strategy <- factor(knobabdf$scheduling_strategy, levels = c("CYCLIC_STATIC_SCHEDULE", "BLOCK_STATIC_SCHEDULE", "GUIDED_SCHEDULE", "MONOTONIC_DYNAMIC_SCHEDULE", "TASK_SIZE_PREDICTION_BLOCK_STATIC", "TASK_SIZE_PREDICTION_UNBALANCED_DYNAMIC"))

ggplot(data=knobabdf, aes(x=no_threads, y=model_ltlf_query_time), group=model_filename) +
  geom_line(aes(color=scheduling_strategy)) +
  geom_point(aes(color=scheduling_strategy)) +
  facet_wrap( ~ model_filename, nrow=2, ncol=4) +
  labs(y = "Execution Time (ms)", x = expression("|" ~ omega ~ "|")) +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))) +
  theme(legend.position="bottom") +
  geom_hline(data = singledf, aes(yintercept = model_ltlf_query_time), color="red", linetype="dashed") + 
  scale_color_discrete(labels = c('BLOCK_STATIC_SCHEDULE' = 'BLOCKED STATIC', 'CYCLIC_STATIC_SCHEDULE' = "BLOCK-CYCLIC STATIC", "GUIDED_SCHEDULE" = "GUIDED SCHEDULE", "MONOTONIC_DYNAMIC_SCHEDULE" = "MONOTONIC:DYNAMIC", "TASK_SIZE_PREDICTION_BLOCK_STATIC" = "TASK SIZE PREDICTION BLOCKED STATIC", "TASK_SIZE_PREDICTION_UNBALANCED_DYNAMIC" = "TASK SIZE PREDICTION UNBALANCED DYNAMIC"))

