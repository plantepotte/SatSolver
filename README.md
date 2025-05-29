# SatSolver
Simple DPLL based SAT solver for fun.

## Install instructions

### Linux:
This requires CMake to be installed.
1. Clone the repo.
2. Create a subfolder in the repo called build.
3. Navigate to the build-subfolder in a terminal.
4. Run the command.
   ```bash
   cmake ..
   ```
5. Compile the library with the following command.
   ```bash
   cmake --build .
   ```
6. Install the library with the following command. You might have to run this as super-user.
   ```bash
   cmake --install .
   ```

### Windows:
 ¯\\\_(ツ)\_/¯

## Usage instructions
Include the library in your CMake project.
```cmake
find_package(SatSolver CONFIG REQUIRED)

target_link_libraries(<Target Name> PRIVATE SatSolver::SatSolver)
```
Then, the solver can be included in your C++ files.
```cpp
#include <DPLLSolver.h>
```

