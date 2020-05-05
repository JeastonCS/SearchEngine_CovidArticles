se <- read.csv("SearchEngineData.csv")
head(se)

with(se, plot(file.to.wordIndex.s. ~ ï..num.of.docs))

# testing whats the closest complexity of reading json to indexes
fit1 <- with(se, lm(file.to.wordIndex.s. ~ ï..num.of.docs)) # linear
summary(fit1)
plot(fit1)

fit2 <- with(se, lm(file.to.wordIndex.s. ~ ï..num.of.docs*log(ï..num.of.docs, base = 2))) # log
summary(fit2)
plot(fit2)

fit3 <- with(se, lm(file.to.wordIndex.s. ~ ï..num.of.docs^2)) # quadratic
summary(fit3)
plot(fit3)

