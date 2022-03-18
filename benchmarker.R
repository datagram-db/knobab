library(ggplot2)
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))
timings <- read.csv(file = 'test.csv')
timings = timings[,c('query_method', 'log_filename', 'model_filename', 'model_ltlf_query_time')]

p <-ggplot(timings, aes(x=model_filename, y=model_ltlf_query_time, group=query_method)) +
  geom_point(aes(color=query_method)) +
  stat_summary(
    aes(fill=query_method),
    geom = "point",
    fun = "mean",
    col = "black",
    size = 5,
    shape = 24,
  ) +
  labs(x = "Declare Clause", y = "Time (ms)", title=timings$log_filename) +
  theme_minimal() +
  theme(legend.position="right", legend.title = element_text(face="bold"))+
  guides(colour = guide_legend(override.aes = list(shape = 22)))
p
  
# timings <- read.csv(file = 'test2.csv')
# timings = timings[,c('log_filename', 'model_filename', 'model_ltlf_query_time')]
# 
# p <-ggplot(timings, aes(x=model_filename, y=model_ltlf_query_time)) +
#   geom_point(aes(color=query_method)) +
#   stat_summary(
#     aes(fill=query_method),
#     geom = "point",
#     fun = "mean",
#     col = "black",
#     size = 5,
#     shape = 24,
#   ) +
#   labs(x = "Declare Clause", y = "Time (ms)", title=timings$log_filename) +
#   theme_minimal() +
#   theme(legend.position="right", legend.title = element_text(face="bold"))+
#   guides(colour = guide_legend(override.aes = list(shape = 22)))
# p