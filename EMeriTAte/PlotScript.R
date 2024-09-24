library(ggplot2)
require(scales)
library(reshape2)
library(dplyr)
library(scales)
library(latex2exp)

## http://www.cookbook-r.com/Manipulating_data/Summarizing_data/
summarySE <- function(data=NULL, measurevar, groupvars=NULL, na.rm=FALSE,
                      conf.interval=.95, .drop=TRUE) {
  library(plyr)
  
  # New version of length which can handle NA's: if na.rm==T, don't count them
  length2 <- function (x, na.rm=FALSE) {
    if (na.rm) sum(!is.na(x))
    else       length(x)
  }
  
  # This does the summary. For each group's data frame, return a vector with
  # N, mean, and sd
  datac <- ddply(data, groupvars, .drop=.drop,
                 .fun = function(xx, col) {
                   c(N    = length2(xx[[col]], na.rm=na.rm),
                     mean = mean   (xx[[col]], na.rm=na.rm),
                     sd   = sd     (xx[[col]], na.rm=na.rm)
                   )
                 },
                 measurevar
  )
  
  # Rename the "mean" column    
  datac <- rename(datac, c("mean" = measurevar))
  
  datac$se <- datac$sd / sqrt(datac$N)  # Calculate standard error of the mean
  
  # Confidence interval multiplier for standard error
  # Calculate t-statistic for confidence interval: 
  # e.g., if conf.interval is .95, use .975 (above/below), and use df=N-1
  ciMult <- qt(conf.interval/2 + .5, datac$N-1)
  datac$ci <- datac$se * ciMult
  
  return(datac)
}

## http://www.sthda.com/english/wiki/ggplot2-error-bars-quick-start-guide-r-software-and-data-visualization
data_summary <- function(data, varname, groupnames){
  require(plyr)
  summary_func <- function(x, col){
    c(mean = mean(x[[col]], na.rm=TRUE),
      sd = sd(x[[col]], na.rm=TRUE))
  }
  data_sum<-ddply(data, groupnames, .fun=summary_func,
                  varname)
  data_sum <- rename(data_sum, c("mean" = varname))
  return(data_sum)
}


## Running Time Analysis
path <- "/home/giacomo/projects/knobab2_loggen/EMeriTAte/italy_power_demand/"
results <- paste(c(path,"/__results/"),collapse="")
benchmark_poly <- paste(c(results,"benchmark_poly.csv"),collapse="")
t<- read.csv(benchmark_poly)
t$PolyMine <-as.logical(t$isFilenamePolyadic)
fr <- t[,c("mining_supp","reduction","isFilenamePolyadic","refining")]
fr <- fr[fr$reduction==1,]

## Running time effects for refining
ggplot(fr, aes(x=mining_supp,y=refining,fill=PolyMine))+geom_bar(stat="identity", position = "dodge")+xlab("Support")+ylab("Reduction Time (ms)")+ scale_fill_grey(start = 0, end = .6)+theme_bw()

## Mining time
t$Reduce <-as.logical(t$reduction)
ggplot(t, aes(x=mining_supp,y=mining,shape=PolyMine,color=PolyMine))+stat_summary(fun.data = 'mean_sdl', mult = 1, geom = 'smooth')+geom_point(size=3)+scale_y_log10(breaks = trans_breaks("log10", function(x) 10^x),
                                                                                             labels = trans_format("log10", math_format(10^.x)))+ scale_colour_grey(start = 0, end = .6)+theme_bw()+xlab("Support")+ylab("Mining Time (ms)")


## Clauses effect over parameters
clauseanalysis_results_tot <- paste(c(results,"clauseanalysis_results_tot.csv"),collapse="")
t <- read.csv(clauseanalysis_results_tot)
t <- melt(t, id=c("filename", "class", "mining_supp", "reduction", "isFilenamePolyadic", "reclassify"))
t = t[t$variable != "Total", ]
t$Timed = t$variable == "timed"
t <- t %>% mutate(
  Reduction = case_when(
    reduction == 0 ~ "Not Reduced",
    reduction == 1 ~ "Reduced"
  )
) %>% mutate(
  Mining = case_when(
    isFilenamePolyadic == 0 ~ "Traditional",
    isFilenamePolyadic == 1 ~ "Polyadic+Hierarchy"
  ) 
)
ggplot(t, aes(x=mining_supp, y=value, color=Timed,linetype=Reduction,shape=Mining))+geom_line()+geom_point(size=4)+facet_grid( ~ as.character(class))+xlab("Support")+ylab(TeX("$|{Clauses}|$"))+
  scale_y_continuous(trans = log10_trans(),
                     breaks = trans_breaks("log10", function(x) 10^x),
                     labels = trans_format("log10", math_format(10^.x)))+ scale_colour_grey(start = 0, end = .7)+theme_bw()



results_proposed <- paste(c(results,"results_proposed.csv"),collapse="")
t<- read.csv(results_proposed)
t$PolyMine <- as.logical(t$poly)
t$Reduce <- as.logical(t$red)

tf2 <- data_summary(t, varname="f1", groupnames=c("supp","PolyMine","Reduce"))
ggplot(tf2, aes(x=supp, y=f1, group=interaction(PolyMine,Reduce), color=PolyMine,linetype=Reduce)) + 
  geom_line() +
  geom_point()+
  geom_errorbar(aes(ymin=f1-sd, ymax=f1+sd), width=.05)+
  xlab("Support")+ylab("F1")+scale_colour_grey(start = 0, end = .7)+theme_bw()

tf2 <- data_summary(t, varname="precision", groupnames=c("supp","PolyMine","Reduce"))
ggplot(tf2, aes(x=supp, y=precision, group=interaction(PolyMine,Reduce), color=PolyMine,linetype=Reduce)) + 
  geom_line() +
  geom_point()+
  geom_errorbar(aes(ymin=precision-sd, ymax=precision+sd), width=.05)+
  xlab("Support")+ylab("Precision")+scale_colour_grey(start = 0, end = .7)+theme_bw()

tf2 <- data_summary(t, varname="recall", groupnames=c("supp","PolyMine","Reduce"))
ggplot(tf2, aes(x=supp, y=recall, group=interaction(PolyMine,Reduce), color=PolyMine,linetype=Reduce)) + 
  geom_line() +
  geom_point()+
  geom_errorbar(aes(ymin=recall-sd, ymax=recall+sd), width=.05)+
  xlab("Support")+ylab("Recall")+scale_colour_grey(start = 0, end = .7)+theme_bw()

tf2 <- data_summary(t, varname="accuracy", groupnames=c("supp","PolyMine","Reduce"))
ggplot(tf2, aes(x=supp, y=accuracy, group=interaction(PolyMine,Reduce), color=PolyMine,linetype=Reduce)) + 
  geom_line() +
  geom_point()+
  geom_errorbar(aes(ymin=accuracy-sd, ymax=accuracy+sd), width=.05)+
  xlab("Support")+ylab("Accuracy")+scale_colour_grey(start = 0, end = .7)+theme_bw()