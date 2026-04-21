import numpy as np

# ======================
# STATE + STACK
# ======================
workspace = np.array([[1.]])
namestack = []

# ======================
# BASIC GATES
# ======================
X_gate = np.array([[0, 1],
                   [1, 0]])

Z_gate = np.array([[1, 0],
                   [0, -1]])

H_gate = np.array([[1, 1],
                   [1, -1]]) * np.sqrt(1/2)

CNOT_gate = np.array([[1, 0, 0, 0],
                      [0, 1, 0, 0],
                      [0, 0, 0, 1],
                      [0, 0, 1, 0]])

TOFF_gate = np.array([[1, 0, 0, 0, 0, 0, 0, 0],
                      [0, 1, 0, 0, 0, 0, 0, 0],
                      [0, 0, 1, 0, 0, 0, 0, 0],
                      [0, 0, 0, 1, 0, 0, 0, 0],
                      [0, 0, 0, 0, 1, 0, 0, 0],
                      [0, 0, 0, 0, 0, 1, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 1],
                      [0, 0, 0, 0, 0, 0, 1, 0]])


# ======================
# CORE SIMULATOR
# ======================
def pushQubit(name, weights):
    global workspace, namestack

    if workspace.shape == (1, 1):
        namestack = []

    namestack.append(name)

    weights = np.array(weights, dtype=workspace.dtype)
    weights = weights / np.linalg.norm(weights)

    workspace = np.reshape(workspace, (1, -1))
    workspace = np.kron(workspace, weights)


def tosQubit(name):
    global workspace, namestack

    k = len(namestack) - namestack.index(name)
    if k > 1:
        namestack.append(namestack.pop(-k))
        workspace = np.reshape(workspace, (-1, 2, 2**(k-1)))
        workspace = np.swapaxes(workspace, -2, -1)


def applyGate(gate, *names):
    global workspace

    if list(names) != namestack[-len(names):]:
        for n in names:
            tosQubit(n)

    workspace = np.reshape(workspace, (-1, 2**len(names)))
    sub = workspace[:, -gate.shape[0]:]

    np.matmul(sub, gate.T, out=sub)


# ======================
# MEASUREMENT
# ======================
def probQubit(name):
    global workspace

    tosQubit(name)
    workspace = np.reshape(workspace, (-1, 2))

    prob = np.linalg.norm(workspace, axis=0)**2
    return prob / prob.sum()


def measureQubit(name):
    global workspace, namestack

    prob = probQubit(name)
    result = np.random.choice([0, 1], p=prob)

    workspace = workspace[:, [result]] / np.sqrt(prob[result])
    namestack.pop()

    return str(result)


# ======================
# SIMPLE ORACLES (GROVER STYLE)
# ======================
def zero_phaseOracle(qubits):
    for q in qubits:
        applyGate(X_gate, q)

    applyGate(Z_gate, *namestack)

    for q in qubits:
        applyGate(X_gate, q)


def sample_phaseOracle(qubits):
    # flips phase based on a simple condition (example)
    applyGate(X_gate, qubits[1])
    applyGate(Z_gate, *namestack)
    applyGate(X_gate, qubits[1])


# ======================
# GROVER SEARCH
# ======================
def groverSearch(n, show_prob=False):
    global workspace, namestack

    workspace = np.array([[1.]])
    namestack = []

    qubits = list(range(n))
    optimal = int(np.pi/4 * np.sqrt(2**n))

    # initialise superposition
    for q in qubits:
        pushQubit(q, [1, 1])

    for _ in range(optimal):
        sample_phaseOracle(qubits)

        for q in qubits:
            applyGate(H_gate, q)

        zero_phaseOracle(qubits)

        for q in qubits:
            applyGate(H_gate, q)

        if show_prob:
            print(probQubit(qubits[0]))

    return ''.join(measureQubit(q) for q in reversed(qubits))


# ======================
# TEST RUN
# ======================
if __name__ == "__main__":

    print("Single qubit test (H gate)")
    workspace = np.array([[1.]])
    namestack = []

    pushQubit("q0", [1, 0])
    applyGate(H_gate, "q0")

    print("State:", workspace)

    print("Measured:", measureQubit("q0"))
    print()

    print("Grover demo (6 qubits)")
    result = groverSearch(6, show_prob=True)
    print("\nResult:", result)
