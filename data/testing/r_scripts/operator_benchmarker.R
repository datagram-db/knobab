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

print_chart <- function(df, labels, type.labs){
  # test.labs <- c(TeX(r'(x)'), TeX(r'(x)'), TeX(r'(x)'))
  test = c("hello", "there", "i", "am", "sam")
  test1 = lapply(test, TeX)
  
  # test2 = lapply(test, TeX)
  alpha <- 1:5
  another =  sprintf('($hi %d$)', alpha)
  
  another1 = lapply(sprintf(r'($\alpha = %d$)', alpha), TeX)
  another2 = unlist(another1)
  names(another2) <- c("A&F(B)", "A&G(B)", "AUB")
  
  ggplot(df, aes(x=Log_Size, y=Time, shape=Type, color=Max_Trace_Length)) +
    geom_line() +
    geom_point(size=3) +
    labs(x = "Log Size", y = "Time (ms)") +
    facet_wrap( ~ Test, nrow=2, ncol=2, labeller = labeller(Test = labels)) +
    theme(legend.position="bottom") +
    scale_color_discrete(name = TeX(r'($Max Trace Length$)')) +
    scale_shape_discrete(labels=type.labs, name = TeX(r'($Algorithm$)')) + 
    scale_y_continuous(trans='log10') +
    scale_x_log10(
      breaks = scales::trans_breaks("log10", function(x) 10^x),
      labels = scales::trans_format("log10", scales::math_format(10^.x))) + 
    guides(colour = guide_legend(order = 1), shape = guide_legend(order = 2))
}

df$Max_Trace_Length <- as.character(df$Max_Trace_Length)
# df$Test[df$Test == "And_Future"] <- TeX("X")
# df$Test[df$Test == "And_Future"] <- "A&F(B)"
# df$Test[df$Test == "And_Globally"] <- "A&G(B)"
# df$Test[df$Test == "Until"] <- "AUB"
# 
# df$Type[df$Type == "fast"] <- TeX(r'($OPTIMIZED$)')
# df$Type[df$Type == "fast_old"] <- TeX(r'($VARIANT-1$)')
# df$Type[df$Type == "logic"] <- TeX(r'($VARIANT-2$)')

labels = c("AND", "OR")
names(labels) <- c("And", "Or")
print_chart(df[df$Test == "And" | df$Test == "Or",],  labels, c(TeX(r'($OPTIMIZED$)'), TeX(r'($LOGICAL$)')))

labels = c("Choice")
names(labels) <- c("Choice")
print_chart(df[df$Test == "Choice",],  labels, c(TeX(r'($OPTIMIZED$)'), TeX(r'($LOGICAL$)')))

labels = c("TIMED AND FUTURE", "TIMED AND GLOBALLY", "UNTIL")
names(labels) <- c("And_Future", "And_Globally", "AUB")
print_chart(df[df$Test == "And_Future" | df$Test == "And_Globally" | df$Test == "Until",], labels, c(TeX(r'($VARIANT-2$)'), TeX(r'($VARIANT-1$)'), TeX(r'($LOGICAL$)')))

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
