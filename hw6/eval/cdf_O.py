import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
####ATTENTION: REUSING SCRIPT FROM LAST PSET. VARS WILL BE MISNAMED. DEAL WITH IT.
# Define a function to calculate the empirical CDF
def ecdf(data):
        x = np.sort(data)
        y = np.arange(1, len(data) + 1) / len(data)
        return x, y
colnames=["id","sent","op","ow","clid","servid","recieved","start","completed"]
operations=["IMG_REGISTER","IMG_ROT90CLKW","IMG_BLUR","IMG_SHARPEN","IMG_VERTEDGES","IMG_HORIZEDGES"]
# Load the data for FIFO and SJN policies
for i in range(6):
            fifo_df = pd.read_csv(f"out_small/images_small_{operations[i]}_ops.csv",sep="[:,]", engine="python",names=colnames)
            sjn_df = pd.read_csv(f"out_all/images_{operations[i]}_ops.csv",sep="[:,]", engine="python",names=colnames)

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
            print(x_fifo[-1]-x_fifo[0],operations[i])
            print(x_sjn[-1]-x_sjn[0],operations[i])
            # Create the first plot for FIFO
            plt.figure(figsize=(10, 5))
            plt.plot(x_fifo, y_fifo, label=f'FIFO (Avg: {avg_fifo:.5f}, 99th Pct: {p99_fifo:.5f})')
            plt.axvline(avg_fifo, color='red', linestyle='--', label=f'Avg: {avg_fifo:.5f}')
            plt.axvline(p99_fifo, color='green', linestyle='--', label=f'99th Pct: {p99_fifo:.5f}')
            plt.xlabel('Response Time')
            plt.ylabel('Cumulative Probability')
            plt.title(f'{operations[i]} - Empirical CDF')
            plt.legend()
            #plt.xlim(0,0.2)
            plt.savefig(f'images_small_{operations[i]}_cdf.png')  # Save the plot

            # Create the second plot for SJN
            plt.figure(figsize=(10, 5))
            plt.plot(x_sjn, y_sjn, label=f'SJN (Avg: {avg_sjn:.5f}, 99th Pct: {p99_sjn:.5f})')
            plt.axvline(avg_sjn, color='blue', linestyle='--', label=f'Avg: {avg_sjn:.5f}')
            plt.axvline(p99_sjn, color='purple', linestyle='--', label=f'99th Pct: {p99_sjn:.5f}')
            plt.xlabel('Response Time')
            plt.ylabel('Cumulative Probability')
            plt.title(f'{operations[i]} - Empirical CDF')
            plt.legend()
            #plt.xlim(0,0.2)
            plt.savefig(f'images_{operations[i]}_cdf.png')  # Save the plot
