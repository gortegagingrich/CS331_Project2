library(ggplot2)
library(reshape)

table0 = read.table("sample_k0.out", header = T, sep = ":")
table1 = read.table("sample_k1.out", header = T, sep = ":")
table2 = read.table("sample_k2.out", header = T, sep = ":")
table3 = read.table("sample_k3.out", header = T, sep = ":")
table4 = read.table("sample_k4.out", header = T, sep = ":")

attach(table1)

table0 <- table0[order(N), ]
qplot(N, Time, data=table0, color=Algorithm, main="K = 1") + geom_line()

table1 <- table1[order(N), ]
qplot(N, Time, data=table1, color=Algorithm, main="K = 0.25N") + geom_line()

table2 <- table2[order(N), ]
qplot(N, Time, data=table2, color=Algorithm, main="K = 0.5N") + geom_line()

table3 <- table3[order(N), ]
qplot(N, Time, data=table3, color=Algorithm, main="K = 0.75N") + geom_line()

table4 <- table4[order(N), ]
qplot(N, Time, data=table4, color=Algorithm, main="K = N") + geom_line()