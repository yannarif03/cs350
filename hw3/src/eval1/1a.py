import math
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
data=pd.read_csv("b.csv",engine="python",sep="[,:]");
index=max(data['length'])
print(index//0.005)
bins=[i*0.005 for i in range(int(index//0.005+1))]
values=[0]*len(bins)
for i in range(len(data['length'])):
    values[int(data.iloc[i,2]//0.005)]+=1/1500
plt.plot(bins,values)

plt.xlabel("0.005s increments in request length i")
plt.ylabel("number of requests")
plt.savefig('graphs/length.png')
plt.clf()
intertimes=[]


for i in range(999):
    intertimes.append(data.iloc[i+1,3]-data.iloc[i,3])


index=max(intertimes)
print(index//0.005)
bins=[i*0.005 for i in range(int(index//0.005+1))]
values=[0]*len(bins)
for i in range(len(intertimes)):
    values[int(intertimes[i]//0.005)]+=1/1499
plt.plot(bins,values)

plt.xlabel("interarrival time occurrence in bin 0.005*i")
plt.ylabel("number of occurrences")
plt.savefig("graphs/interarrival.png")
