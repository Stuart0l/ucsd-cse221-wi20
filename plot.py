import matplotlib.pyplot as plt
import numpy as np
import csv

stride = []
latency = []
with open("lat_mem.csv", "r") as f:
	reader = csv.reader(f)
	for row in reader:
		stride.append(int(row[0]))
		latency.append([float(t) * 0.4176717098689242 for t in row[1:-1]])  # convert cycles to ns

late = np.asarray(latency)
lgsz = np.arange(9, 30, 1)

i = 0
for lt in late:
	lbl = "stride=" + str(stride[i])
	plt.plot(lgsz, lt, label=lbl)
	i+=1

plt.xticks(lgsz)
plt.xlabel("log2 array size")
plt.ylabel("latency in ns")
plt.legend()

plt.annotate('L1 cache', (11, 1))
plt.annotate('L2 cache', (16, 1.5))
plt.annotate('L3 cache', (21, 4))
plt.annotate('main mem', (26, 10))

plt.savefig("lat_mem.pdf")

pass
