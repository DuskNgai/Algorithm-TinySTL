"""
Using Box Muller to implement normal distribution.
"""
import numpy as np
import matplotlib.pyplot as plt

n = 5000
U1 = np.random.random_sample((n, ))
U2 = np.random.random_sample((n, )) * 2 * np.pi
Z1 = np.sqrt(-2 * np.log(U1)) * np.sin(U2)
Z2 = np.sqrt(-2 * np.log(U1)) * np.cos(U2)

plt.figure(figsize = (8, 8))
plt.axis([-4, 4, -4, 4])
plt.scatter(Z1, Z2, color = 'cornflowerblue', marker = '.', linewidths = 0.5)
plt.tight_layout()
plt.show()
