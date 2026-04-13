import subprocess
import time
import matplotlib.pyplot as plt
import random

def run_test(exe, n):
    mat = [[random.randint(1, 100) if i != j else 0 for j in range(n)] for i in range(n)]
    inp = f"{n}\n" + "\n".join(" ".join(map(str, row)) for row in mat)
    start = time.time()
    subprocess.run(exe, input=inp, text=True, capture_output=True)
    return (time.time() - start) * 1000

sizes = list(range(3, 21))
t_main, t_alsh = [], []

for n in sizes:
    tm = [run_test("./main", n) for _ in range(3)]
    ta = [run_test("./alsh", n) for _ in range(3)]
    t_main.append(sum(tm) / len(tm))
    t_alsh.append(sum(ta) / len(ta))

scale_main = t_main[-1] / ((2 ** sizes[-1]) * (sizes[-1] ** 2))
scale_alsh = t_alsh[-1] / (sizes[-1] ** 4)
theor_main = [scale_main * (2 ** n) * (n ** 2) for n in sizes]
theor_alsh = [scale_alsh * (n ** 4) for n in sizes]

plt.figure(figsize=(10, 6))
plt.plot(sizes, t_main, 'bo-', label='./main')
plt.plot(sizes, theor_main, 'r--', label='O(n²·2ⁿ)')
plt.xlabel('n'); plt.ylabel('мс'); plt.legend(); plt.grid(True, alpha=0.3)
plt.savefig('graph1.png', dpi=100)

plt.figure(figsize=(10, 6))
plt.plot(sizes, t_alsh, 'go-', label='./alsh')
plt.plot(sizes, theor_alsh, 'r--', label='O(n⁴)')
plt.xlabel('n'); plt.ylabel('мс'); plt.legend(); plt.grid(True, alpha=0.3)
plt.savefig('graph2.png', dpi=100)