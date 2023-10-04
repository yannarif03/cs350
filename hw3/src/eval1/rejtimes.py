import pandas as pd
from matplotlib import pyplot as plt
import numpy as np
df=pd.read_csv("rejqueues/rejects_d.csv")
interrej=[df['recieved'].iloc[i+1]-df['recieved'].iloc[i] for i in range(len(df['recieved'])-1)]
index=max(interrej)
print(index//0.05)
bins=[i*0.05 for i in range(int(index//0.05+1))]
values=[0]*len(bins)
for i in range(len(interrej)):
    values[int(interrej[i]//0.05)]+=1/len(interrej)
plt.plot(bins,values)
plt.xlabel("inter-rejection time")
plt.ylabel("frequency")
plt.savefig("rejqueues/d_plot.png")
print(len(interrej))
