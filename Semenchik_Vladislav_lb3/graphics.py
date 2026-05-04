import subprocess
import time
import matplotlib.pyplot as plt

def run_test(exe, n):
    import random
    A = ''.join(random.choice('abcdefghijklmnopqrstuvwxyz') for _ in range(n))
    B = ''.join(random.choice('abcdefghijklmnopqrstuvwxyz') for _ in range(n))
    
    inp = f"1 1 1 1\n{A}\n{B}\n"
    
    start = time.time()
    subprocess.run(exe, input=inp, text=True, capture_output=True)
    return (time.time() - start) * 1000

sizes = list(range(10, 500))
times = []

for n in sizes:
    runs = [run_test("./a.out", n) for _ in range(3)]
    avg_time = sum(runs) / len(runs)
    times.append(avg_time)
    print(f"n={n}: {avg_time:.2f} ms")


scale = times[-1] / (sizes[-1] * sizes[-1])
theor = [scale * (n * n) for n in sizes]

plt.figure(figsize=(10, 6))
plt.plot(sizes, times, 'bo-', label='Actual time')
plt.plot(sizes, theor, 'r--', label='O(n·m) = O(n²)')
plt.xlabel('String length (n = m)')
plt.ylabel('Time (ms)')
plt.legend()
plt.grid(True, alpha=0.3)
plt.savefig('graph.png', dpi=100)