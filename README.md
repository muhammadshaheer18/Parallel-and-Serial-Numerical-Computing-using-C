# ⚙️ Parallel-and-Serial-Numerical-Computing-using-C

This project demonstrates **parallel and serial implementations** of several numerical computing methods using the **C programming language**. It covers core numerical techniques such as **Numerical Differentiation**, **Numerical Integration**, and **Matrix Operations**, with performance comparisons using **OpenMP** for parallel execution.

---

## 📌 Features

- 🧮 **Numerical Differentiation**
  - Forward, backward, and central difference methods
  - Uses CSV datasets for testing
- ∫ **Numerical Integration**
  - Trapezoidal and Simpson’s Rule implementations
- 🧾 **Matrix Operations**
  - Gaussian elimination, LU decomposition, etc.
  - Read matrix data from `matrix.txt`
- ⏱️ **Performance Benchmarking**
  - Compare execution time of serial vs. parallel code
- ⚙️ **Parallelization**
  - Implemented using [OpenMP](https://www.openmp.org/) for multi-threaded execution

---

## 📂 File Structure

```

.
├── Linear\_Matrix\_Systems.h          # Matrix-related methods
├── Numerical\_Differentiation.h      # Headers for numerical differentiation
├── Numerical\_Integration.h          # Headers for numerical integration
├── main.c                           # Main source file for running the program
├── main\_backup.c                    # Backup of main file
├── random\_num\_generator.c           # Utility to generate random test datasets
├── matrix.txt                       # Sample input matrix
├── forward\_output.csv               # Output file for forward difference results
├── backward\_output.csv              # Output file for backward difference results
├── num\_diff\_dataset.csv             # Dataset used for differentiation methods
├── README.md                        # Project documentation

````

---

## 🔧 Requirements

- GCC Compiler
- [OpenMP](https://www.openmp.org/) Support
- CSV-capable file reader (for input/output verification)

Install `gcc` and ensure OpenMP is supported:

```bash
sudo apt-get install build-essential
gcc --version  # Should be GCC 4.2+ with OpenMP support
````

---

## 🚀 Usage

### 🔹 Compile and Run Serial Version:

```bash
gcc main.c -o serial_exec -lm
./serial_exec
```

### 🔹 Compile and Run Parallel Version:

```bash
gcc main.c -o parallel_exec -fopenmp -lm
./parallel_exec
```

Make sure to modify the source files as needed to switch between serial and parallel execution regions.

---

## 📊 Output

* CSV files such as `forward_output.csv` and `backward_output.csv` store calculated results.
* Execution times for serial and parallel methods are printed to the terminal.
* Useful for performance analysis and plotting graphs.

---

## 🧠 Concepts Used

* Numerical Analysis
* Finite Difference Methods
* Matrix Solvers (LU, Gaussian)
* Parallel Computing with OpenMP
* File I/O in C (CSV/text parsing)

---

## 📈 Sample Visualization (Optional)

You can use Python, Excel, or MATLAB to visualize CSV results:

```python
import pandas as pd
df = pd.read_csv("forward_output.csv")
df.plot(x="X", y="f'(X)", title="Forward Difference")
```

---

## 🧑‍💻 Author

**Muhammad Shaheer**

---

## 📝 License

This project is licensed.

---

## 🤝 Contributing

Feel free to fork and improve this code. Pull requests are welcome for optimizations, new numerical methods, or enhanced OpenMP integration.

```
