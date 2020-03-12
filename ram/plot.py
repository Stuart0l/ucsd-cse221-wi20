import matplotlib.pyplot as plt
import numpy as np
import csv

stride = []
latency = []
with open("lat_mem.csv", "r") as f:
	reader = csv.reader(f)
	for row in reader:
		stride.append(int(row[0]))
		this_latency = []
		for t in row[1:-1]:
			if int(t) < 2000:
				this_latency.append(int(t) * 0.4176717098689242)
		latency.append(this_latency)  # convert cycles to ns

ylim = [200, 200, 200, 200, 600, 600, 600, 600, 600, 600, 600, 600]
late = np.asarray(latency)
i = 0
for lt in late:
	tlt = "stride=" + str(stride[i])
	plt.subplot(6,2,i+1)
	plt.ylim(0, ylim[i]) 
	plt.xlim(0, 100000)
	plt.plot(lt, 'b.')
	plt.xticks(fontsize='x-small')
	plt.yticks(fontsize='x-small', rotation=90)
	plt.title(tlt, fontsize='x-small')
	plt.ylabel("latency in ns", fontsize='x-small')
	i+=1
	if i >= 10:
		break

plt.subplots_adjust(left=0, bottom=0, top=1, right=1)
plt.tight_layout(pad=0)
plt.savefig("lat_mem.png", dpi=300)

pass
