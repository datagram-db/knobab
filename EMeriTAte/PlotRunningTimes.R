library(ggplot2)
mpg <- read.csv("~/projects/knobab2_loggen/EMeriTAte/running_times.csv")
mpg$Algorithm <- factor(mpg$Algorithm, levels=c("E-KNN", "Rocket", "CIF", "STC", "TapNet"))
mpg$Dataset <- factor(mpg$Dataset, levels=c("Italy Power Demand", "Basic Motions",  "OsuLeaf","Japanese Vowels", "Dyskinetic Events"))
mpg$Phase <- factor(mpg$Phase, levels=c("Training", "Testing"))
ggplot(mpg) + 
  geom_bar(aes(x = Algorithm, y = Time..ms., fill = Phase), stat='identity', position = position_dodge(preserve = 'single')) + facet_grid(. ~ Dataset)+  scale_y_log10(
    breaks = scales::trans_breaks("log10", function(x) 10^x),
    labels = scales::trans_format("log10", scales::math_format(10^.x))
  ) + scale_fill_manual(values=c("#b2182b", "#2166ac", "#ef8a62", "#67a9cf", "#fddbc7", "#d1e5f0")) + ylab("Time (ms)")