library(ggplot2)
library(reshape2)
library(reshape)
library(dplyr)
library(tidyr)
library(scales)
library(latex2exp)

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

df <- read.csv(file = '/home/sam/Documents/Repositories/Codebases/knobab/benchmarking/mdpi_22/data/results/custom_operators.csv')

df <- aggregate(list(Time = df$Time),
                by=list(Test = df$Test,Type=df$Type,Log_Size=df$Log_Size,Max_Trace_Length=df$Max_Trace_Length,Alphabet=df$Alphabet),data=df,FUN=mean)

print_chart <- function(df, new_labs){
  ggplot(df, aes(x=Log_Size, y=Time, shape=Type, color=Max_Trace_Length)) +
    geom_line() +
    geom_point(size=3) +
    labs(x = TeX(r'($ |{L}| $)'), y = "Execution Time (ms)") +
    facet_wrap( ~ Test, nrow=2, ncol=2) +
    theme(legend.position="bottom") +
    scale_color_discrete(name = TeX(r'($\epsilon$)')) +
    scale_shape_discrete(name = "Algorithm", labels = new_labs) + 
    scale_y_log10(
      breaks = scales::trans_breaks("log10", function(x) 10^x),
      labels = scales::trans_format("log10", scales::math_format(10^.x))) +
  scale_x_log10(
      breaks = scales::trans_breaks("log10", function(x) 10^x),
      labels = scales::trans_format("log10", scales::math_format(10^.x))) + 
    guides(colour = guide_legend(order = 1), shape = guide_legend(order = 2))
}

df$Max_Trace_Length <- as.character(df$Max_Trace_Length)
df <- df[df$Time != -1, ] 
df$Test <- gsub("\\bAnd\\b", "AND", df$Test)
df$Test <- gsub("\\bOr\\b", "OR", df$Test)
df$Test <- gsub("\\bChoice\\b", "CHOICE", df$Test)
df$Test <- gsub("\\bAnd_Future\\b", "TIMED AND FUTURE", df$Test)
df$Test <- gsub("\\bAnd_Globally\\b", "TIMED AND GLOBALLY", df$Test)
df$Test <- gsub("\\bUntil\\b", "UNTIL", df$Test)

df_set <- df[df$Test == "AND" | df$Test == "OR",]
print_chart(df_set, c('logic' = expression('SLOW'), 'fast'   = "FAST"))

df_choice <- df[df$Test == "CHOICE",]
print_chart(df_choice, c('logic' = expression(LTL[f] ~ 'REWRITING'), 'fast'   = "OPTIMIZED"))

df_derived <- df[df$Test == "TIMED AND FUTURE" | df$Test == "TIMED AND GLOBALLY",]
print_chart(df_derived, c('logic' = expression(LTL[f] ~ 'REWRITING'), 'variant_1' = "VARIANT-1",  'variant_2' = "VARIANT-2"))

print_chart(df[df$Test == "UNTIL" ,], c('logic' = expression(LTL[f] ~ 'REWRITING'), 'fast'   = "OPTIMIZED"))
