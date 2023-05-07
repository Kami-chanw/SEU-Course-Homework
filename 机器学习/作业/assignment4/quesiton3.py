import numpy as np

algorithms = ['A', 'B', 'C', 'D', 'E']
ranks = np.array([[2, 3, 1, 5, 4], [5, 4, 2, 3, 1], [4, 5, 1, 2, 3],
                  [2, 3, 1, 5, 4], [3, 4, 1, 5, 2]])
average_ranks = np.array([3.2, 3.8, 1.2, 4, 2.8])

# calculate Friedman statistic
N, k = ranks.shape
Friedman_statistic = 12 * N / (k * (k + 1)) * (np.sum(
    (average_ranks**2)) - k * (k + 1)**2 / 4)

from scipy.stats import f

F_statistic = (N - 1) * Friedman_statistic / (N * (k - 1) - Friedman_statistic)
p_value = 1 - f.cdf(F_statistic, k - 1, (N - 1) * (k - 1))

print(Friedman_statistic, F_statistic)

alpha = 0.05
if p_value < alpha:
    print(
        "According to the Friedman test, the performance of all algorithms is not the same"
    )
    q = 2.728
    CD = q * np.sqrt(k * (k + 1) / (6 * N))
    print(CD)
    mean_ranks_diff = np.abs(np.subtract.outer(average_ranks, average_ranks))
    np.fill_diagonal(mean_ranks_diff, np.inf)
    significant_diff = mean_ranks_diff > CD
    for i in range(k):
        for j in range(i + 1, k):
            if significant_diff[i, j]:
                print(
                    f"According to the Nemenyi test, algorithm {algorithms[i]} and algorithm {algorithms[j]} have significant difference in performance"
                )
else:
    print("All algorithms have the same performance as tested by Friedman")
