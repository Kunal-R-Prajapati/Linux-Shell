# Linux-Shell - A Simple Shell in C

> A fun and educational project to learn C/C++ in depth by building a simple command-line shell for Linux.

![C](https://img.shields.io/badge/Language-C-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Linux-orange.svg)

## 📌 About

This is a mini project inspired by an online tutorial to help understand the fundamentals of systems programming using the C language. A minimalist yet powerful custom Linux shell with built-in commands like `weather`, `radio`, `todo`, and `ai`—designed for both utility and fun. Built with C, it supports external commands, internal tools, and Google Gemini AI integration. It mimics a very simplified version of a Unix shell.

This project is **Linux-only compatible** for now.

## 🛠️ Features

- Executes standard Linux commands like `ls`, `pwd`, `echo`, etc.
- Built-in internal commands:
  - `cd` – Change directory
  - `help` – Show help message
  - `weather` – View real-time weather updates from the terminal
  - `radio` – Listen to Indian radio stations from the terminal
  - `todo` – Add, remove, and list tasks directly in your terminal
  - `ai` – Ask questions and get responses from Google Gemini AI
  - `exit` – Exit the shell
- Forks child processes to run commands
- Uses `execvp()` for external programs
- Tokenizes input with custom delimiters
- Graceful handling of `EOF` and invalid commands
- Simple, modular, and extensible design


## 📂 File Structure
- main.c # Source code for the shell
- README.md # You're reading it!

## 🧰 Dependencies

To run all features properly, make sure you have the following installed:

- `curl` – For API calls (weather, AI)
- `ffplay` or `mpv` – For playing radio streams
- `jq` – For parsing JSON (used in AI response parsing)

Install them on Ubuntu/Debian using:

```bash
sudo apt update
sudo apt install curl ffmpeg jq
```

---

## 🔐 Gemini API Key Setup

1. Create an account at [Google AI Studio](https://aistudio.google.com/)
2. Generate a Gemini API key.
3. Save your key in a file named `gemini_key.h`:

```c
// gemini_key.h
#define GEMINI_API_KEY "your-api-key-here"
```

4. Add `gemini_key.h` to `.gitignore`:

```gitignore
gemini_key.h
```

5. Use the provided `gemini_key_example.h` as a template.

---

## ⚙️ Build & Run

### 🔨 Build the shell

```bash
gcc -o main main.c
```

### ▶️ Run the shell

```bash
./main
```

---


> ⚠️ **Note**  
> This shell is compatible with **Linux only**.  
> Make sure you have GCC installed before compiling.

## 🚀 Usage


### 📻 Radio

```bash
radio
```
Choose from a list of working Indian stations.

---

### ✅ Todo

```bash
todo add "Buy groceries"
todo list
todo remove 1
todo help
```

---

### 🤖 AI

```bash
ai Tell me a fun fact about the moon
ai help
```

---

## 📌 Example

```bash

$ todo add "Fix bug in radio"
Added task: Fix bug in radio

$ ai What is the capital of France?
Paris is the capital city of France.
```

---

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

## 👨‍💻 Author

Made with ❤️ by Kunal Prajapati