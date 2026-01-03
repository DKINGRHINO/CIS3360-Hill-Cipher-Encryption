```markdown
# CIS3360 Hill Cipher Encryption

A C++ program to encrypt plaintext files using the Hill Cipher (2x2 to 9x9 matrix), as assigned in **CIS3360: Security in Computing** at the University of Central Florida (Summer 2025).

This project demonstrates matrix mathematics, modular arithmetic, file I/O, and classic encryption techniques.

## Project Overview

- Takes a key file (containing the matrix) and a plaintext file as command-line arguments.
- Prints the key matrix.
- Processes plaintext: converts to lowercase, removes non-alphabetic characters, pads with 'x' if needed for block alignment.
- Encrypts using matrix multiplication modulo 26.
- Outputs ciphertext in lines of 80 characters.

## My Implementation

Implemented solo:
- Validated command-line arguments and file access.
- Parsed and printed the key matrix (dynamic size 2–9).
- Cleaned and padded plaintext appropriately.
- Performed block-by-block matrix multiplication (mod 26) for encryption.
- Handled formatting and edge cases (e.g., non-square input, padding).

## How to Build and Run

### Requirements
- G++ compiler

### Compilation
```bash
g++ -o pa02 pa02.cpp
```

### Execution
```bash
./pa02 key.txt plaintext.txt
```
- Create your own simple test key and plaintext files to run.

## Credits & Attribution

- Assignment guidelines by Michael McAlpin, UCF CIS3360.
- Implemented as a solo academic project – shared for portfolio purposes only.

## License / Usage Note

Developed for UCF coursework. Feel free to view and learn from it, but do not use it to complete active assignments in CIS3360 or similar courses.

---

**Christopher Otto**  
Computer Science B.S. Candidate, Class of 2027  
University of Central Florida
