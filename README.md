# File-encryptor
This is a C++ project for encrypting and decrypting files using the **Crypto++** library with the AES algorithm.

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


## 🔄 Customization

### **Change the Ciphering Directory**
By default, the encryption directory is set to:
```cpp
return std::string(userProfile) + "\\dir";
```
This means files will be stored in:
```
C:\Users\User\dir
```
You can modify this path to store encrypted files elsewhere by changing the `"dir"` string in the source code.

### **Change the Encrypted File Extension**
By default, encrypted files will have the extension:
```cpp
const std::vector<std::string> extension = { ".cipher81" };
```
If you want to change the extension, modify `".cipher81"` to your preferred file extension, e.g., `.enc`.

### **Change the Target File Extensions**
The program looks for files with specific extensions to encrypt. By default, it searches for:
```cpp
const std::vector<std::string> exampleExtensions = { ".txt" };
```
You can add more file types or change them based on your needs, for example:
```cpp
const std::vector<std::string> exampleExtensions = { ".txt", ".docx", ".pdf" };
```
This will allow the program to find and encrypt `.txt`, `.docx`, and `.pdf` files.

---

## 📜 License
This project is licensed under the MIT License.

