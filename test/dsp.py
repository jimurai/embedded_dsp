"""
Filename: bb_dsp.py
Bindings to DSP C code that I (JACP) am using in my thesis:

"""
import numpy as np
from scipy import signal
import ctypes, sys
# Load the library as _libdsp.
# Why the underscore (_) in front of _libdsp below?
# To mimimise namespace pollution -- see PEP 8 (www.python.org).
_libdsp = None
for directory in sys.path:
    try:
        _libdsp = np.ctypeslib.load_library('libembedded_dsp', directory)
        break
    except:
        pass
if _libdsp == None: raise EnvironmentError('Cannot find the DLL.')

# void lwdf_gamma2alpha(float gamma, LWDF_ALPHA *alpha, uint8_t *type)
_libdsp.lwdf_gamma2alpha.argtypes = [	ctypes.c_float,\
										np.ctypeslib.ndpointer(dtype = np.uint16),\
										np.ctypeslib.ndpointer(dtype = np.uint8)]
_libdsp.lwdf_gamma2alpha.restype =		None
# void lwdf_initFilter(LWDF_FILTER* filter, uint8_t order, LWDF_ALPHA *alpha, uint8_t *type);
_libdsp.lwdf_initFilter.argtypes = [ctypes.c_void_p,\
									ctypes.c_uint8,\
									np.ctypeslib.ndpointer(dtype = np.uint16),\
									np.ctypeslib.ndpointer(dtype = np.uint8)]# Declare arg type, same below.
_libdsp.lwdf_initFilter.restype =	None
# LWDF_FILTER* lwdf_newFilter(void)
_libdsp.lwdf_newFilter.argtypes = []
_libdsp.lwdf_newFilter.restype = ctypes.c_void_p

# void lwdf_write(LWDF_FILTER* filter, LWDF_TYPE input)
_libdsp.lwdf_write.argtypes = [	ctypes.c_void_p,\
								ctypes.c_int16]
_libdsp.lwdf_write.restype =	ctypes.c_void_p

# LWDF_TYPE* lwdf_read(LWDF_FILTER* filter)
_libdsp.lwdf_read.argtypes = [ctypes.c_void_p]
_libdsp.lwdf_read.restype = ctypes.POINTER(ctypes.c_int32)

# LWDF_TYPE lwdf_read_lpf(LWDF_FILTER* filter)
_libdsp.lwdf_read_lpf.argtypes = [ctypes.c_void_p]
_libdsp.lwdf_read_lpf.restype = ctypes.c_int32

# LWDF_TYPE lwdf_read_hpf(LWDF_FILTER* filter)
_libdsp.lwdf_read_hpf.argtypes = [ctypes.c_void_p]
_libdsp.lwdf_read_hpf.restype = ctypes.c_int32

# Peak Detector memory structure
class PD(ctypes.Structure):
	_fields_ = [("peak", ctypes.c_int32),
				("output", ctypes.c_int32),
				("delta", ctypes.c_int32),
				("mode", ctypes.c_char),
				("state", ctypes.c_char)]
				
# PEAK_DETECTOR* pd_new(void);
_libdsp.pd_new.argtypes = []
_libdsp.pd_new.restype = ctypes.POINTER(PD)
			
# void pd_init(PEAK_DETECTOR* detector,PEAK_VALUE delta);
_libdsp.pd_init.argtypes = [	ctypes.POINTER(PD),\
										ctypes.c_int32]
_libdsp.pd_init.restype =		None

# void pd_del(PEAK_DETECTOR* detector);
_libdsp.pd_del.argtypes = [ctypes.POINTER(PD)]
_libdsp.pd_del.restype = None

# void pd_write(PEAK_DETECTOR* detector, PEAK_VALUE value);
_libdsp.pd_write.argtypes = [	ctypes.POINTER(PD),\
								ctypes.c_int32]
_libdsp.pd_write.restype =		None

# void pd_reset(PEAK_DETECTOR* detector, PEAK_VALUE value, PEAK_MODE mode);
_libdsp.pd_reset.argtypes = [	ctypes.POINTER(PD),\
								ctypes.c_int32,\
								ctypes.c_char]
_libdsp.pd_reset.restype =		None

class IPD(ctypes.Structure):
	_fields_ = [("down", ctypes.c_uint32),
				("up", ctypes.c_uint32),
				("hold", ctypes.c_uint32),
				("output", ctypes.c_uint32),
				("detector", PD)]
				
# I_PEAK_DETECTOR* ipd_new(void);
_libdsp.ipd_new.argtypes = []
_libdsp.ipd_new.restype = ctypes.POINTER(IPD)
			
# void ipd_init(I_PEAK_DETECTOR* detector,PEAK_VALUE delta);
_libdsp.ipd_init.argtypes = [	ctypes.POINTER(IPD),\
								ctypes.c_int32]
_libdsp.ipd_init.restype =		None

# void ipd_del(I_PEAK_DETECTOR* detector);
_libdsp.ipd_del.argtypes = [ctypes.POINTER(IPD)]
_libdsp.ipd_del.restype = None

# void ipd_write(I_PEAK_DETECTOR* detector, PEAK_VALUE value, PEAK_VALUE integrand);
_libdsp.ipd_write.argtypes = [	ctypes.POINTER(IPD),\
								ctypes.c_int32,\
								ctypes.c_int32]
_libdsp.ipd_write.restype =		None

# PPG Quadrature Delineation
class DD(ctypes.Structure):
	_fields_ = [("value", ctypes.c_int32),
				("log", ctypes.c_int32),
				("time", ctypes.c_uint32)]
class QUADRATURE(ctypes.Structure):
	_fields_ = [("phase", ctypes.c_uint8*2),
				("peak", ctypes.c_int32*2),
				("time", ctypes.c_uint32*4),
				("log", ctypes.c_uint32*4)]
class DELINEATOR(ctypes.Structure):
	_fields_ = [("previous_value", ctypes.c_int32),
				("delta", ctypes.c_int32*2),
				("track", DD*2),
				("output", DD),
				("quad", QUADRATURE)]

# DELINEATOR* quad_new(void);
_libdsp.quad_new.argtypes = []
_libdsp.quad_new.restype = ctypes.POINTER(DELINEATOR)

# void quad_del(DELINEATOR* delineator);
_libdsp.quad_del.argtypes = [ctypes.POINTER(DELINEATOR)]
_libdsp.quad_del.restype = None
				
# void quad_init(DELINEATOR* delineator, DELIN_VALUE delta_raw, DELIN_VALUE delta_diff);
_libdsp.quad_init.argtypes = [	ctypes.POINTER(DELINEATOR),\
								ctypes.c_int32,\
								ctypes.c_int32]
_libdsp.quad_init.restype =		None

# uint8_t quad_write(DELINEATOR* delineator, DELIN_VALUE value);
_libdsp.quad_write.argtypes = [	ctypes.POINTER(DELINEATOR),\
								ctypes.c_int32]
_libdsp.quad_write.restype =	ctypes.c_uint8

class Filter():
	def __init__(self, order=3, wn=0.01, rp=1, rs=60, ftype='butter'):
		if order%2 == 0:
			print 'No even order filters allowed.'
			return
		# Filter settings
		self.order = order
		self.wn = wn
		self.rp = rp
		self.rs = rs
		self.ftype = ftype
		gammas = []
		psi = np.tan(np.pi*self.wn/2.0)
		psi2 = psi*psi
		if self.ftype == 'butter':
			(z,p,k) = signal.iirfilter(self.order, psi, btype='lowpass', analog=1, ftype='butter', output='zpk')
			filter_roots = np.sort(p)
		elif self.ftype == 'bessel':
			print 'Please avoid using Bessel filters as they don\'t translate well to LWDFs.'
			(z,p,k) = signal.iirfilter(self.order, psi, btype='lowpass', analog=1, ftype='bessel', output='zpk')
			filter_roots = np.sort(p)
		elif self.ftype == 'cheby1':
			(z,p,k) = signal.iirfilter(self.order, psi, rp=1, btype='lowpass', analog=1, ftype='cheby1', output='zpk')
			filter_roots = np.sort(p)
		elif self.ftype == 'cheby2':
			(z,p,k) = signal.iirfilter(self.order, psi, rs=self.rs, btype='lowpass', analog=1, ftype='cheby2', output='zpk')
			filter_roots = np.sort(p)
		else:
			print 'Invalid filter type.'
			return
		# Separate the real pole from the complex poles
		real_index = 0;
		for i in range(self.order):
			if abs(filter_roots[i].imag) <= 1e-16:
				real_index = i
				break
		complex_roots = np.concatenate((filter_roots[0:real_index],filter_roots[real_index+1:]))
		# Put in the real pole's gamma value
		h_B = -1.0*filter_roots[real_index].real
		gammas.append((1.0 - h_B) / (1.0 + h_B))
		# Calculate coefficients of the individual Hurwitz polynomials
		for i in (range((order-1)/2)):
			h_A = -2.0*(complex_roots[2*i].real)
			h_B = abs(complex_roots[2*i])**2
			gammas.append((h_A - h_B - 1.0)/(h_A + h_B + 1.0))
			gammas.append((1.0 - h_B) / (1.0 + h_B))
		# Construct filter
		gammas_out = np.asarray(gammas,dtype=np.float32)
		alphas = []
		types = []
		for x in gammas:
			print '{0:.10f}'.format(x)
			alpha = np.array((1,),dtype = np.uint16)
			type = np.array((1,),dtype = np.uint8)
			_libdsp.lwdf_gamma2alpha(x,alpha,type)
			alphas.append(alpha)
			types.append(type)
		alphas_out = np.asarray(alphas,dtype=np.uint16)
		types_out = np.asarray(types,dtype=np.uint8)
		print alphas_out.transpose()
		print types_out.transpose()
		self.filter = _libdsp.lwdf_newFilter()
		_libdsp.lwdf_initFilter(self.filter,self.order,alphas_out,types_out)
			
	def push(self, value):
		_libdsp.lwdf_write(self.filter,value)
		filter_output = ctypes.c_int32*2
		filter_output = _libdsp.lwdf_read(self.filter)
		return filter_output

class PeakDetector():
	def __init__(self, delta=50):
		self.detector = _libdsp.pd_new()
		_libdsp.pd_init(self.detector,delta)

	def __del__(self):
		_libdsp.pd_del(self.detector)

	def write(self,value):
		_libdsp.pd_write(self.detector,value)

class IPeakDetector():
	def __init__(self, delta=50):
		self.detector = _libdsp.ipd_new()
		_libdsp.ipd_init(self.detector,delta)

	def __del__(self):
		_libdsp.ipd_del(self.detector)

	def write(self,value,integrand):
		_libdsp.ipd_write(self.detector,value,integrand)

class QuadDelineator():
	def __init__(self, delta_raw=50, delta_diff=1):
		self.delineator = _libdsp.quad_new()
		_libdsp.quad_init(self.delineator,delta_raw,delta_diff)

	def __del__(self):
		_libdsp.quad_del(self.delineator)

	def write(self,value):
		return _libdsp.quad_write(self.delineator,value)