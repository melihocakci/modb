import random
import time
import os
import matplotlib.pyplot as plt
import numpy as np

# inputs
recordCounts = [2000, 4000, 6000, 8000, 10000]
querySamples = 200

# outputs
writeTimes = []
queryTimes = []

for recordCount in recordCounts:
    # remove previous file
    os.system("rm test.*")

    start = time.time()
    # start load process
    os.system('./build/bin/load_data test_data_long.txt test ' +
              str(recordCount) + ' 0.4')
    end = time.time()

    # save results
    writeTimes.append(end - start)

    start = time.time()
    for i in range(querySamples):
        # longitude
        x = random.uniform(-120, -80)
        # latitude
        y = random.uniform(30, 50)

        dx = random.uniform(1.0, 10.0)
        dy = random.uniform(1.0, 10.0)
        # start query process
        os.system('./build/bin/intersection_query test ' + str(x) + ' ' +
                  str(y) + ' ' + str(x + dx) + ' ' + str(y + dy))
    end = time.time()

    # save results
    queryTimes.append(end - start)


# Define the data
labels = list(map(str, recordCounts))
values1 = writeTimes
values2 = queryTimes

# Set the positions of the bars and their width
bar_width = 0.35
index = np.arange(len(labels))
bar_positions1 = index
bar_positions2 = index + bar_width

# Plot the bars
plt.bar(bar_positions1, values1, width=bar_width, label='write time')
plt.bar(bar_positions2, values2, width=bar_width, label='query time')

# Add labels and title
plt.xlabel('Record Count')
plt.ylabel('Time (seconds)')
plt.title('Execution Time for Different Record Counts')

# Set the x-axis tick labels
plt.xticks(index + bar_width/2, labels)

# Add a legend
plt.legend()

# Display the chart
plt.savefig('record_stats.png')
plt.clf()
