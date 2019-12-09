import matplotlib.pyplot as plt
import numpy as np
from random import random, randint

def k_mean(data, num_centroids):
	centroids = [(random()*5, random()*5) for _ in range(num_centroids)]
	for i in range(1):
		for c in centroids:
			np.linalg.norm(data-c, axis=1)


if __name__ == '__main__':
	centers = [(random()*5, random()*5)for i in range(3)]
	data = np.array([(c[0]+random(), c[1]+random()) for c in centers for _ in range(5)])
	print(data.shape)
	x = data[:, 0]
	y = data[:, 1]
	plt.plot(x, y, 'o')
	plt.show()


	k_mean(data, 3)
