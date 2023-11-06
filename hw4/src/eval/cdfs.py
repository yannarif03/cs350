import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Define a function to calculate the empirical CDF
def ecdf(data):
        x = np.sort(data)
        y = np.arange(1, len(data) + 1) / len(data)
        return x, y

# Load the data for FIFO and SJN policies
fifo_df = pd.read_csv("FIFO_utils/arr_40.csv",sep="[:,]", engine="python")
sjn_df = pd.read_csv("SJN_utils/arr_40.csv",sep="[:,]", engine="python")

# Extract response times
fifo_df["response"]=fifo_df["completed"] - fifo_df["sent"]

fifo_response_times = fifo_df['completed'] - fifo_df['sent']
sjn_response_times = sjn_df['completed'] - sjn_df['sent']

# Calculate average and 99th percentile response times
avg_fifo = fifo_response_times.mean()
p99_fifo = np.percentile(fifo_response_times, 99)

avg_sjn = sjn_response_times.mean()
p99_sjn = np.percentile(sjn_response_times, 99)

# Compute ECDF for both policies
x_fifo, y_fifo = ecdf(fifo_response_times)
x_sjn, y_sjn = ecdf(sjn_response_times)
print(x_fifo[-1]-x_fifo[0])
print(x_sjn[-1]-x_sjn[0])
# Create the first plot for FIFO
plt.figure(figsize=(10, 5))
plt.plot(x_fifo, y_fifo, label=f'FIFO (Avg: {avg_fifo:.2f}, 99th Pct: {p99_fifo:.2f})')
plt.axvline(avg_fifo, color='red', linestyle='--', label=f'Avg: {avg_fifo:.2f}')
plt.axvline(p99_fifo, color='green', linestyle='--', label=f'99th Pct: {p99_fifo:.2f}')
plt.xlabel('Response Time')
plt.ylabel('Cumulative Probability')
plt.title('FIFO Policy - Empirical CDF')
plt.legend()
plt.xlim(0,15)
plt.savefig('fifo_cdf.png')  # Save the plot

# Create the second plot for SJN
plt.figure(figsize=(10, 5))
plt.plot(x_sjn, y_sjn, label=f'SJN (Avg: {avg_sjn:.2f}, 99th Pct: {p99_sjn:.2f})')
plt.axvline(avg_sjn, color='blue', linestyle='--', label=f'Avg: {avg_sjn:.2f}')
plt.axvline(p99_sjn, color='purple', linestyle='--', label=f'99th Pct: {p99_sjn:.2f}')
plt.xlabel('Response Time')
plt.ylabel('Cumulative Probability')
plt.title('SJN Policy - Empirical CDF')
plt.legend()
plt.savefig('sjn_cdf.png')  # Save the plot
