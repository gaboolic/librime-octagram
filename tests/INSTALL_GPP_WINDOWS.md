# Windows 上安装 g++ 编译器 - 快速指南

## 🚀 方法1: 使用 MSYS2（推荐，最简单）

### 步骤：

1. **下载 MSYS2**
   - 访问：https://www.msys2.org/
   - 下载 `msys2-x86_64-*.exe` 安装程序
   - 运行安装程序，安装到默认位置（`C:\msys64`）

2. **打开 MSYS2 UCRT64 终端**
   - 从开始菜单找到 "MSYS2 UCRT64" 并打开
   - 或者运行：`C:\msys64\ucrt64.exe`

3. **安装 g++ 和工具**
   ```bash
   # 更新包数据库
   pacman -Syu
   
   # 安装 g++ 编译器
   pacman -S mingw-w64-ucrt-x86_64-gcc
   
   # 安装 CMake（如果需要）
   pacman -S mingw-w64-ucrt-x86_64-cmake
   ```

4. **添加到 PATH（在 PowerShell 中）**
   ```powershell
   # 临时添加到当前会话
   $env:Path += ";C:\msys64\ucrt64\bin"
   
   # 永久添加到系统 PATH（需要管理员权限）
   [Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\msys64\ucrt64\bin", "User")
   ```

5. **验证安装**
   ```powershell
   g++ --version
   ```

---

## 🔧 方法2: 使用 WinLibs（预编译的 MinGW-w64）

### 步骤：

1. **下载 WinLibs**
   - 访问：https://winlibs.com/
   - 下载最新版本的 "Win64 - GCC"（选择 UCRT 版本）
   - 解压到某个目录，例如 `C:\mingw64`

2. **添加到 PATH**
   ```powershell
   # 临时添加
   $env:Path += ";C:\mingw64\bin"
   
   # 永久添加（需要管理员权限）
   [Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\mingw64\bin", "User")
   ```

3. **验证安装**
   ```powershell
   g++ --version
   ```

---

## 🎯 方法3: 使用 Chocolatey（如果你有包管理器）

```powershell
# 以管理员身份运行 PowerShell
choco install mingw
```

---

## ⚠️ 重要提示

**不要直接编译单个测试文件！** 这个项目需要 CMake 来管理依赖。

### 正确的使用方式：

```powershell
# 1. 在项目根目录（不是 tests 目录）
cd D:\vscode\rime_projs\librime-octagram

# 2. 创建构建目录
mkdir build
cd build

# 3. 配置项目
cmake .. -DBUILD_TESTING=ON

# 4. 构建
cmake --build .

# 5. 运行测试
ctest --output-on-failure
```

---

## 🔍 验证安装

安装完成后，在 PowerShell 中运行：

```powershell
# 检查 g++
g++ --version

# 应该看到类似输出：
# g++ (MinGW-W64 x86_64-ucrt-...) x.x.x
```

---

## ❓ 常见问题

### Q: 安装后仍然找不到 g++？
**A:** 
1. 确保已添加到 PATH
2. **关闭并重新打开 PowerShell**
3. 检查路径是否正确：`Get-Command g++`

### Q: 应该选择哪个方法？
**A:** 
- **MSYS2**: 最推荐，提供完整开发环境
- **WinLibs**: 快速简单，适合只需要编译器
- **Chocolatey**: 如果你已经使用包管理器

### Q: 为什么不能直接 `g++ test_gram_db.cc`？
**A:** 测试文件依赖多个库（gtest, rime, darts等），需要 CMake 来管理这些依赖和链接。

---

## 📝 快速检查清单

- [ ] 已安装 g++ 编译器
- [ ] 已添加到 PATH 环境变量
- [ ] 已重启 PowerShell
- [ ] `g++ --version` 可以运行
- [ ] 已安装 CMake（从 https://cmake.org/download/）
- [ ] 使用 CMake 构建项目（不是直接 g++）

