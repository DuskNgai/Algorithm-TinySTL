import numpy as np

class YoungTableau(object):
    """
    A tableau object is a m x n matrix with the following properties:
    1. Non-exist entries are represented by np.inf.
    2. Each row is sorted from left to right.
    3. Each column is sorted from top to bottom.
    4. The smallest entry is in the top left corner.
    5. The largest entry is in the bottom right corner.
    """
    def __init__(self, m: int, n: int):
        self.m = m
        self.n = n
        self.tableau = np.full((m, n), np.inf)

    def full(self):
        """Check if the tableau is full."""
        return self.tableau[-1, -1] == np.inf

    def empty(self):
        """Check if the tableau is empty."""
        return self.tableau[0, 0] == np.inf

    def top(self):
        """Return the smallest entry in the tableau."""
        return self.tableau[0, 0]

    def pop(self):
        """Return and delete the smallest entry in the tableau."""
        result = self.tableau[0, 0]
        self.tableau[0, 0] = np.inf

        r, c = 0, 0
        er, ec = 0, 0
        while True:
            if r + 1 < self.m and self.tableau[r + 1, c] < self.tableau[r, c]:
                er, ec = r + 1, c
            else:
                er, ec = r, c

            if c + 1 < self.n and self.tableau[r, c + 1] < self.tableau[er, ec]:
                er, ec = r, c + 1

            if er != r or ec != c:
                self.tableau[r, c], self.tableau[er, ec] = self.tableau[er, ec], self.tableau[r, c]
                r, c = er, ec
            else:
                break

        return result

    def push(self, value):
        """Insert a new entry into the tableau."""
        self.tableau[-1, -1] = value

        r, c = self.m - 1, self.n - 1
        er, ec = 0, 0
        while True:
            if r > 0 and self.tableau[r, c] < self.tableau[r - 1, c]:
                er, ec = r - 1, c
            else:
                er, ec = r, c

            if c > 0 and self.tableau[er, ec] < self.tableau[r, c - 1]:
                er, ec = r, c - 1

            if er != r or ec != c:
                self.tableau[er, ec], self.tableau[r, c] = self.tableau[r, c], self.tableau[er, ec]
                r, c = er, ec
            else:
                break

    def find(self, value):
        """Find the index of the given value in the tableau."""
        r, c = 0, self.n - 1
        while r < self.m and c >= 0:
            if value < self.tableau[r, c]:
                c -= 1
            elif value > self.tableau[r, c]:
                r += 1
            else:
                return r, c
        return None

    def __getitem__(self, index):
        return self.tableau[index]
