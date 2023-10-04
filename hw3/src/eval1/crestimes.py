import pandas as pd
from matplotlib import pyplot as plt
import numpy as np
avg_response_times=[]
server_util=[]
for i in range(10,20):
    dfi=pd.read_csv(f"cdata/requests/req0-{i}.csv",sep='[,:]',engine='python')
    dfi['response time']=dfi['completed']-dfi['timestamp']
    avgt=dfi['response time'].mean()
    active_time=dfi['length'].sum()
    total_time=dfi['completed'].iloc[-1]-dfi['recieved'][0]
    server_util.append(active_time/total_time)
    avg_response_times.append(avgt)

print(server_util)
print(avg_response_times)
plt.xlabel("server util")
plt.ylabel("response time")
plt.plot(server_util,avg_response_times,color='g')

avg_response_times=[]
server_util=[]
for i in range(10,20):
    dfi=pd.read_csv(f"cdata/requests/req1-{i}.csv",sep='[,:]',engine='python')
    dfi['response time']=dfi['completed']-dfi['timestamp']
    avgt=dfi['response time'].mean()
    active_time=dfi['length'].sum()
    total_time=dfi['completed'].iloc[-1]-dfi['recieved'][0]
    server_util.append(active_time/total_time)
    avg_response_times.append(avgt)


plt.plot(server_util,avg_response_times,color='r')


plt.savefig("resplot.png")
