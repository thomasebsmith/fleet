# Fleet
###### A simple, functional programming language

Fleet is a simple, functional programming language designed to work for any
task.

This project includes an interpreter written in C++, although the interpreter
is still in progress.

## Building the Project
##### You need:
 * `git`
 * `g++` with C++17 support
 * `make`
 * A Unix-compatible shell

##### Instructions:
 1. Clone the project:
  ```sh
  git clone https://github.com/thomasebsmith/fleet.git
  ```
 2. Navigate to the new folder:
  ```sh
  cd fleet
  ```
 3. Build the appropriate target:
  ```sh
  make tests
  ```
  or simply
  ```sh
  make
  ```
 4. Run the resulting executable:
  ```sh
  ./build/fleet
  ```
  or
  ```sh
  ./build/testFleet
  ```
  depending on what target was built.

For more information about Fleet, visit https://thomasebsmith.github.io/fleet/.
