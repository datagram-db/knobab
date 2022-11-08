library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(scales)
library(latex2exp)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

knobabdf <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/results/sql.csv')

knobabdf$execution_time <- knobabdf$model_data_decomposition_time + knobabdf$model_atomization_time + knobabdf$model_declare_to_ltlf + knobabdf$model_ltlf_query_time

knobabdf = knobabdf[,c('n_traces', 'model_filename', 'atomization_conf', 'execution_time')]

knobabdf <- aggregate(list(execution_time = knobabdf$execution_time),
                     by=list(atomization_conf = knobabdf$atomization_conf, n_traces=knobabdf$n_traces,model_filename=knobabdf$model_filename),data=knobabdf,FUN=mean)

knobabdf[knobabdf == "scripts/maxsat_pipeline.yaml"] <- "KnoBAB + MAX-SAT [VARIANT-1]"
knobabdf[knobabdf == "scripts/maxsat_pipeline_hybrid.yaml"] <- "KnoBAB + MAX-SAT [HYBRID]"
knobabdf[knobabdf == "scripts/support_pipeline.yaml"] <- "KnoBAB + Support [VARIANT-1]"
knobabdf[knobabdf == "scripts/support_pipeline_hybrid.yaml"] <- "KnoBAB + Support [HYBRID]"

knobabdf[knobabdf == "AltPrecedence"] <- "Alternate Precedence"
knobabdf[knobabdf == "AltResponse"] <- "Alternate Response"
knobabdf[knobabdf == "NegSuccession"] <- "Not Succession"
knobabdf[knobabdf == "ChainPrecedence"] <- "Chain Precedence"
knobabdf[knobabdf == "ChainResponse"] <- "Chain Response"
knobabdf[knobabdf == "RespExistence"] <- "Responded Existence"

sqltimings <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/results/sqlminer.csv')

sqltimings <- sqltimings[sqltimings$failure == 0,]

sqltimings <- aggregate(list(execution_time = sqltimings$execution_time),
                        by=list(n_traces=sqltimings$n_traces,model_filename=sqltimings$model_filename,atomization_conf=sqltimings$atomization_conf),data=sqltimings,FUN=mean)

combineddf <- rbind(knobabdf, sqltimings)
  
ggplot(data=combineddf, aes(x=n_traces, y=execution_time), group=atomization_conf) +
  geom_line(aes(color=atomization_conf)) +
  geom_point(aes(color=atomization_conf)) +
  facet_wrap( ~ model_filename, nrow=2, ncol=4) +
  labs(x = TeX(r'($ |{L}| $)'), y = "Execution Time (ms)") + 
  scale_y_continuous(trans='log10') + 
  scale_x_continuous(trans='log10',
                     breaks=c(10,100,1000)) + 
  theme(legend.position="bottom") + 
  labs(color="")
