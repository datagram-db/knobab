library(ggplot2)
library(reshape2)
library(reshape)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

knoababtimings <- read.csv(file = 'C:/Users/Sam/Documents/Repositories/Codebases/knobab/data/testing/results/knobab_sql_benchmark.csv')

knoababtimings$execution_time <- knoababtimings$model_data_decomposition_time + knoababtimings$model_atomization_time + knoababtimings$model_declare_to_ltlf + knoababtimings$model_ltlf_query_time

knoababtimings = knoababtimings[,c('n_traces', 'model_filename', 'atomization_conf', 'execution_time')]

knoababtimings <- aggregate(list(execution_time = knoababtimings$execution_time),
                     by=list(atomization_conf = knoababtimings$atomization_conf, n_traces=knoababtimings$n_traces,model_filename=knoababtimings$model_filename),data=knoababtimings,FUN=mean)

knobabdf <- as.data.frame(melt(knoababtimings, id.vars = c("n_traces", "model_filename", "atomization_conf","execution_time")))

sqltimings <- read.csv(file = 'C:/Users/Sam/Documents/Repositories/Codebases/knobab/data/testing/results/sql_benchmark.csv')

sqltimings <- aggregate(list(execution_time = sqltimings$execution_time),
                        by=list(n_traces=sqltimings$n_traces,model_filename=sqltimings$model_filename,atomization_conf=sqltimings$atomization_conf,failure=sqltimings$failure),data=sqltimings,FUN=mean)

validsqltimings <- subset(sqltimings, select = c(atomization_conf,n_traces,model_filename,execution_time), failure==0)

validsqltimings$failure <- NULL

sqlsuccessdf <- as.data.frame(melt(validsqltimings, id.vars = c("n_traces", "model_filename", "atomization_conf","execution_time")))

rbind(knobabdf, sqlsuccessdf) %>%
ggplot(aes(x=n_traces, y=execution_time), group=atomization_conf) +
  geom_line(aes(color=atomization_conf)) +
  geom_point(aes(color=atomization_conf)) +
  facet_wrap( ~ model_filename, nrow=2, ncol=4) +
  labs(x = "Log Size", y = "Time (ms)") + 
  scale_y_continuous(trans='log10') + 
  scale_x_continuous(trans='log10') + 
  theme(legend.position="bottom") + 
  labs(color="")
