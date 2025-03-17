# FilesEncryptorDecryptorCpp

This is a C++ project for encrypting and decrypting files using the **Crypto++** library.

## 🔧 Installation

### **1. Install Crypto++**

#### **Windows (Visual Studio)**
1. **Download Crypto++** from the official repository: [Crypto++ GitHub](https://github.com/weidai11/cryptopp).
2. Extract the archive to a folder (e.g., `C:\cryptopp`).
3. Open **Visual Studio**.
4. Navigate to the extracted folder and open `cryptest.sln`.
5. Select **Release** mode.
6. Build the solution (`Ctrl+Shift+B`).
7. This will generate the `cryptlib.lib` and `cryptlib.dll` files in `C:\cryptopp\x64\Release` (or `Debug` if compiled in Debug mode).

#### **Linux (Make)**
```sh
# Clone the Crypto++ repository
git clone https://github.com/weidai11/cryptopp.git
cd cryptopp

# Build the Crypto++ library
make

# (Optional) Run tests to verify the build
make test

# Install Crypto++ system-wide
sudo make install
```

#### **Alternative: Install via vcpkg (Cross-Platform)**
```sh
# Install vcpkg if not installed
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat # Windows
./bootstrap-vcpkg.sh  # Linux/macOS

# Install Crypto++
vcpkg install cryptopp
```

---

## **2. Integrate Crypto++ into Your Project**

### **Windows (Visual Studio Setup)**
1. **Include Crypto++ Headers:**
   - Open **Project Properties** (`Alt+Enter` on your project in Solution Explorer).
   - Navigate to **C/C++ → General**.
   - Under **Additional Include Directories**, add `C:\cryptopp\`.

2. **Link Crypto++ Library:**
   - Go to **Project Properties → Linker → General**.
   - Under **Additional Library Directories**, add `C:\cryptopp\x64\Release\`.
   - In **Linker → Input → Additional Dependencies**, add:
     ```
     cryptlib.lib
     ```

### **Linux (g++ Setup)**
1. **Include Crypto++ headers** when compiling:
   ```sh
   g++ -I/usr/local/include -L/usr/local/lib -lcryptopp my_program.cpp -o my_program
   ```


## 📜 License
This project is licensed under the MIT License.

