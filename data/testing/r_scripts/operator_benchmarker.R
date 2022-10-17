library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(tidyr)
library(scales)
library(latex2exp)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

df <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/testing/results/benchmarking/results.csv')

df <- aggregate(list(Time = df$Time),
                            by=list(Test = df$Test,Type=df$Type,Log_Size=df$Log_Size,Max_Trace_Length=df$Max_Trace_Length,Alphabet=df$Alphabet),data=df,FUN=mean)

print_chart <- function(df){
  # TODO relabel with latex based on if("choice") df['fast'] <- latex label
  ggplot(df, aes(x=Log_Size, y=Time, shape=Type, color=Max_Trace_Length)) +
    geom_line() +
    geom_point(size=3) +
    labs(x = "Log Size", y = "Time (ms)") +
    facet_wrap( ~ Test, nrow=2, ncol=2) +
    theme(legend.position="bottom") +
    scale_y_continuous(trans='log10') +
    scale_x_log10(
      breaks = scales::trans_breaks("log10", function(x) 10^x),
      labels = scales::trans_format("log10", scales::math_format(10^.x)))
}

df$Max_Trace_Length <- as.character(df$Max_Trace_Length)
# df$Test[df$Test == "And_Future"] <- TeX("X")
df$Test[df$Test == "And_Future"] <- "A&F(B)"
df$Test[df$Test == "And_Globally"] <- "A&G(B)"
df$Test[df$Test == "Until"] <- "AUB"

df$Type[df$Type == "fast"] <- "OPTIMIZED"
df$Type[df$Type == "fast_old"] <- "OPTIMIZED OLD"
df$Type[df$Type == "logic"] <- "LOGICAL"

print_chart(df[df$Test == "And" | df$Test == "Or",])
print_chart(df[df$Test == "Choice",])
print_chart(df[df$Test == "A&F(B)" | df$Test == "A&G(B)" | df$Test == "AUB",])

# ggplot(df, aes(x=Log_Size, y=Time, fill=interaction(Max_Trace_Length,Type))) +
#   geom_bar(position = "dodge", stat = "identity") +
#   facet_wrap( ~ Test, nrow=2, ncol=2) +
#   labs(x = "Log Size", y = "Time (ms)") +
#   theme(legend.position="bottom") +
#   guides(fill=guide_legend(title="")) +
#   scale_y_continuous(
#     trans = pseudo_log_trans(base = 10),
#     breaks = c(0, 10^(-2:2)),
#     labels = label_number(accuracy = 0.01)
#   ) +
#   scale_x_log10(
#     breaks = scales::trans_breaks("log10", function(x) 10^x),
#     labels = scales::trans_format("log10", scales::math_format(10^.x)))
