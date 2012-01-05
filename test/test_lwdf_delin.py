#! /usr/bin/python
import sys
sys.path.append('../Release')
import numpy as np
import matplotlib.pyplot as plt
import dsp
# Fetch example data
[ir,red,amb] = np.load('test_data.npy')

# Define filter (cutt-off in pi radians per seconds; limit is 0.5pi)
# ftype = 'bessel'
# ftype = 'butter'
ftype = 'cheby2'
order = 5
wn = 0.1
rs = 70
ir_lwdfilter = dsp.Filter(order,wn,rs=rs,ftype=ftype)
red_lwdfilter = dsp.Filter(order,wn,rs=rs,ftype=ftype)

# Define the turning point detector
ir_delin = dsp.PPGDelineator(200000,20000)
red_delin = dsp.PPGDelineator(300000,10000)
# These settings for dichrotic
# ir_delin = dsp.PPGDelineator(50000,5000)
# red_delin = dsp.PPGDelineator(50000,5000)

# Perform sample-by-sample filtering
ir_out = []
ir_q = []
ir_i = []
ir_t = []
red_out = []
red_q = []
red_i = []
red_t = []
ir_timestamp = 0
ir_accumulator = 0
red_timestamp = 0
red_accumulator = 0
ir_state = 0
red_state = 0
ir_time = np.zeros((4,),dtype=np.int)
ir_log = np.zeros((4,),dtype=np.int)
ir_vout = np.zeros((2,),dtype=np.int)
ir_tout = np.zeros((2,),dtype=np.int)
red_time = np.zeros((4,),dtype=np.int)
red_log = np.zeros((4,),dtype=np.int)
red_vout = np.zeros((2,),dtype=np.int)
red_tout = np.zeros((2,),dtype=np.int)
# for i in range(len(ir)):
for i,r in zip(ir,red):
	# Filter the raw IR signal
	temp = ir_lwdfilter.push(i)
	value = (temp[0]+temp[1]+(1<<0))>>1
	ir_out.append(value)
	# Detect peak
	res = ir_delin.write(value)
	if res!=0:
		temp = ir_delin.delineator.contents
		ir_timestamp += (temp.output.t)
		ir_accumulator += (temp.output.x)
		ir_q.append(temp.output.dx)
		ir_i.append(ir_accumulator)
		ir_t.append(ir_timestamp)		
	# Filter the raw red signal
	temp = red_lwdfilter.push(r)
	value = (temp[0]+temp[1]+(1<<0))>>1
	red_out.append(value)
	# Detect peak
	res = red_delin.write(value)
	if res!=0:
		temp = red_delin.delineator.contents
		red_timestamp += (temp.output.t)
		red_accumulator += (temp.output.x)
		red_q.append(temp.output.dx)
		red_i.append(red_accumulator)
		red_t.append(red_timestamp)

# Plot the data
plt.subplot(211)
plt.plot(ir_out, 'k-', linewidth=2.0)
plt.plot(red_out, 'r-', linewidth=2.0)
plt.grid()
plt.subplot(212)
plt.plot(np.array(ir_out[2:])-np.array(ir_out[:-2]), 'k-', linewidth=2.0)
plt.plot(np.array(red_out[2:])-np.array(red_out[:-2]), 'r-', linewidth=2.0)
plt.grid()
plt.figure()
plt.subplot(211)
plt.plot(ir_t,ir_i, 'k-', linewidth=2.0)
plt.plot(red_t,red_i, 'r-', linewidth=2.0)
plt.grid()
plt.subplot(212)
plt.plot(ir_t,ir_q, 'k-', linewidth=2.0)
plt.plot(red_t,red_q, 'r-', linewidth=2.0)
plt.grid()
plt.figure()
plt.subplot(121)
plt.plot(ir_i,ir_q, 'k-', linewidth=2.0)
plt.grid()
plt.subplot(122)
plt.plot(red_i,red_q, 'r-', linewidth=2.0)
plt.grid()
plt.show()
