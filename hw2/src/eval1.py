import math
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
data=pd.read_csv("mtout.csv",engine="python",sep="[,:]");
print(data.iloc[1,3]-data.iloc[0,3])
#sort reqlens into time brackets, make bar graph. goodnight.

#good evening. lets get to work

index=max(data['length'])
print(index//0.005)
bins=[i*0.005 for i in range(int(index//0.005+1))]
values=[0]*len(bins)
print(len(bins))
print(len(values))
for i in range(len(data['length'])):
    values[int(data.iloc[i,2]//0.005)]+=1/1000
plt.plot(bins,values)
normvals=np.random.normal(1/10,1,size=10000)
normvals.sort()


norm=lambda x: (1/(1*math.sqrt(2*np.pi))*math.exp(-1/2*((x-0.1)/1)**2))
x=[norm(val) for val in normvals]
#x=[val/1000 for val in x]

plt.plot(normvals,x)

#exponential values

expvals=np.random.exponential(10,size=10000)
expvals.sort()

norm=lambda x: 10*math.exp(-10*x)
x=[norm(val) for val in expvals]


#x=[val/1000 for val in x]
#plt.plot(expvals,x)
#print(sum(x))
#plt.plot([i for i in range(10000)],[np.random.normal(1/10,1) for i in range(10000)])
#plt.plot([i for i in range(10000)],[np.random.exponential(1/10) for i in range(10000)])

         
               

# Set the heights of the bars to the normalized values
#plt.yticks([i for i in plt.yticks()[0]], [f'{i/1000}' for i in plt.yticks()[0]])
plt.xlabel("0.005s increments in request length i")
plt.ylabel("number of requests")
plt.savefig('lendist.png')
plt.clf()
intertimes=[]

print(data.iloc[999,3])
for i in range(999):
    intertimes.append(data.iloc[i+1,3]-data.iloc[i,3])

index=max(intertimes)
bins=[i*0.005 for i in range(int(index//0.005+2))]
print("exit")
plt.hist(intertimes,bins=bins)
print("exit")
#plt.yticks([i for i in plt.yticks()[0]], [f'{i/999:.2}' for i in plt.yticks()[0]])
print("exit")
plt.xlabel("interarrival time occurrence in bin 0.005*i")
plt.ylabel("number of occurrences")
plt.savefig("inter.png")
