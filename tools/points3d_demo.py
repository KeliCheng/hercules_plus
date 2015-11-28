import random
from matplotlib import pyplot
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import sys

if __name__ == "__main__":
	if len(sys.argv) >= 2:
		path = sys.argv[1];
	else:
		path = raw_input("== Enter the path to data file: ")


	mypoints = []
	try:
		x, y, z, vs = np.loadtxt(path, delimiter = ',', unpack = True)
	except IOError:
		print "[ERROR]: no such file."
		sys.exit()

	for i in range(len(x)):
	    mypoints.append([x[i],    #x
	                    y[i],     #y
	                    z[i],     #z
	                    vs[i]]) #scalar

	data = zip(*mypoints)

	fig = pyplot.figure()
	ax = fig.add_subplot(111, projection='3d')

	ax.scatter(data[0], data[1], data[2], c=data[3])
	pyplot.show()