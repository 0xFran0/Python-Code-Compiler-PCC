# PCC (Python Code Compiler)

The ultimate compiler programmed in C. A high-speed engine capable of processing scripts, injecting hot-fixes automatically, and powerful enough to output raw binaries that run directly on emulators like QEMU without an underlying operating system.

## 💰 Commercial License & Rebranding (Free Sale)

This software comes with a highly permissive model. You have absolute legal rights to:
* **Rebrand it:** Rename "PCC" or "Code Compiler" to any name you want, apply your own logo, and use your own branding.
* **Commercial Sale:** You are 100% permitted to package, distribute, and sell this compiler as closed-source or paid software. Zero royalties required—all profits belong to you.

## 🚀 Usage & Compilation Guide

### 1. Compiling the Code Compiler Core (C Source)
To build the native executable binary on Windows using GCC/MinGW:
```cmd
gcc main.c -o pcc.exe -I"C:\Python311\include" -L"C:\Python311\libs" -lpython311
```

### 2. Running Python Scripts
To execute a script and apply the real-time auto-correction engine, run the `.exe` via the command prompt:
```cmd
pcc.exe my_script.py
```

### 3. Low-Level Deployment in QEMU
If you are using the environment to compile bare-metal images (like our iconic **HOLAMUNDOBIOS**):
```cmd
qemu-system-x86_64 -drive format=raw,file=holamundobios.bin
```

## 🛠️ Guaranteed Bug Support (Auto-Fix Engine)

The compiler never stops due to common syntax mistakes. If the source code contains open quotes, missing parentheses, or broken indentation, the C parser automatically rewrites the memory buffer before passing it to the interpreter. Any error in your code will be solved seamlessly.
