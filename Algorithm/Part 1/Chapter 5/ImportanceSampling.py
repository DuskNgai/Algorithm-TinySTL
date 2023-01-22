"""
Using important sampling to get the probability of normal tail distribution.
"""
import numpy as np
from scipy.stats import norm

u = 8
print("Theoretically, Phi({}) = {}".format(u, norm.cdf(-u)))

n = 100000
U1 = np.random.random_sample((n, ))
U2 = np.random.random_sample((n, )) * 2 * np.pi
Z1 = np.sqrt(-2 * np.log(U1)) * np.sin(U2)
more_than_u = np.count_nonzero(Z1 > u)
print("Without importance sampling: P(Y > {}) = {}".format(u, more_than_u))

# g ~ N(u, 1)
G = Z1 + u
more_than_u = G[G > u]
c = np.sum(np.exp(0.5 * u ** 2 - u * more_than_u)) / n
print("With importance sampling: P(Y > {}) = {}".format(u, c))
