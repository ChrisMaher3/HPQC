In this code, the quantum simulator from week 8 was updated to use PyTorch tensors instead of NumPy arrays so it can run on either CPU or GPU hardware. The workspace is stored directly as a tensor on a device, which allows operations to be executed on a GPU when one is available.

Most of the changes were direct replacements of NumPy functions with PyTorch equivalents such as reshape, kron, swapaxes, matmul, and linalg.norm. The overall structure of the simulator, including the qubit stack system and Grover search algorithm, was kept the same.

If no GPU is available, the program automatically runs on the CPU instead and continues without errors. In this case, a message is printed to show that GPU acceleration is not being used.

The Grover search still produces the expected result, showing that the implementation works correctly even without GPU support. While no performance improvement can be measured on a CPU-only system, the code is written so it would run on a CUDA GPU without any modifications if available.

No GPU was available for testing, so the program was run on CPU only.
