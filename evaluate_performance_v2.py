import subprocess
import re
import sys
import os
import matplotlib.pyplot as plt
import numpy as np

# inputs
recordCount = 40000
mbrSizes = [0.05, 0.1, 0.2]
query_samples = 200

# outputs
dbUpdates = []
idxUpdates = []

allPositives = []
falsePositives = []

for mbrSize in mbrSizes:
    # remove previous file
    os.system("rm test.*")

    # start load process
    process = subprocess.Popen(['./build/bin/load_data', 'test_data_long.txt', 'test', str(
        recordCount), str(mbrSize)], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)

    # get process output
    out, err = process.communicate()

    errStr = err.decode()

    sys.stderr.write(errStr)
    sys.stderr.flush()

    # save results
    dbUpdates.append(
        int(re.search('number of updates to b-tree: ([0-9]+)', errStr).group(1)))
    idxUpdates.append(
        int(re.search('number of updates to r-tree: ([0-9]+)', errStr).group(1)))


    # start query process
    process = subprocess.Popen(['./build/bin/query_modb', 'test', '-82',
                               '27', '-81', '28'], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)

    # get process output
    out, err = process.communicate()

    errStr = err.decode()

    sys.stderr.write(errStr)
    sys.stderr.flush()

    # save results
    allPositives.append(int(
        re.search('number of all positives: ([0-9]+)', errStr).group(1)))
    falsePositives.append(int(
        re.search('number of false positives: ([0-9]+)', errStr).group(1)))


# Define the data
labels = list(map(str, mbrSizes))
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
plt.xlabel('MBR Size')
plt.ylabel('Number of Updates')
plt.title('Update Rates for Different MBR Sizes')

# Set the x-axis tick labels
plt.xticks(index + bar_width/2, labels)

# Add a legend
plt.legend()

# Display the chart
plt.savefig('update_stats.png')
plt.clf()


# Define the data
labels = list(map(str, mbrSizes))
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
plt.xlabel('MBR Size')
plt.ylabel('Number of Positives')
plt.title('Number of Positive Query Results for Different MBR Sizes')

# Set the x-axis tick labels
plt.xticks(index + bar_width/2, labels)

# Add a legend
plt.legend()

# Display the chart
plt.savefig('query_stats.png')
plt.clf()
