library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(scales)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

knobabdf <- read.csv(file = '/home/giacomo/projects/knobab/data/testing/results/benchmarking/knobab_sql2.csv')
#knobabdf <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/testing/results/benchmarking/knobab_sql.csv')

knobabdf$execution_time <- knobabdf$model_data_decomposition_time + knobabdf$model_atomization_time + knobabdf$model_declare_to_ltlf + knobabdf$model_ltlf_query_time

knobabdf = knobabdf[,c('n_traces', 'model_filename', 'atomization_conf', 'execution_time')]

knobabdf <- aggregate(list(execution_time = knobabdf$execution_time),
                     by=list(atomization_conf = knobabdf$atomization_conf, n_traces=knobabdf$n_traces,model_filename=knobabdf$model_filename),data=knobabdf,FUN=mean)

knobabdf <- as.data.frame(melt(knobabdf, id.vars = c("n_traces", "model_filename", "atomization_conf","execution_time")))

#sqltimings <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/testing/results/benchmarking/sqlminer.csv')
sqltimings <- read.csv(file = '/home/giacomo/projects/knobab/data/testing/results/benchmarking/sqlminer.csv')


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
  labs(x = "Log Size", y = "Time (ms)") + 
  scale_y_continuous(trans='log10') + 
  scale_x_continuous(trans='log10',
                     breaks=c(10,100,1000)) + 
  theme(legend.position="bottom") + 
  labs(color="")
