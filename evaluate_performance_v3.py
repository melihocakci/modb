import subprocess
import re
import sys
import os
import matplotlib.pyplot as plt
import numpy as np

# inputs
recordCount = 50000
windowSizes = [0.05, 0.1, 0.2]
query_samples = 200

# outputs
dbUpdates = []
idxUpdates = []

allPositives = []
falsePositives = []

queryTime = []
filterTime = []
idxWriteTime = []

for windowSize in windowSizes:
    # remove previous file
    os.system("rm test.*")

    # start load process
    process = subprocess.Popen(['./build/bin/load_query', 'dataset_mixed_long.txt', 'test', str(
        recordCount), str(windowSize)], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)

    # get process output
    out, err = process.communicate()

    errStr = err.decode()

    sys.stderr.write(errStr)
    sys.stderr.flush()

    # save results
    dbUpdates.append(
        int(re.search('number of updates to b-tree:\s+([0-9]+)', errStr).group(1)))
    idxUpdates.append(
        int(re.search('number of updates to r-tree:\s+([0-9]+)', errStr).group(1)))

    allPositives.append(int(
        re.search('number of all positives:\s+([0-9]+)', errStr).group(1)))
    falsePositives.append(int(
        re.search('number of false positives:\s+([0-9]+)', errStr).group(1)))
    
    queryTime.append(float(
        re.search('time spent for queries:\s+([0-9]*\.[0-9]+)s', errStr).group(1)))
    filterTime.append(float(
        re.search('time spent for filtering:\s+([0-9]*\.[0-9]+)s', errStr).group(1)))
    idxWriteTime.append(float(
        re.search('time spent for r-tree writes:\s+([0-9]*\.[0-9]+)s', errStr).group(1)))

##################################################

# Define the data
labels = list(map(str, windowSizes))
values1 = dbUpdates
values2 = idxUpdates

# Set the positions of the bars and their width
bar_width = 0.35
index = np.arange(len(labels))
bar_positions1 = index
bar_positions2 = index + bar_width

# Plot the bars
plt.bar(bar_positions1, values1, width=bar_width, label='B-tree Updates')
plt.bar(bar_positions2, values2, width=bar_width, label='R-tree Updates')

# Add labels and title
plt.xlabel('Window Size')
plt.ylabel('Number of Updates')
plt.title('Update Rates for Different Window Sizes')

# Set the x-axis tick labels
plt.xticks(index + bar_width/2, labels)

# Add a legend
plt.legend()

# Display the chart
plt.savefig('update_stats.png')
plt.clf()

##################################################

# Define the data
labels = list(map(str, windowSizes))
values1 = allPositives
values2 = falsePositives

# Set the positions of the bars and their width
bar_width = 0.35
index = np.arange(len(labels))
bar_positions1 = index
bar_positions2 = index + bar_width

# Plot the bars
plt.bar(bar_positions1, values1, width=bar_width, label='All Positives')
plt.bar(bar_positions2, values2, width=bar_width, label='False Positives')

# Add labels and title
plt.xlabel('Window Size')
plt.ylabel('Number of Positives')
plt.title('Number of Positive Query Results for Different Window Sizes')

# Set the x-axis tick labels
plt.xticks(index + bar_width/2, labels)

# Add a legend
plt.legend()

# Display the chart
plt.savefig('query_stats.png')
plt.clf()

##################################################

# Define the data
labels = list(map(str, windowSizes))
values1 = queryTime
values2 = filterTime

# Set the positions of the bars and their width
bar_width = 0.35
index = np.arange(len(labels))
bar_positions1 = index
bar_positions2 = index + bar_width

# Plot the bars
plt.bar(bar_positions1, values1, width=bar_width, label='Query Time')
plt.bar(bar_positions2, values2, width=bar_width, label='Filter Time')

# Add labels and title
plt.xlabel('Window Size')
plt.ylabel('Time (seconds)')
plt.title('Query and Filter times For Different Window Sizes')

# Set the x-axis tick labels
plt.xticks(index + bar_width/2, labels)

# Add a legend
plt.legend()

# Display the chart
plt.savefig('query_times.png')
plt.clf()

#####################################################################

# Plot the bar chart
plt.bar(list(map(str, windowSizes)), idxWriteTime)

# Add labels and title
plt.xlabel('Window size')
plt.ylabel('Time (seconds)')
plt.title('Write time of R-tree for Different Window Sizes')

# Save the plot as a PNG file
plt.savefig('idx_write_times.png')

plt.clf()
