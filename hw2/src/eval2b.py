import matplotlib.pyplot as plt
import pandas as pd
import math
import sys
import numpy as np

restimes=[]
qsizes=[]
utils=[]
for i in range(1,16):
    data=open(f"pyout/pydat{i}.txt","r")
    qsizes.append(data.readline()[1:5])
    restimes.append(data.readline()[1:5])
    utils.append(data.readline()[1:5])

print(qsizes)
print(restimes)
fig, ax1 = plt.subplots()
ax2=ax1.twinx()
ax1.plot(utils,restimes,"b")
ax2.plot(utils,qsizes,"g")

ax1.set_xlabel("server util",color="r")
ax1.set_ylabel('avg response time',color="b")
ax2.set_ylabel('avg queue sizes',color="g")
plt.savefig("2b.png")
