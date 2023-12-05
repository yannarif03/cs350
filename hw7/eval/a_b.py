#!/usr/bin/env python3

import matplotlib.pyplot as plt

x_10=[]
x_1=[]


x_10.append(03.80)
x_10.append(04.80)
x_10.append(06.04)
x_10.append(08.76)
x_10.append(10.96)
x_10.append(12.52)
x_10.append(14.31)
x_10.append(15.87)
x_10.append(17.43)
x_10.append(19.12)


x_1.append(04.53)
x_1.append(07.04)
x_1.append(10.64)
x_1.append(12.32)
x_1.append(18.59)
x_1.append(19.52)
x_1.append(24.01)
x_1.append(27.12)
x_1.append(30.15)
x_1.append(33.33)

c_10=[]


c_10.append(03.20)
c_10.append(04.28)
c_10.append(05.21)
c_10.append(05.84)
c_10.append(07.37)
c_10.append(10.26)
c_10.append(08.98)
c_10.append(14.31)
c_10.append(16.01)
c_10.append(12.82)



mid_reps=[x for x in range(1,11)]


# Plotting x_10
plt.figure(figsize=(10, 5))
plt.plot(mid_reps, x_10, marker='o', linestyle='-', color='b', label='x_10')
plt.title('x_10 as a function of mid_reps')
plt.xlabel('mid_reps')
plt.ylabel('runtime in seconds')


# Plotting x_1
plt.plot(mid_reps, x_1, marker='o', linestyle='-', color='r', label='x_1')


# Plotting x_1
plt.plot(mid_reps, c_10, marker='o', linestyle='-', color='g', label='c_10')
plt.legend()
plt.grid(True)
plt.savefig("c.png")
