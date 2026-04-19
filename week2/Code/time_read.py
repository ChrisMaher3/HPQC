import time

start = time.time()

with open("../../data/output.txt", "r") as f:
    data = f.readlines()

end = time.time()

print("Read time:", end - start)
