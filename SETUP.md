# 🚀 Parcel Sorting System - Quick Start Guide

A quick, step-by-step guide to compile, run, and test the **Parcel Sorting and Delivery Management System**.

---

## 🛠️ Step 1: Install GCC Compiler (If you don't have it)

Make sure you have GCC installed. Open your terminal/PowerShell and run:
```bash
gcc --version
```
* **Windows**: If not installed, open PowerShell and run:
  ```powershell
  winget install MSYS2.MSYS2
  ```
* **macOS**: If not installed, open Terminal and run:
  ```bash
  xcode-select --install
  ```
* **Linux**: Open Terminal and run:
  ```bash
  sudo apt update && sudo apt install build-essential
  ```

---

## 🔨 Step 2: Compile the Program

Open your terminal in the project's root folder (`parcel-sorting`) and run the command for your OS:

### **Windows**
```powershell
gcc -Wall -Wextra -g -I./include src/*.c -o parcel_system.exe
```

### **macOS / Linux**
```bash
make
# OR (if make is not installed):
gcc -Wall -Wextra -g -I./include src/*.c -o parcel_system
```

---

## 🚀 Step 3: Run the Program

### **Windows**
```powershell
.\parcel_system.exe
```

### **macOS / Linux**
```bash
./parcel_system
```

---

## 🔄 Step 4: Updating & Recompiling (After pulling updates)

If you pull new updates from git, the compiled executable file (`parcel_system` or `parcel_system.exe`) does **not** update automatically. You must re-compile the program to see your changes:

### **Windows**
1. Open PowerShell or Command Prompt in the project's root folder (`parcel-sorting`).
2. Run the compilation command to overwrite the old `.exe`:
   ```powershell
   gcc -Wall -Wextra -g -I./include src/*.c -o parcel_system.exe
   ```
3. Run the newly compiled program:
   ```powershell
   .\parcel_system.exe
   ```

### **macOS / Linux**
1. Open Terminal in the project's root folder (`parcel-sorting`).
2. Run the compilation command:
   ```bash
   make
   ```
3. Run the newly compiled program:
   ```bash
   ./parcel_system
   ```

---

## 🧪 Step 5: Run Unit Tests

The codebase includes a suite of unit tests to verify the core parcel sorting logic.

### **macOS / Linux**
```bash
make test
# OR (if make is not installed):
gcc -Wall -Wextra -g -I./include tests/test_suite.c src/address.c src/database.c src/login.c src/output.c src/parcel_list.c src/search.c src/sorting.c src/status.c src/validation.c -o test_suite
./test_suite
```

### **Windows**
```powershell
gcc -Wall -Wextra -g -I./include tests/test_suite.c src/address.c src/database.c src/login.c src/output.c src/parcel_list.c src/search.c src/sorting.c src/status.c src/validation.c -o test_suite.exe
.\test_suite.exe
```

---

## 🧹 Step 6: Clean Build Files (macOS / Linux)

To delete compiled object files and binaries to clean up your workspace:
```bash
make clean
```

---

## 🔑 Step 7: Login Credentials

The system loads pre-configured accounts from `dataset/users.csv`. Use these credentials to log in:

| Role | Username | Password | Assigned Road / Notes |
| :--- | :--- | :--- | :--- |
| **Admin** | `admin01` | `admin123` | Full access. Manage users, parcels, view reports. |
| **Admin** | `superadmin` | `admin456` | Full access. Manage users, parcels, view reports. |
| **Rider** | `rider01` | `rider123` | Assigned to: `Jalan Mawar` (Taman Batu Pahat, Johor) |
| **Rider** | `rider02` | `rider456` | Assigned to: `Jalan Mawar 2` (Taman Batu Pahat, Johor) |
| **Rider** | `rider04` | `rider789` | Assigned to: `Jalan Kenanga` (Taman Batu Pahat, Johor) |

*(Note: If the CSV files are deleted, the system will initialize a new mock database with fallback credentials `admin`/`admin123` and `rider1`/`rider123`)*
