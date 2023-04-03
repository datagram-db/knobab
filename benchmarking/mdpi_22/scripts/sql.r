library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(scales)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

knobabdf <- read.csv(file = '/home/sam/Documents/Repositories/Codebases/knobab/benchmarking/mdpi_22/data/results/sql.csv')

knobabdf$execution_time <- knobabdf$model_data_decomposition_time + knobabdf$model_atomization_time + knobabdf$model_declare_to_ltlf + knobabdf$model_ltlf_query_time

knobabdf = knobabdf[,c('n_traces', 'model_filename', 'atomization_conf', 'execution_time')]

knobabdf <- aggregate(list(execution_time = knobabdf$execution_time),
                     by=list(atomization_conf = knobabdf$atomization_conf, n_traces=knobabdf$n_traces,model_filename=knobabdf$model_filename),data=knobabdf,FUN=mean)

knobabdf <- as.data.frame(melt(knobabdf, id.vars = c("n_traces", "model_filename", "atomization_conf","execution_time")))

sqltimings <- read.csv(file = '/home/sam/Documents/Repositories/Codebases/knobab/benchmarking/mdpi_22/data/results/sqlminer.csv')

sqltimings <- aggregate(list(execution_time = sqltimings$execution_time),
                        by=list(n_traces=sqltimings$n_traces,model_filename=sqltimings$model_filename,atomization_conf=sqltimings$atomization_conf,failure=sqltimings$failure),data=sqltimings,FUN=mean)

validsqltimings <- subset(sqltimings, select = c(atomization_conf,n_traces,model_filename,execution_time), failure==0)

validsqltimings$failure <- NULL

sqlsuccessdf <- as.data.frame(melt(validsqltimings, id.vars = c("n_traces", "model_filename", "atomization_conf","execution_time")))

combineddf <- rbind(knobabdf, sqlsuccessdf)
  
ggplot(data=combineddf, aes(x=n_traces, y=execution_time), group=atomization_conf) +
  geom_line(aes(color=atomization_conf)) +
  geom_point(aes(color=atomization_conf)) +
  facet_wrap( ~ model_filename, nrow=2, ncol=4) +
  labs(x = "Log Size", y = "Execution Time (ms)") + 
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))) +
  scale_x_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))) +
  theme(legend.position="bottom") + 
  labs(color="") + 
  scale_color_discrete(labels = c('scripts/maxsat_pipeline_hybrid.yaml' = 'KnoBAB + MAX-SAT [HYBRID]', 'scripts/maxsat_pipeline.yaml' = "KnoBAB + MAX-SAT [VARIANT-1]", "scripts/support_pipeline_hybrid.yaml" = "KnoBAB + Support [HYBRID]", "scripts/support_pipeline.yaml" = "KnoBAB + Support [VARIANT-1]"))
