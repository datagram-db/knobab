library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(tidyr)
library(tidyverse)
library(scales)
library(plot3D)
library(latex2exp)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

df <- read.csv(file = '/home/sam/Documents/Repositories/CodeBases/knobab/data/testing/results/benchmarking/results.csv')

df <- aggregate(list(Time = df$Time),
                            by=list(Test = df$Test,Type=df$Type,Log_Size=df$Log_Size,Max_Trace_Length=df$Max_Trace_Length,Alphabet=df$Alphabet),data=df,FUN=mean)

# ggplot(df, aes(x=Log_Size, y=Time, fill=interaction(Max_Trace_Length,Type))) +
#   geom_bar(position = "dodge", stat = "identity") +
#   facet_wrap( ~ Test, nrow=4, ncol=2) +
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

print_chart <- function(df){
  # TODO relabel with latex based on if("choice") df['fast'] <- latex label
  ggplot(df, aes(x=Log_Size, y=Time, color=interaction(Max_Trace_Length,Type))) +
    geom_line(aes(linetype=interaction(Max_Trace_Length,Type))) +
    geom_point(aes(shape=interaction(Max_Trace_Length,Type))) +
    labs(x = "Log Size", y = "Time (ms)") +
    facet_wrap( ~ Test, nrow=2, ncol=2) +
    theme(legend.position="bottom") +
    guides(fill=guide_legend(title="")) +
    scale_y_continuous(trans='log10') +
    scale_x_log10(
      breaks = scales::trans_breaks("log10", function(x) 10^x),
      labels = scales::trans_format("log10", scales::math_format(10^.x)))
}

print_chart(df[df$Test == "And" | df$Test == "Or",])
print_chart(df[df$Test == "Choice",])
print_chart(df[df$Test == "And_Future" | df$Test == "And_Globally" | df$Test == "And_Next_Globally" | df$Test == "Until",])

