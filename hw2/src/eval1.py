import pandas as pd
import matplotlib.pyplot as plt
data=pd.read_csv("mtout.csv",engine="python",sep="[,:]");
print(data.iloc[1,3]-data.iloc[0,3])
#sort reqlens into time brackets, make bar graph. goodnight.

#good evening. lets get to work

index=max(data['length'])
bins=[i*0.005 for i in range(int(index//0.005+2))]

plt.hist(data['length'],bins=bins)


# Set the heights of the bars to the normalized values
plt.yticks([i for i in plt.yticks()[0]], [f'{i/1000}' for i in plt.yticks()[0]])
plt.xlabel("0.005s increments in request length i")
plt.ylabel("number of requests")
plt.savefig('bars.png')
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
plt.yticks([i for i in plt.yticks()[0]], [f'{i/999:.2}' for i in plt.yticks()[0]])
print("exit")
plt.xlabel("interarrival time occurrence in bin 0.005*i")
plt.ylabel("number of occurrences")
plt.savefig("inter.png")
