library(ggplot2)

t <- read.csv("/home/giacomo/projects/knobab2_loggen/EMeriTAte/total_emeritate_times2.csv")
t$Algorithm <- factor(t$Algorithm, levels=c("EMeriTAte", "EMeriTAte+DF"))
t$Dataset <- factor(t$Dataset, levels=c("Italy Power Demand", "Basic Motions",  "OsuLeaf","Japanese Vowels", "Dyskinetic Events"))
t$Phase <- factor(t$Phase, levels=c("Mining",
                                    "Loading",
                                    "Serialization",
                                    "Embedding Generation",
                                    "Training"))

aggr <- aggregate(t$Time..ms., by=list(Algorithm=t$Algorithm,Dataset=t$Dataset), FUN=sum)
aggr$Phase <- "Training"
names(aggr) <- c("Algorithm", "Dataset", "Time..ms.", "Phase")
aggr <- aggr[, c("Algorithm", "Dataset", "Phase", "Time..ms.")]

mpg <- read.csv("~/projects/knobab2_loggen/EMeriTAte/running_times.csv")
mpg <- mpg[(mpg$Phase == "Training") & (mpg$Dataset != "Japanese Vowels"),]
mpg$Dataset <- factor(mpg$Dataset, levels=c("Italy Power Demand", "Basic Motions",  "OsuLeaf","Japanese Vowels", "Dyskinetic Events"))
mpg$Phase <- factor(mpg$Phase, levels=c("Training", "Testing"))
mpg <- rbind(aggr,mpg)
mpg$Algorithm <- factor(mpg$Algorithm, levels=c("E-KNN", "Rocket", "CIF", "STC", "TapNet","EMeriTAte", "EMeriTAte+DF"))

# rsvg-convert --keep-image-data -d 300 -p 300 -a -f pdf EMeriTAte_Cmp.svg > EMeriTAte_Cmp.pdf
ggplot(t) + 
  geom_bar(aes(x = Algorithm, y = Time..ms., fill = Phase), stat='identity', position = position_dodge(preserve = 'single')) + facet_grid(. ~ Dataset)+  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))
  ) + scale_fill_manual(values=c("#b2182b", "#2166ac", "#ef8a62", "#67a9cf", "#fddbc7", "#d1e5f0", "#ffffbf")) + ylab("Time (ms)")


ggplot(mpg) + 
  geom_bar(aes(x = Algorithm, y = Time..ms., fill = Algorithm), stat='identity', position = position_dodge(preserve = 'single')) + facet_grid(. ~ Dataset)+  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))
  ) + scale_fill_manual(values=c("#b2182b", "#2166ac", "#ef8a62", "#67a9cf", "#fddbc7", "#d1e5f0", "#ffffbf")) + ylab("Time (ms)") + 
  theme(axis.text.x = element_blank(),axis.ticks.x = element_blank())

