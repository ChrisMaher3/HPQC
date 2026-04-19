import time
import sys

n = int(sys.argv[1])

start = time.time()

with open("../../data/output.txt", "w") as f:
    for i in range(n):
        f.write(f"{i}\n")

end = time.time()

print("Write time:", end - start)
