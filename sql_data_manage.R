library(stringr)
library(dplyr)
library(tidyr)
library(ggplot2)
library(scales)
data <- read.csv("/home/giacomo/projects/knobab/mining_run.csv")
data$total <- data$model_data_decomposition_time + data$model_atomization_time + data$model_declare_to_ltlf + data$model_ltlf_query_time
data <- aggregate(list(query_time=data[, 23]), list(n_traces=data$n_traces, model_filename=data$model_filename, atomization_conf=data$atomization_conf), mean)
data[data$model_filename == "NegSuccession", c("model_filename")] <- "NotSuccession"


toAppend <- read.csv("/home/giacomo/projects/knobab/competitors/SQLMinerBenchmarker/ht23.csv")
#toAppend <- read.csv("/home/giacomo/projects/knobab/competitors/SQLMinerBenchmarker/hospital_timings.csv")
toAppend$model_filename <- toAppend$model_filename %>% str_replace("25", "")
toAppend[toAppend$model_filename == "RespondedExistence", c("model_filename")] <- "RespExistence"
toAppend <- aggregate(list(query_time=toAppend$query_time), list(n_traces=toAppend$n_traces, model_filename=toAppend$model_filename, atomization_conf=toAppend$atomization_conf), mean)

U <- union(data, toAppend)

union(data, toAppend) %>%
  ggplot(aes(x = n_traces, colour = atomization_conf, y=query_time)) +
  theme(legend.position = 'bottom', legend.title = element_blank()) +
  facet_wrap(vars(model_filename), ncol = 4) +
  geom_point() +
  geom_line() +
  labs(x = "Log Size", y = "Query Time (ms)") +
  scale_x_log10(
    breaks =unique(U$n_traces),
    labels =unique(U$n_traces)
  ) +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))
  ) 