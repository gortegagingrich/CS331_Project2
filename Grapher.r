library(ggplot2)
library(reshape)

table1 = read.table("D:/CS331_Project2/sample_k1.out", header = T, sep = ":")
table2 = read.table("D:/CS331_Project2/sample_k2.out", header = T, sep = ":")

attach(table1)

table1 <- table1[order(N), ]
qplot(N, Time, data=table1, color=Algorithm, main="K = 0.25N") + geom_line()

table2 <- table2[order(N), ]
qplot(N, Time, data=table2, color=Algorithm, main="K = 0.5N") + geom_line()
