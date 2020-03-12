import csv
import numpy as np

latency = []
l1 = []
l2 = []
l3 = []
mm = []
cycle_time = 0.4176717098689242

with open("ram/lat_mem.csv", "r") as f:
	reader = csv.reader(f)
	for row in reader:
		latency.append([int(t) for t in row[1:-1]])

for lt in latency:
	for t in lt:
		if t >= 20 and t <= 25:
			l1.append(t)
		elif t >= 170 and t <= 230:
			l2.append(t)
		elif t >= 450 and t <= 550:
			l3.append(t)
		elif t >= 800 and t < 2000:
			mm.append(t)
		else:
			pass

print(len(l1)+len(l2)+len(l3)+len(mm))
l1_lat = (np.average(l1)-20) * cycle_time
l1_std = np.std(l1) * cycle_time
l2_lat = (np.average(l2)-20) * cycle_time
l2_std = np.std(l2) * cycle_time
l3_lat = (np.average(l3)-20) * cycle_time
l3_std = np.std(l3) * cycle_time
mm_lat = (np.average(mm)-20) * cycle_time
mm_std = np.std(mm) * cycle_time

print(l1_lat)
print(l1_std)
print(l2_lat)
print(l2_std)
print(l3_lat)
print(l3_std)
print(mm_lat)
print(mm_std)