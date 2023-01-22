"""
Named distribution learned in Probability and Statistics.
"""
import numpy as np
import matplotlib.pyplot as plt
import scipy.special

#! <<<---------- Discrete ---------->>>
class Discrete(object):

    @staticmethod
    def Bern(p: float) -> bool:
        assert 0 <= p <= 1, "p should be in [0, 1]."

        return np.random.random_sample() < p

    @staticmethod
    def Bin(n, p: float) -> int:
        assert 0 <= p <= 1, "p should be in [0, 1]."

        return np.count_nonzero(np.random.random_sample(n) < p)

    @staticmethod
    def Geom(p: float) -> int:
        assert 0 <= p <= 1, "p should be in [0, 1]."

        total = 0
        while not Discrete.Bern(p):
            total += 1

        return total

    @staticmethod
    def FS(p: float) -> int:
        assert 0 <= p <= 1, "p should be in [0, 1]."

        return Discrete.Geom(p) + 1

    @staticmethod
    def NBin(r: int, p: float) -> int:
        assert 0 <= p <= 1, "p should be in [0, 1]."
        assert isinstance(r, int) and r > 0, "r should be positive integer."

        total = 0
        for _ in range(r):
            total += Discrete.Geom(p)

        return total

    @staticmethod
    def HGeom(w: int, b: int, n: int):
        assert w >= 0, "w should be non negative."
        assert b >= 0, "b should be non negative."
        assert w + b >= n, "n should be no more than w + b."

        localW = w
        localB = b
        total = 0

        for _ in range(n):
            if Discrete.Bern(localW / (localW + localB)) == True:
                total += 1
                localW -= 1
            else:
                localB -= 1

        return total

    @staticmethod
    def Pois(L: float) -> int:
        assert L > 0, "L should be greater than 0."

        total = 0
        temp = Continuous.Expo(L)
        while temp < 1:
            total += 1
            temp += Continuous.Expo(L)

        return total

    @staticmethod
    def draw(index, pmf, real_pmf, n_samples, *args):

        if len(args) == 2:
            out_str = "{}({})".format(*args)
        else:
            out_str = "{}{}".format(args[0], args[1:])

        fig = plt.figure(figsize = (12, 8), tight_layout=True)
        ax1 = fig.add_subplot(1, 2, 1)
        ax1.bar(index, pmf, color = "yellowgreen", label = "PMF")
        ax1.set_ylabel("PMF")
        ax1.set_ylim(0, np.max(real_pmf) * n_samples * 1.05)
        ax1.legend(bbox_to_anchor = (-0.05, 1.05), loc = "upper left")

        ax2 = ax1.twinx()
        ax2.plot(index, real_pmf, color = "indianred", label = "real PMF")
        ax2.set_ylabel("real PMF")
        ax2.set_ylim(0, np.max(real_pmf) * 1.05)
        ax2.set_title("PMF of " + out_str)
        ax2.legend(bbox_to_anchor = (1.05, 1.05), loc = "upper right")

        ax3 = fig.add_subplot(1, 2, 2)
        ax3.bar(index, np.cumsum(pmf), color = "yellowgreen", label = "CDF")
        ax3.set_ylim(0, n_samples * 1.05)
        ax3.legend(bbox_to_anchor = (-0.05, 1.05), loc = "upper left")

        ax4 = ax3.twinx()
        ax4.plot(index, np.cumsum(real_pmf), color = "indianred", label = "real CDF")
        ax4.set_ylabel("real CDF")
        ax4.set_ylim(0, 1.05)
        ax4.set_title("CDF of " + out_str)
        ax4.legend(bbox_to_anchor = (1.05, 1.05), loc = "upper right")

        plt.show()

#! <<<---------- Continuous ---------->>>
class Continuous(object):

    @staticmethod
    def Unif(a: float, b: float) -> float:
        assert a <= b, "a should be no greater than b"

        return (b - a) * np.random.random_sample() + a

    @staticmethod
    def Expo(L: float) -> float:
        assert L > 0, "L should be greater than 0."

        return -(np.log(Continuous.Unif(0, 1)) / L)

    @staticmethod
    def Normal(mu: float, sigma: float) -> float:
        assert sigma >= 0, "sigma should be no less than 0"
        
        X = Continuous.Unif(0, 1)
        Y = Continuous.Unif(0, 1) * 2 * np.pi
        Z = np.sqrt(-2 * np.log(X)) * np.sin(Y) * sigma + mu

        return Z

    @staticmethod
    def Beta(a: float, b: float) -> float:
        assert a > 0, "a should be greater than 0"
        assert b > 0, "b should be greater than 0"

        X1 = Continuous.Gamma(a, 1)
        X2 = Continuous.Gamma(b, 1)
        
        Y = X1 / (X1 + X2)
        return Y

    @staticmethod
    def Gamma(a: float, L: float) -> float:
        assert a > 0, "a should be greater than 0"
        assert L > 0, "L should be greater than 0"

        # i.i.d Expo(L) is restrictive 
        # since "a" may not be an integer
        
        integer = int(a)
        total = 0
        for _ in range(integer):
            total += Continuous.Expo(L)
        a -= integer
        if a == 0:
            return total

        y = 0
        b = 1 + a / np.e
        while True:
            U = Continuous.Unif(0, 1)
            P = Continuous.Unif(0, 1) * b
            
            if P > 1:
                y = -np.log((b - P) / a)
                if U < np.power(y, a - 1):
                    return y * L + total
            else:
                y = np.power(P, 1 / a)
                if U < np.exp(-y):
                    return y * L + total

    @staticmethod
    def draw(index, n_bins, samples, real_pdf, real_cdf, *args):

        if len(args) == 2:
            out_str = "{}({})".format(*args)
        else:
            out_str = "{}{}".format(args[0], args[1:])

        plt.figure(figsize = (12, 8), tight_layout = True)

        plt.subplot(2, 2, (1, 3))
        plt.hist(samples, bins = n_bins, color = "r", alpha = 0.4, edgecolor = "y", label = "sample")
        plt.title("Sample of " + out_str)
        plt.legend(bbox_to_anchor = (-0.1, 1.1), loc = "upper left")

        plt.subplot(2, 2, 2)
        plt.plot(index, real_pdf, color = "indigo", label = "PDF")
        plt.title("PDF of " + out_str)
        plt.legend(bbox_to_anchor = (1.1, 1.15), loc = "upper right")

        plt.subplot(2, 2, 4)
        plt.plot(index, real_cdf, color = "lightseagreen", label = "CDF")
        plt.title("CDF of " + out_str)
        plt.legend(bbox_to_anchor = (1.1, 1.15), loc = "upper right")

        plt.show()


if __name__ == "__main__":

    n_samples = 10000
    n_bins = 500

    #! <<<---------- Bern ---------->>>
    p = 0.3
    samples = np.asarray([Discrete.Bern(p) for _ in range(n_samples)])

    index = np.arange(0, 2, dtype = float)
    pmf = np.asarray([np.sum(samples == False), np.sum(samples == True)])
    real_pmf = np.asarray([(1 - p), p])

    Discrete.draw(index, pmf, real_pmf, n_samples, "Bern", p)

    #! <<<---------- Bin ---------->>>
    p, n = 0.4, 20
    samples = np.asarray([Discrete.Bin(n, p) for _ in range(n_samples)])

    index = np.arange(0, n + 1, dtype = float)
    pmf = np.asarray([np.sum(samples == i) for i in index])
    real_pmf = scipy.special.comb(n, index) * p ** index * (1 - p) ** (n - index)

    Discrete.draw(index, pmf, real_pmf, n_samples, "Bin", n, p)

    #! <<<---------- Geom ---------->>>
    p = 0.3
    samples = np.asarray([Discrete.Geom(p) for _ in range(n_samples)])

    index = np.arange(0, np.max(samples) + 1, dtype = float)
    pmf = np.asarray([np.sum(samples == i) for i in index])
    real_pmf = (1 - p) ** index * p

    Discrete.draw(index, pmf, real_pmf, n_samples, "Geom", p)

    #! <<<---------- FS ---------->>>
    p = 0.2
    samples = np.asarray([Discrete.FS(p) for _ in range(n_samples)])

    index = np.arange(1, np.max(samples) + 1, dtype = float)
    pmf = np.asarray([np.sum(samples == i) for i in index])
    real_pmf = (1 - p) ** (index - 1) * p

    Discrete.draw(index, pmf, real_pmf, n_samples, "FS", p)

    #! <<<---------- NBin ---------->>>
    p, r = 0.2, 10
    samples = np.asarray([Discrete.NBin(r, p) for _ in range(n_samples)])

    index = np.arange(0, np.max(samples) + 1, dtype = float)
    pmf = np.asarray([np.sum(samples == i) for i in index])
    real_pmf = scipy.special.comb(index + r - 1, r - 1) * (1 - p) ** index * p ** r

    Discrete.draw(index, pmf, real_pmf, n_samples, "NBin", r, p)

    #! <<<---------- HGeom ---------->>>
    w, b, n = 30, 30, 20
    samples = np.asarray([Discrete.HGeom(w, b, n) for _ in range(n_samples)])

    index = np.arange(0, n + 1, dtype = float)
    pmf = np.asarray([np.sum(samples == i) for i in index])
    real_pmf = scipy.special.comb(w, index) * scipy.special.comb(b, n - index) / scipy.special.comb(w + b, n)

    Discrete.draw(index, pmf, real_pmf, n_samples, "HGeom", w, b, n)

    #! <<<---------- Pois ---------->>>
    L = 100
    samples = np.asarray([Discrete.Pois(L) for _ in range(n_samples)])

    index = np.arange(0, np.max(samples) + 1, dtype = float)
    pmf = np.asarray([np.sum(samples == i) for i in index])
    real_pmf = np.exp(-L) * L ** index / scipy.special.factorial(index)

    Discrete.draw(index, pmf, real_pmf, n_samples, "Pois", L)

    #! <<<---------- Unif ---------->>>
    a, b = 0, 5
    samples = np.asarray([Continuous.Unif(a, b) for _ in range(n_samples)])

    index = np.linspace(a, b, n_bins)
    real_pdf = np.ones((n_bins, )) * (1 / (b - a))
    real_cdf = (index - a) / (b - a)

    Continuous.draw(index, n_bins, samples, real_pdf, real_cdf, "Unif", a, b)

    #! <<<---------- Expo ---------->>>
    L = 1
    samples = np.asarray([Continuous.Expo(L) for _ in range(n_samples)])

    index = np.linspace(0, np.max(samples), n_bins)
    real_pdf = L * np.exp(- L * index)
    real_cdf = 1 - np.exp(- L * index)

    Continuous.draw(index, n_bins, samples, real_pdf, real_cdf, "Expo", L)

    #! <<<---------- Normal ---------->>>
    mu, sigma = 10, 5
    samples = np.asarray([Continuous.Normal(mu, sigma) for _ in range(n_samples)])

    index = np.linspace(np.min(samples), np.max(samples), n_bins)
    real_pdf = (1 / np.sqrt(2 * np.pi * sigma ** 2)) * np.exp(-(index - mu) ** 2 / (2 * sigma ** 2))
    real_cdf = np.cumsum(real_pdf) / np.sum(real_pdf)

    Continuous.draw(index, n_bins, samples, real_pdf, real_cdf, "Normal", mu, sigma)

    #! <<<---------- Beta ---------->>>
    a, b = 0.5, 2
    samples = np.asarray([Continuous.Beta(a, b) for _ in range(n_samples)])

    index = np.linspace(0, 1, n_bins)[1:-1]
    real_pdf = index ** (a - 1) * (1 - index) ** (b - 1) / scipy.special.beta(a, b)
    real_cdf = np.cumsum(real_pdf) / np.sum(real_pdf)

    Continuous.draw(index, n_bins, samples, real_pdf, real_cdf, "Beta", a, b)

    #! <<<---------- Gamma ---------->>>
    a, L = 5.5, 1
    samples = np.asarray([Continuous.Gamma(a, L) for _ in range(n_samples)])

    index = np.linspace(0, np.max(samples), n_bins)[1:]
    real_pdf = (L * index) ** a * np.exp(-L * index) / (index * scipy.special.gamma(a))
    real_cdf = np.cumsum(real_pdf) / np.sum(real_pdf)

    Continuous.draw(index, n_bins, samples, real_pdf, real_cdf, "Gamma", a, L)
