import pandas as pd
from matplotlib import pyplot as plt
import numpy as np
columns=["request id","send time", "request length","recieved time","completed time"]
df=pd.read_csv("request_times.csv",usecols=columns,sep=',:')
df['processing time']=df['completed time']-df['recieved time']
print(len(df['processing time'])/df['processing time'].sum())
active_time=df['request length'].sum()
total_time=df['completed time'].iloc[-1]-df['recieved time'][0]
print(active_time/total_time)

