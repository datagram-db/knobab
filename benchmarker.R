library(ggplot2,reshape)
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

timings <- read.csv(file = 'test.csv')
timings = timings[,c('log_filename', 'model_filename', 'model_ltlf_query_time', 'sqlminer_query_time')]
dat_l <- melt(timings, id.vars = c("log_filename", "model_filename"))
dat_2 <- aggregate(dat_l[, c("value")], list(dat_l$log_filename,dat_l$model_filename,dat_l$variable), mean)

names(dat_2) = c("log_filename", "model_filename", "variable", "value")

p <- ggplot(data = dat_2, aes(x = model_filename, y = value, group = variable, fill = variable)) +
  geom_bar(stat = "identity", width = 0.1, position = "dodge") + 
  facet_grid(. ~ log_filename) +
  theme_bw() +
  labs(x = "Declare Clause", y = "Time (ms)") + 
  theme(axis.text.x = element_text(angle = 90)) 
p
