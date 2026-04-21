This program is a simple quantum computing simulator written in Python using NumPy. It is based on the stack-based approach from the ENCCS tutorial.

The simulator represents qubits as vectors and builds multi-qubit states using the Kronecker product. Quantum gates such as X, Hadamard, SWAP, and Toffoli are applied using matrix multiplication on the workspace.

The Hadamard gate always produces the same output state from a given input because quantum gates are deterministic transformations. For example, applying H to |0⟩ always results in [0.707, 0.707]. Randomness only appears during measurement, where the state collapses probabilistically based on calculated amplitudes.

To demonstrate variation, different input states can be used before applying gates (e.g. non-basis states like [0.6, 0.8]), which leads to different resulting superpositions and measurement outcomes.
