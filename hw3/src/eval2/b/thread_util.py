import pandas as pd
from matplotlib import pyplot as plt
import numpy as np
avg_response_times=[]
server_util=[]
dfi=pd.read_csv(f"T0/reqs.csv",sep='[,:]',engine='python')
dfi['response time']=dfi['completed']-dfi['timestamp']
avgt=dfi['response time'].mean()
active_time=dfi['length'].sum()
total_time=dfi['completed'].iloc[-1]-dfi['recieved'][0]
print(active_time/total_time)
# server_util.append(active_time/total_time)
# avg_response_times.append(avgt)
# plt.xlabel("thread 0 util")
# plt.ylabel("response time")
# plt.plot(server_util,avg_response_times)
# plt.savefig("T0/util.png")
# plt.clf()

avg_response_times=[]
server_util=[]

dfi=pd.read_csv(f"T1/reqs.csv",sep='[,:]',engine='python')
dfi['response time']=dfi['completed']-dfi['timestamp']
avgt=dfi['response time'].mean()
active_time=dfi['length'].sum()
total_time=dfi['completed'].iloc[-1]-dfi['recieved'][0]
print(active_time/total_time)
# server_util.append(active_time/total_time)
# avg_response_times.append(avgt)
# plt.xlabel("thread 1 util")
# plt.ylabel("response time")
# plt.plot(server_util,avg_response_times)
# plt.savefig("T1/util.png")
