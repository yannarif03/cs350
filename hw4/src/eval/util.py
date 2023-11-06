import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
utils=[]
restimes=[]
for i in range(22,42,2):
    df=pd.read_csv(f"FIFO_utils/arr_{i}.csv",sep="[:,]",engine="python")
    df['response time']=df["completed"]-df["sent"]
    util=df["completed"].iloc[-1]-df["recieved"].iloc[0]
    util=df["length"].sum()/util
    utils.append(util)
    restimes.append(df["response time"].mean())

plt.xlabel("server utilizations")
plt.ylabel("response times")
sns.lineplot(x=utils,y=restimes,label="FIFO")

utils=[]
restimes=[]
for i in range(22,42,2):
    df=pd.read_csv(f"SJN_utils/arr_{i}.csv",sep="[:,]",engine="python")
    df['response time']=df["completed"]-df["sent"]
    util=df["completed"].iloc[-1]-df["recieved"].iloc[0]
    util=df["length"].sum()/util
    utils.append(util)
    restimes.append(df["response time"].mean())

plt.xlabel("server utilizations")
plt.ylabel("response times")
sns.lineplot(x=utils,y=restimes,label="SJN")
plt.legend()


plt.savefig("polutil.png")
