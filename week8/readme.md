This program is a simple quantum computing simulator based on the ENCCS tutorial. It uses a statevector approach where all qubits are stored in a single NumPy array called the workspace, which is updated using tensor products and matrix operations.

The simulator supports basic quantum gates including X, Z and Hadamard, as well as multi-qubit gates like CNOT and Toffoli. Qubits are added using pushQubit and managed using a simple stack system that keeps track of their order.

Measurement is probabilistic and is calculated from the squared amplitudes of the statevector. Once a qubit is measured, the system collapses and the qubit is removed from the stack.

The GroverSearch function implements a simplified version of Grover’s algorithm. It uses an oracle to mark a target state and then applies repeated amplitude amplification using Hadamard transforms. After several iterations, the probability of the correct state becomes very high and is consistently measured as the output.

### Results

The simulator correctly demonstrates quantum behaviour such as superposition and probabilistic measurement. After applying the Hadamard gate, a qubit is placed into an equal superposition, and repeated measurements produce random outcomes consistent with a 50/50 probability.

In the Grover search example, the probability of the correct state increases with each iteration, eventually converging close to 1.0. This results in the correct bitstring being consistently measured (e.g. 111101 in the 6-qubit case), showing that amplitude amplification is working as expected.

Different runs may show variation in intermediate measurements, but the final Grover output remains stable due to the high probability of the target state.
