import os
import time
import matplotlib.pyplot as plt
import random

# inputs
recordCount = 10000
mbrSizes = [5.0, 0.5, 0.05]
query_samples = 200

# outputs
load_times = []
query_times = []

for mbrSize in mbrSizes:
    # remove previous file
    os.system("rm test.*")

    # measure load time
    start = time.time()
    os.system("./build/bin/load_data test_data_long.txt test " +
              str(recordCount) + " " + str(mbrSize) + " > /dev/null")
    end = time.time()
    load_times.append(end - start)

    start = time.time()
    for i in range(query_samples):
        # longitude
        x = random.uniform(-120, -80)
        # latitude
        y = random.uniform(30, 50)

        dx = random.uniform(0.2, 1.0)
        dy = random.uniform(0.2, 1.0)

        # measure query time
        os.system("./build/bin/test_modb test " + str(x) + " " +
                  str(y) + " " + str(x + dx) + " " + str(y + dy))

    end = time.time()

    query_times.append(end - start)

# Plot the bar chart
plt.bar(list(map(str, mbrSizes)), load_times)

# Add labels and title
plt.xlabel('MBR size')
plt.ylabel('Time (seconds)')
plt.title('Loading Time of ' + str(recordCount) +
          ' Records for Different MBR Sizes')

# Save the plot as a PNG file
plt.savefig('load_times.png')

plt.clf()

#####################################################################

# Plot the bar chart
plt.bar(list(map(str, mbrSizes)), query_times)

# Add labels and title
plt.xlabel('MBR size')
plt.ylabel('Time (seconds)')
plt.title('Query Time of ' + str(recordCount) +
          ' Records for Different MBR Sizes')

# Save the plot as a PNG file
plt.savefig('query_times.png')

plt.clf()
