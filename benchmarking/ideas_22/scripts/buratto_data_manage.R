library(stringr)
library(dplyr)
library(tidyr)
library(ggplot2)
library(scales)
## Aggre per query
data <- read.csv("/home/giacomo/paper_testset/burattin/vs_burattino_final2.csv")
data$total <- data$model_data_decomposition_time + data$model_atomization_time + data$model_declare_to_ltlf + data$model_ltlf_query_time
data <- aggregate(list(query_time=data[, 23]), list(query=data$model_filename, atomization_conf=data$atomization_conf), mean)
toAppend <- read.csv("/home/giacomo/paper_testset/burattin/thorough.csv")
toAppend <- aggregate(list(query_time=toAppend$time), list(query=toAppend$query_id, atomization_conf=toAppend$stategy), mean)

union(data, toAppend) %>%
  ggplot(aes(x = factor(query, level =unique(data$query)[order(nchar(unique(data$query)), unique(data$query))]), fill = atomization_conf, y=query_time)) +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))
  ) +
  guides(fill=guide_legend(title="Solutions"))+
  geom_bar(position="dodge", stat="identity")+ 
  labs(x = "Query", y = "Query Time (ms)") 

## Aggre per model size
data <- read.csv("/home/giacomo/paper_testset/burattin/vs_burattino_final.csv")
data$total <- data$model_data_decomposition_time + data$model_atomization_time + data$model_declare_to_ltlf + data$model_ltlf_query_time
data <- aggregate(list(query_time=data[, 23]), list(model_size=data$model_size, atomization_conf=data$atomization_conf), mean)
toAppend <- read.csv("/home/giacomo/paper_testset/burattin/thorough.csv")
toAppend <- aggregate(list(query_time=toAppend$time), list(model_size=toAppend$model_size, atomization_conf=toAppend$stategy), mean)

U <- union(data, toAppend)



union(data, toAppend) %>%
  ggplot(aes(x = model_size, colour = atomization_conf, y=query_time)) +
  theme(legend.position = 'bottom', legend.title = element_blank()) +
  geom_point() +
  geom_line() +
  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))
  ) + scale_x_log10(
    breaks =unique(U$n_traces),
    labels =unique(U$n_traces)
  ) +
  labs(x = "Query Time (ms)", y = "Model Size") 