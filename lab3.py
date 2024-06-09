import numpy as np
from scipy.stats import chi2
from scipy.stats import norm
import time
import matplotlib.pyplot as plt

class RandomNumberGenerator:
    def __init__(self, seed):
        self.seed = seed
        self.state = seed

    def lcg(self, n, a=1664525, c=1013904221, m=2**32):
        numbers = []
        x = self.seed
        for _ in range(n):
            x = (a * x + c) % m
            numbers.append(x / m)
        return numbers

    def mersenne_twister(self, n):
        mt = np.zeros(624, dtype=np.uint32)
        mt[0] = self.seed
        for i in range(1, 624):
            mt[i] = (1812433253 * (mt[i-1] ^ (mt[i-1] >> 30)) + i) & 0xFFFFFFFF
        numbers = []
        for _ in range(n):
            if len(numbers) % 624 == 0:
                self.twist(mt)
            y = mt[len(numbers) % 624]
            y ^= y >> 11
            y ^= (y << 7) & 0x9D2C5680
            y ^= (y << 15) & 0xEFC60000
            y ^= y >> 18
            numbers.append(y / 2**32)
        return numbers

    def twist(self, mt):
        for i in range(624):
            y = (mt[i] & 0x80000000) + (mt[(i+1) % 624] & 0x7FFFFFFF)
            mt[i] = mt[(i + 397) % 624] ^ (y >> 1)
            if y % 2 != 0:
                mt[i] ^= 0x9908B0DF

def monobit_test(data):
    s = sum(1 if bit == '1' else -1 for bit in ''.join(format(int(x * (2**32)), '032b') for x in data))
    p_value = norm.cdf(s / np.sqrt(len(data)))
    return p_value

def poker_test(data):
    num_samples = len(data) // 5
    observed = {}
    for i in range(num_samples):
        sample = ''.join(str(int(x * (2**32)) % 10) for x in data[i*5:(i+1)*5])
        if sample in observed:
            observed[sample] += 1
        else:
            observed[sample] = 1
    chi_square_stat = sum((obs - num_samples * 0.01)**2 / (num_samples * 0.01) for obs in observed.values())
    p_value = 1 - chi2.cdf(chi_square_stat, 999)
    return p_value

def runs_test(data):
    runs = [data[0]]
    for i in range(1, len(data)):
        if data[i] != data[i-1]:
            runs.append(data[i])
    num_runs = len(runs)
    pi = sum(1 for bit in data if bit == 1) / len(data)
    mean = (2 * len(data) - 1) / 3
    std_dev = np.sqrt((16 * len(data) - 29) / 90)
    z = (num_runs - mean) / std_dev
    p_value = 1 - norm.cdf(z)
    return p_value

def calculate_statistics(data):
    mean = np.mean(data)
    std_dev = np.std(data)
    coeff_var = std_dev / mean
    return mean, std_dev, coeff_var

def chi_square_test(data, bins=10):
    observed, _ = np.histogram(data, bins=bins)
    expected = np.full_like(observed, fill_value=len(data) / bins)
    chi_square_stat = np.sum((observed - expected)**2 / expected)
    degrees_of_freedom = bins - 1
    p_value = 1 - chi2.cdf(chi_square_stat, degrees_of_freedom)
    return chi_square_stat, p_value

def measure_generation_time(generator, sample_sizes):
    times = []
    for size in sample_sizes:
        start_time = time.time()
        generator(size)
        end_time = time.time()
        times.append(end_time - start_time)
    return times

# Создание генераторов и генерация выборок
seed = 42
rng = RandomNumberGenerator(seed)
lcg_sample = rng.lcg(100000)
mt_sample = rng.mersenne_twister(100000)

lcg_stats = calculate_statistics(lcg_sample)
mt_stats = calculate_statistics(mt_sample)

# Проверка на равномерность распределения и случайность
lcg_chi2_stat, lcg_p_value = chi_square_test(lcg_sample)
mt_chi2_stat, mt_p_value = chi_square_test(mt_sample)

# Проведение тестов NIST
lcg_monobit_p_value = monobit_test(lcg_sample)
lcg_poker_p_value = poker_test(lcg_sample)
lcg_runs_p_value = runs_test(lcg_sample)

mt_monobit_p_value = monobit_test(mt_sample)
mt_poker_p_value = poker_test(mt_sample)
mt_runs_p_value = runs_test(mt_sample)



print("Линейный Конгруэнтный Генератор:")
print("Среднее: {:.4f}, Отклонение: {:.4f}, Коэффициент вариации: {:.4f}".format(*lcg_stats))
print("Хи-квадрат статистика: {:.4f}, p-value: {:.4f}".format(lcg_chi2_stat, lcg_p_value))
print("Тесты NIST для Линейного Конгруэнтного Генератора:")
print("Monobit test p-value:", lcg_monobit_p_value)
print("Poker test p-value:", lcg_poker_p_value)
print("Runs test p-value:", lcg_runs_p_value)

print("\nMersenne Twister:")
print("Среднее: {:.4f}, Отклонение: {:.4f}, Коэффициент вариации: {:.4f}".format(*mt_stats))
print("Хи-квадрат статистика: {:.4f}, p-value: {:.4f}".format(mt_chi2_stat, mt_p_value))
print("\nТесты NIST для Mersenne Twister:")
print("Monobit test p-value:", mt_monobit_p_value)
print("Poker test p-value:", mt_poker_p_value)
print("Runs test p-value:", mt_runs_p_value)


lcg_generator = lambda n: rng.lcg(n)
mt_generator = lambda n: rng.mersenne_twister(n)

# Задание размеров выборок
sample_sizes = [100, 1000, 10000, 100000, 1000000]

# Измерение времени генерации для каждого метода
lcg_times = measure_generation_time(lcg_generator, sample_sizes)
mt_times = measure_generation_time(mt_generator, sample_sizes)

print("\n")
print(lcg_times)
print(mt_times)

# Построение графиков
plt.figure(figsize=(10, 6))
plt.plot(sample_sizes, lcg_times, marker='o', label='LCG')
plt.plot(sample_sizes, mt_times, marker='o', label='Mersenne Twister')
plt.xlabel('Размер выборки')
plt.ylabel('Время генерации (сек)')
plt.title('Сравнение времени генерации для разных объемов выборок')
plt.legend()
plt.grid(True)
plt.xscale('log')
plt.yscale('log')
plt.show()