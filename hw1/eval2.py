import pandas as pd
from matplotlib import pyplot as plt
import numpy as np
columns=["request id","send time", "request length"," recieved time","completed time"]
df=pd.read_csv("request_times.csv",usecols=columns,sep='[,:]',engine='python')
df['processing time']=df['completed time']-df[' recieved time']

print(len(df['processing time'])/df['processing time'].sum())
active_time=df['request length'].sum()
total_time=df['completed time'].iloc[-1]-df[' recieved time'][0]
print(active_time/total_time)


df10=pd.read_csv("csv_times/times_a10.csv",usecols=columns,sep='[,:]',engine='python')
df10['response time']=df10['completed time']-df10['send time']

avgt=df10['response time'].mean()
maxt=df10['response time'].max()
mint=df10['response time'].min()
stdev=df10['response time'].std()
print(f'avg: {avgt}\nmax: {maxt}\nmin:{mint}\nstdev: {stdev}')
avg_response_times=[]
server_util=[]
for i in range(1,13):
    dfi=pd.read_csv(f"csv_times/times_a{i}.csv",usecols=columns,sep='[,:]',engine='python')
    dfi['response time']=dfi['completed time']-dfi['send time']
    avgt=dfi['response time'].mean()
    maxt=dfi['response time'].max()
    mint=dfi['response time'].min()
    stdev=dfi['response time'].std()
    active_time=dfi['request length'].sum()
    total_time=dfi['completed time'].iloc[-1]-dfi[' recieved time'][0]
    server_util.append(active_time/total_time)
    avg_response_times.append(avgt)


a_arg=[i for i in range(1,13)]
plt.xlabel("a arg val")
plt.ylabel("server util percent")
plt.plot(a_arg,server_util)
plt.savefig("bashplot.png")
plt.clf()
plt.xlabel("server util")
plt.ylabel("response time")
print(avg_response_times)
plt.plot(server_util,avg_response_times)
plt.savefig("resplot.png")
