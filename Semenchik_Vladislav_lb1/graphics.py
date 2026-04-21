import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np

def run_test(N):
    start = time.time()
    subprocess.run(f"echo {N} | ./main", shell=True, capture_output=True)
    end = time.time()
    return (end - start) * 1000

def main():
    sizes = list(range(3, 30, 2))
    avg_times = []
    
    for N in sizes:
        times = []
        for run in range(5):
            t = run_test(N)
            times.append(t)
            print(f"  N={N}, run={run+1}: {t:.2f} ms")
        
        avg = np.mean(times)
        avg_times.append(avg)
        print(f"N={N}: average = {avg:.2f} ms\n")
    
    theoretical = [2**(N/2) for N in sizes]
    scale = avg_times[0] / theoretical[0]
    theoretical_scaled = [t * scale for t in theoretical]
    
    plt.figure(figsize=(10, 6))
    plt.plot(sizes, avg_times, 'bo-', linewidth=2, markersize=8, label='Real time (ms)')
    plt.plot(sizes, theoretical_scaled, 'r--', linewidth=2, label='O(2^N)')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.xlabel('N', fontsize=12)
    plt.ylabel('Time (ms)', fontsize=12)
    plt.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig('graph.png', dpi=100)
    
    with open('data.txt', 'w') as f:
        for N, t in zip(sizes, avg_times):
            f.write(f"{N} {t:.2f}\n")

if __name__ == "__main__":
    main()