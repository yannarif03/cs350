import pandas as pd
from matplotlib import pyplot as plt
import numpy as np
worker_x=[i*2 for i in range(1,5)]
avg_response_times=[]
for i in worker_x:
    df=pd.read_csv(f"creqs/w{i}.csv",sep='[,:]',engine='python')
    df['response time']=df['completed']-df['timestamp']
    avg_response_times.append(df['response time'].mean())
plt.xlabel("number of workers")
plt.ylabel("response time avg")
print(worker_x)
print(avg_response_times)
plt.plot(worker_x,avg_response_times,color='r')
plt.savefig("workresp.png")
                              
