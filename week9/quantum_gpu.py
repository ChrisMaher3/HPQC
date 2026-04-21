import numpy as np
import torch as pt
import time

pt.autograd.set_grad_enabled(False)

# check GPU
if pt.cuda.is_available():
    print("GPU available")
else:
    print("No GPU, running on CPU only")

workspace = None
namestack = []

# basic gates
X_gate = np.array([[0, 1],
                   [1, 0]])

Z_gate = np.array([[1, 0],
                   [0, -1]])

H_gate = np.array([[1, 1],
                   [1, -1]]) * np.sqrt(1 / 2)


# -------------------------
# Core simulator functions
# -------------------------

def pushQubit(name, weights):
    global workspace, namestack

    if workspace is None or workspace.shape == (1, 1):
        namestack = []

    namestack.append(name)

    weights = weights / np.linalg.norm(weights)
    weights = pt.tensor(weights, device=workspace.device, dtype=workspace.dtype)

    workspace = pt.reshape(workspace, (1, -1))
    workspace = pt.kron(workspace, weights)


def tosQubit(name):
    global workspace, namestack

    k = len(namestack) - namestack.index(name)

    if k > 1:
        namestack.append(namestack.pop(-k))
        workspace = pt.reshape(workspace, (-1, 2, 2 ** (k - 1)))
        workspace = pt.swapaxes(workspace, -2, -1)


def applyGate(gate, *names):
    global workspace

    if list(names) != namestack[-len(names):]:
        for name in names:
            tosQubit(name)

    workspace = pt.reshape(workspace, (-1, 2 ** len(names)))

    subworkspace = workspace[:, -gate.shape[0]:]
    gate = pt.tensor(gate.T, device=workspace.device, dtype=workspace.dtype)

    if workspace.device.type == "cuda":
        pt.matmul(subworkspace, gate, out=subworkspace)
    else:
        subworkspace[:, :] = pt.matmul(subworkspace, gate)


def probQubit(name):
    global workspace

    tosQubit(name)
    workspace = pt.reshape(workspace, (-1, 2))

    prob = pt.linalg.norm(workspace, axis=0) ** 2
    prob = pt.Tensor.cpu(prob).numpy()

    return prob / prob.sum()


def measureQubit(name):
    global workspace, namestack

    prob = probQubit(name)
    measurement = np.random.choice(2, p=prob)

    workspace = workspace[:, [measurement]] / np.sqrt(prob[measurement])
    namestack.pop()

    return str(measurement)


# -------------------------
# Grover oracles
# -------------------------

def zero_phaseOracle(qubits):
    for q in qubits:
        applyGate(X_gate, q)

    applyGate(Z_gate, *namestack)

    for q in qubits:
        applyGate(X_gate, q)


def sample_phaseOracle(qubits):
    applyGate(X_gate, qubits[1])
    applyGate(Z_gate, *namestack)
    applyGate(X_gate, qubits[1])


# -------------------------
# Grover search
# -------------------------

def groverSearch(n, printProb=False):
    optimalTurns = int(np.pi / 4 * np.sqrt(2 ** n) - 1 / 2)
    qubits = list(range(n))

    for q in qubits:
        pushQubit(q, [1, 1])

    for _ in range(optimalTurns):
        sample_phaseOracle(qubits)

        for q in qubits:
            applyGate(H_gate, q)

        zero_phaseOracle(qubits)

        for q in qubits:
            applyGate(H_gate, q)

        if printProb:
            print(probQubit(qubits[0]))

    for q in reversed(qubits):
        print(measureQubit(q), end="")


# -------------------------
# Main demo
# -------------------------

if __name__ == "__main__":

    # 6 qubit run
    print("\n6 qubit Grover search (GPU demo)")
    workspace = pt.tensor([[1.]], device=pt.device("cuda" if pt.cuda.is_available() else "cpu"),
                          dtype=pt.float32)

    groverSearch(6, printProb=True)
    print("\n")

    # timing comparison
    print("16 qubit Grover search (GPU)")
    workspace = pt.tensor([[1.]], device=pt.device("cuda"), dtype=pt.float32)

    t = time.perf_counter()
    groverSearch(16, printProb=False)
    gpu_time = time.perf_counter() - t

    print(f"\nGPU time: {gpu_time:.4f} s\n")

    print("16 qubit Grover search (CPU)")
    workspace = pt.tensor([[1.]], device=pt.device("cpu"), dtype=pt.float32)

    t = time.perf_counter()
    groverSearch(16, printProb=False)
    cpu_time = time.perf_counter() - t

    print(f"\nCPU time: {cpu_time:.4f} s\n")

    print(f"Speedup: {cpu_time / gpu_time:.2f}x")
