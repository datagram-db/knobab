library(ggplot2)
library(reshape2)
library(reshape)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

timings <- read.csv(file = 'C:/Users/Sam/Documents/Repositories/Codebases/knobab/data/testing/results/knobab_sql_benchmark.csv')
timings = timings[,c('n_traces', 'model_filename', 'atomization_conf', 'model_ltlf_query_time')]

timings <- aggregate(list(model_ltlf_query_time = timings$model_ltlf_query_time),
                     by=list(atomization_conf = timings$atomization_conf, n_traces=timings$n_traces,model_filename=timings$model_filename),data=timings,FUN=mean)

knobabdf <- melt(timings, id.vars = c("n_traces", "model_filename", "atomization_conf","model_ltlf_query_time"))

sqltimings <- read.csv(file = 'C:/Users/Sam/Documents/Repositories/Codebases/knobab/data/testing/results/sql_benchmark.csv')

sqltimings <- aggregate(list(sql_execution_time = sqltimings$sql_execution_time),
                        by=list(n_traces=sqltimings$n_traces,model_filename=sqltimings$model_filename,query_type=sqltimings$query_type,failure=sqltimings$failure),data=sqltimings,FUN=mean)

validsqltimings <- subset(sqltimings, failure=="FALSE")

sqlsuccessdf <- melt(validsqltimings, id.vars = c("n_traces", "model_filename", "query_type","sql_execution_time", "failure"))

invalidsqltimings <- subset(sqltimings, failure=="TRUE")

sqlfaileddf <- melt(invalidsqltimings, id.vars = c("n_traces", "model_filename", "query_type","sql_execution_time", "failure"))

ggplot(sqlsuccessdf, aes(x=n_traces, y=sql_execution_time), group=query_type) +
  geom_line(aes(color=query_type)) +
  geom_point(aes(color=query_type)) +
  geom_line(data = knobabdf, aes(x=n_traces, y = model_ltlf_query_time, color=atomization_conf)) +
  geom_point(data = knobabdf, aes(x = n_traces, y = model_ltlf_query_time, color=atomization_conf)) +
  facet_wrap( ~ model_filename, nrow=2, ncol=4) +
  labs(x = "Log Size", y = "Time (ms)") + 
  scale_y_continuous(trans='log10') + 
  scale_x_continuous(trans='log10') + 
  theme(legend.position="bottom") + 
  labs(color="")