library(reshape2)
library(ggplot2)
library(latex2exp)
library(extrafont)
library(dplyr)

loadfonts(device="pdf")
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

df_adms <- read.csv(file = '/home/sam/Documents/Repositories/Codebases/ModelMining/real.csv')

names(df_adms)[names(df_adms) == "query_time"] <- "mining_time"
df_adms <- aggregate(cbind(loading_and_indexing_time, mining_time) ~ n_traces + min_support + mining_algorithm, data = df_adms, FUN = mean)

df_knobab <- read.csv(file = '/home/sam/Documents/Repositories/Codebases/knobab/benchmarking/ideas_23/data/results/real.csv')

df_knobab_bolt <- df_knobab[df_knobab$mining_algorithm == 'BOLT', ]
df_knobab_bolt$mining_time <- df_knobab_bolt$model_ltlf_query_time + df_knobab_bolt$model_data_decomposition_time + df_knobab_bolt$model_atomization_time + df_knobab_bolt$model_declare_to_ltlf
df_knobab_bolt$loading_and_indexing_time <- df_knobab_bolt$log_loading_and_parsing_ms + df_knobab_bolt$log_indexing_ms

df_knobab_bolt <- aggregate(cbind(loading_and_indexing_time, mining_time) ~ n_traces + min_support + mining_algorithm, data = df_knobab_bolt, FUN = mean)

df_knobab_batches <- df_knobab[df_knobab$mining_algorithm != 'BOLT', ]

df_knobab_batches_loading <- df_knobab_batches[df_knobab_batches$model_ltlf_query_time == -1, ]
df_knobab_batches_loading$loading_and_indexing_time <- df_knobab_batches_loading$log_loading_and_parsing_ms + df_knobab_batches_loading$log_indexing_ms
df_knobab_batches_loading <- df_knobab_batches_loading[,c("iteration_num","n_traces","min_support","mining_algorithm","loading_and_indexing_time")]

df_knobab_batches_querying<- df_knobab_batches[df_knobab_batches$model_ltlf_query_time != -1, ]
df_knobab_batches_querying$mining_time <- df_knobab_batches_querying$model_ltlf_query_time + df_knobab_batches_querying$model_data_decomposition_time + df_knobab_batches_querying$model_atomization_time + df_knobab_batches_querying$model_declare_to_ltlf
df_knobab_batches_querying <- aggregate(mining_time ~ iteration_num + n_traces + min_support + mining_algorithm, data = df_knobab_batches_querying, FUN = sum)

df_knobab_batches <-merge(df_knobab_batches_loading, df_knobab_batches_querying)
df_knobab_batches <- aggregate(cbind(loading_and_indexing_time, mining_time) ~ n_traces + min_support + mining_algorithm, data = df_knobab_batches, FUN = mean)

df_merge <- rbind(df_adms, df_knobab_bolt, df_knobab_batches)

scientific_10 <- function(x) {
  parse(text=gsub("e", "%*% 10^", scales::scientific_format()(x/10)))
}

df_merge$mining_algorithm <- factor(df_merge$mining_algorithm, levels = c("BOLT", "PREVIOUS_MINING", "APRIORI", "ADM_S"))


df_merge$mining_time <- df_merge$mining_time * 10
df_merge$loading_and_indexing_time <- df_merge$loading_and_indexing_time * 10
df_merge$execution_time <- (df_merge$loading_and_indexing_time + df_merge$mining_time)
df_merge$min_support <- factor(df_merge$min_support)

df_merge$n_traces <- factor(df_merge$n_traces,
                       labels= c(expression("|" ~ L ~ "|" == 1%*%10^1),
                                 expression("|" ~ L ~ "|" == 1%*%10^2),
                                 expression("|" ~ L ~ "|" == 1%*%10^3),
                                 expression("|" ~ L ~ "|" == 1%*%10^4),
                                 expression("|" ~ L ~ "|" == 1%*%10^5)))

df_stack <- df_merge
df_stack$execution_time <- NULL;
df_stack <- melt(df_stack, id.vars=c('n_traces', 'min_support', 'mining_algorithm'))
names(df_stack)[names(df_stack) == "variable"] <- "Time"

df_stack$Time <- factor(df_stack$Time,
                            labels= c("Loading + Indexing",
                                      "Mining"))

df_stack <- df_stack %>% group_by(n_traces, min_support, mining_algorithm)
df_stack <- df_stack %>% mutate(cum_tot = cumsum(value))

ggplot(data = df_stack, aes(min_support, cum_tot, fill=mining_algorithm)) +
  geom_col(data = . %>% filter(Time=="Loading + Indexing"), position = position_dodge(width = 0.9), alpha = 1) +
  geom_col(data = . %>% filter(Time=="Mining"), position = position_dodge(width = 0.9), alpha = 0.3) +
  geom_tile(aes(y=NA_integer_, alpha = Time)) +
  scale_alpha_manual(values = c(1,0.3), labels = c(expression(lambda), expression(mu))) +
  scale_y_log10(labels = scientific_10, breaks=sapply(10, function(v) v**(-2:6))) +
  labs(x = expression(theta), y = expression(lambda + mu  ~ "(ms)"), fill="Algorithm", group="test") + 
  theme(legend.position="bottom", text = element_text(family = "Linux Libertine")) + 
  scale_fill_discrete(labels=c("Bolt", "TopN Declare", "ADM", "ADM + S")) +
  facet_wrap( ~ n_traces, nrow=1, ncol=5, labeller=label_parsed)

# df_plot <- ggplot(data=df_merge, aes(x=min_support, y=execution_time*10, fill=mining_algorithm)) +
#   geom_col(position = "dodge", stat="identity") +
#   labs(x = expression(theta), y = expression(lambda + mu  ~ "(ms)"), fill="Algorithm") +
#   scale_y_log10(labels = scientific_10, breaks=sapply(10, function(v) v**(-2:6))) +
#   theme(legend.position="bottom", text = element_text(family = "Linux Libertine")) +
#   scale_fill_discrete(labels=c("Bolt", "TopN Declare", "ADM", "ADM + S")) +
#   facet_wrap( ~ n_traces, nrow=1, ncol=5, labeller=label_parsed)

# df_plot
# ggsave("benchmark_real.pdf", df_plot, width=16, height=6, device=cairo_pdf)

