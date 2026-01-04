# ONC RPC Client–Server System (C/C++)

This repository contains a client–server system implemented in C/C++ and validated on Linux across multiple terminals. The project demonstrates structured client–server communication, shared interface definitions, and correct coordination between independently executing processes.

## 📁 Files
- `client.cxx` – client program
- `server.cxx` – server program
- `local.h` – shared definitions/header

## 🛠️ Build (Linux / WSL)

```bash
g++ -O2 -Wall server.cxx -o server
g++ -O2 -Wall client.cxx -o client
```

## ▶️ Run

Terminal 1:
```bash
./server
```

Terminal 2:
```bash
./client
```
If your client requires a hostname/port, run: ./client localhost (adjust as needed).

## 📜 License

This project is licensed under the MIT License. See the LICENSE file for details.

## 📬 Contact
For questions or feedback, please contact Hannah G. Simon at hgsimon2@gmail.com.
