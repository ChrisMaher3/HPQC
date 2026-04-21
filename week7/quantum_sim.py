import numpy as np

workspace = np.array([[1.]])
namestack = []

# ---------------- GATES ---------------- #

X_gate = np.array([[0, 1],
                   [1, 0]])

H_gate = np.array([[1, 1],
                   [1, -1]]) * np.sqrt(1/2)

CNOT_gate = np.array([[1, 0, 0, 0],
                      [0, 1, 0, 0],
                      [0, 0, 0, 1],
                      [0, 0, 1, 0]])

SWAP_gate = np.array([[1, 0, 0, 0],
                      [0, 0, 1, 0],
                      [0, 1, 0, 0],
                      [0, 0, 0, 1]])

TOFF_gate = np.array([[1, 0, 0, 0, 0, 0, 0, 0],
                      [0, 1, 0, 0, 0, 0, 0, 0],
                      [0, 0, 1, 0, 0, 0, 0, 0],
                      [0, 0, 0, 1, 0, 0, 0, 0],
                      [0, 0, 0, 0, 1, 0, 0, 0],
                      [0, 0, 0, 0, 0, 1, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 1],
                      [0, 0, 0, 0, 0, 0, 1, 0]])

# ---------------- CORE FUNCTIONS ---------------- #

def pushQubit(name, weights):
    global workspace, namestack

    if workspace.shape == (1, 1):
        namestack = []

    namestack.append(name)

    weights = np.array(weights)
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

    for n in names:
        tosQubit(n)

    workspace = np.reshape(workspace, (-1, gate.shape[0]))
    sub = workspace[:, -gate.shape[0]:]
    np.matmul(sub, gate.T, out=sub)


def probQubit(name):
    global workspace

    tosQubit(name)
    workspace = np.reshape(workspace, (-1, 2))

    prob = np.linalg.norm(workspace, axis=0) ** 2
    return prob / prob.sum()


def measureQubit(name):
    global workspace, namestack

    prob = probQubit(name)
    result = np.random.choice([0, 1], p=prob)

    workspace = workspace[:, [result]] / np.sqrt(prob[result])
    namestack.pop()

    return str(result)

# ---------------- DEMO ---------------- #

if __name__ == "__main__":

    print("Initial state:", workspace)

    # Hadamard test
    workspace = np.array([[1.]])
    pushQubit("Q1", [1, 0])

    applyGate(H_gate, "Q1")
    print("After H:", workspace)

    print("Measured:", measureQubit("Q1"))

    # Coin flip test (better randomness check)
    print("\nCoin flip test:")
    zeros = 0
    ones = 0

    for i in range(50):
        workspace = np.array([[1.]])
        pushQubit("coin", [1, 0])
        applyGate(H_gate, "coin")
        r = measureQubit("coin")

        if r == "0":
            zeros += 1
        else:
            ones += 1

    print("0s:", zeros, "1s:", ones)
