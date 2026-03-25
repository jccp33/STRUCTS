# 📦 STRUCTS Library

**A High-Performance, Multi-Language Data Structures Library in C11**

<p align="center">
  <img src="./assets/image.gif" alt="STRUCTS Library Visualizations (Linked List, Hash Table, Graph)" width="70%">
</p>

**STRUCTS** is a lightweight, zero-dependency shared library providing robust implementations of fundamental data structures. Designed with a focus on **interoperability**, it allows high-level languages like Python and C\# to leverage C's raw speed.

## 🚀 Key Features

  * **Generic Storage:** All structures handle `void*`, allowing you to store any data type.
  * **Opaque Pointers:** Strict encapsulation to prevent memory corruption from external calls.
  * **Universal Persistence:** Built-in module to save/load structures from disk (`.csv`, `.txt`).
  * **Cross-Language:** Ready to use via `ctypes` (Python), `P/Invoke` (C\#), or native C++.

-----

## 🛠️ Data Structures & Complexity

| Structure | Best Case | Worst Case | Memory |
| :--- | :--- | :--- | :--- |
| **LinkedList** | $O(1)$ | $O(n)$ | $O(n)$ |
| **HashTable** | $O(1)$ | $O(n)$ | $O(n)$ |
| **AVL Tree** | $O(\log n)$ | $O(\log n)$ | $O(n)$ |
| **Graph (BFS)** | - | $O(V + E)$ | $O(V^2)$ |

-----

## 🏗️ Building from Source

### 1\. Compile Objects

```bash
make
```

### 2\. Generate Shared Library (DLL)

```bash
# Windows
gcc -shared -o bin/structs.dll build/**/*.o
```

-----

## 🐍 Python Integration Example

```python
import ctypes

lib = ctypes.CDLL("./bin/structs.dll")

# Setup 64-bit pointers
lib.list_create.restype = ctypes.c_void_p
lib.list_is_empty.argtypes = [ctypes.c_void_p]

# Use it!
my_list = lib.list_create()
print(f"Empty: {lib.list_is_empty(my_list)}")
```

-----

## 📂 Project Structure

```text
STRUCTS/
├── bin/          # Compiled .dll / .so binaries
├── build/        # Intermediate object files (.o)
├── docs/         # Full User Manual
├── include/      # Public headers (.h)
├── src/          # Source code (.c)
└── tests/        # Unit tests
```