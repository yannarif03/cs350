import math
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
data=pd.read_csv("mtout.csv",engine="python",sep="[,:]");
print(data.iloc[1,3]-data.iloc[0,3])
#sort reqlens into time brackets, make bar graph. goodnight.


index=max(data['length'])
print(index//0.005)
bins=[i*0.005 for i in range(int(index//0.005+1))]
values=[0]*len(bins)
for i in range(len(data['length'])):
    values[int(data.iloc[i,2]//0.005)]+=1/1000
plt.plot(bins,values)

#normal values
normvals=np.random.normal(1/10,1,size=10000)
normvals.sort()


norm=lambda x: (1/(1*math.sqrt(2*np.pi))*math.exp(-1/2*((x-0.1)/1)**2))
x=[norm(val) for val in normvals]
x=[val/4 for val in x]

plt.plot(normvals,x)

#exponential values

expvals=np.random.exponential(1/10,size=10000)
expvals.sort()

norm=lambda x: 10*math.exp(-10*x)
x=[norm(val) for val in expvals]


x=[val/150 for val in x]
plt.plot(expvals,x)

#uniform values


univals=np.random.uniform(-8/10,1,size=10000)
univals.sort()

uni=lambda x,y: 1/(y-x)
x=[uni(-0.8,1) for val in univals]


x=[val/10 for val in x]
plt.plot(univals,x)
plt.axvline(-0.8,ymin=0.05,ymax=x[0]/0.1,color="r")
plt.axvline(1,ymin=0.05,ymax=x[0]/0.1,color="r")
plt.axhline(y=0,xmax=3/8,color="r")
plt.axhline(y=0,xmin=4.7/8,color="r")

# Set the heights of the bars to the normalized values
#plt.yticks([i for i in plt.yticks()[0]], [f'{i/1000}' for i in plt.yticks()[0]])
plt.xlabel("0.005s increments in request length i")
plt.ylabel("number of requests")
plt.savefig('lendist.png')
plt.clf()
intertimes=[]


for i in range(999):
    intertimes.append(data.iloc[i+1,3]-data.iloc[i,3])

#index=max(intertimes)
#bins=[i*0.005 for i in range(int(index//0.005+2))]


index=max(intertimes)
print(index//0.005)
bins=[i*0.005 for i in range(int(index//0.005+1))]
values=[0]*len(bins)
for i in range(len(intertimes)):
    values[int(intertimes[i]//0.005)]+=1/999
plt.plot(bins,values)
#normal values
normvals=np.random.normal(1/6,1,size=10000)
normvals.sort()


norm=lambda x: (1/(1*math.sqrt(2*np.pi))*math.exp(-1/2*((x-0.1)/1)**2))
x=[norm(val) for val in normvals]
x=[val/4 for val in x]

plt.plot(normvals,x)

#exponential values

expvals=np.random.exponential(1/6,size=10000)
expvals.sort()

norm=lambda x: 10*math.exp(-10*x)
x=[norm(val) for val in expvals]


x=[val/150 for val in x]
plt.plot(expvals,x)

#uniform values


univals=np.random.uniform(-1,4/3,size=10000)
univals.sort()

uni=lambda x,y: 1/(y-x)
x=[uni(-1,4/3) for val in univals]


x=[val/10 for val in x]
plt.plot(univals,x)
plt.axvline(-1,ymin=0.05,ymax=x[0]/0.1,color="r")
plt.axvline(4/3,ymin=0.05,ymax=x[0]/0.1,color="r")
plt.axhline(y=0,xmin=-4,xmax=3.2/9,color="r")
plt.axhline(y=0,xmin=5/8,xmax=4,color="r")


#plt.hist(intertimes,bins=bins)
plt.xlabel("interarrival time occurrence in bin 0.005*i")
plt.ylabel("number of occurrences")
plt.savefig("arrdist.png")
