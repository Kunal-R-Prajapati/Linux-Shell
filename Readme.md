# Linux-Shell - A Simple Shell in C

> A fun and educational project to learn C/C++ in depth by building a simple command-line shell for Linux.

![C](https://img.shields.io/badge/Language-C-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Linux-orange.svg)

## 📌 About

This is a mini project inspired by an online tutorial to help understand the fundamentals of systems programming using the C language. The shell supports basic commands and has a few built-in functions like `cd`, `help`, and `exit`. It mimics a very simplified version of a Unix shell.

This project is **Linux-only compatible** for now.

## 🛠️ Features

- Executes Linux commands like `ls`, `pwd`, `echo`, etc.
- Built-in commands:
  - `cd` – Change directory
  - `help` – Show help message
  - `weather` - Show the weather of city you enter within the terminal
  - `radio` - Play a radio station which you like inside your shell
  - `todo` - A basic todo app right inside your shell as an internal command
  - `exit` – Exit the shell
- Forks a child process to run commands
- Uses `execvp()` to launch programs
- Tokenizes input with custom delimiters
- Graceful handling of EOF and invalid commands

## 📂 File Structure
- main.c # Source code for the shell
- README.md # You're reading it!

## 🚀 How to Run

1. **Clone the Repository**
   ```bash 
   git clone https://github.com/Kunal-R-Prajapati/Linux-Shell.git
   cd lsh-shell```
2. **Compile the Program**
    ```bash
    gcc -o main main.c
3. **Run the Shell**
    ```bash
    ./main

> ⚠️ **Note**  
> This shell is compatible with **Linux only**.  
> Make sure you have GCC installed before compiling.

## ⌨️ Example Usage 
```bash
>>> help
>>> cd /home
>>> ls
>>> echo Hello World!
>>> exit
```

## 🧠 Learning Objectives
- Understand how command-line interfaces work
- Learn about process creation (fork), execution (execvp), and waiting (waitpid)
- Practice dynamic memory allocation and string manipulation
- Grasp how built-in commands and user programs are handled differently

## ❗ Limitations
- No piping (|), redirection (>, <) or background execution (&)
- No command history or advanced prompt


## 🙌 Contribution
You're welcome to contribute! This is a beginner-friendly project. Feel free to:
- Add new built-in commands
- Improve memory management
- Implement features like piping or redirection
- Refactor or document code
## 📃 License
>This project is for educational purposes and is shared freely. No formal license is applied.
