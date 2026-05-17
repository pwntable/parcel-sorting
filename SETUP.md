# 🚀 Parcel Sorting System - Quick Start Guide

A quick, step-by-step guide to compile and run the **Parcel Sorting and Delivery Management System**.

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

## 🔑 Step 4: Login Credentials

The system starts with a built-in mock database. Use these pre-configured accounts to log in:

| Role | Username | Password | What it does |
| :--- | :--- | :--- | :--- |
| **Admin** | `admin` | `admin123` | Manage users, parcels, and view all reports. |
| **Rider** | `rider1` | `rider123` | View and update parcels for your assigned route. |
