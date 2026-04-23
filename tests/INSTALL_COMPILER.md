# Windows 上安装 C++ 编译器指南

在 Windows 上编译 C++ 项目，你有几个选择：

## 方法1: 使用 MSYS2 (推荐，适合 MinGW-w64/g++)

MSYS2 提供了完整的 Unix-like 环境和包管理器，非常适合开发。

### 安装步骤：

1. **下载并安装 MSYS2**
   - 访问 https://www.msys2.org/
   - 下载安装程序并运行
   - 安装到默认位置（通常是 `C:\msys64`）

2. **安装编译工具链**
   - 打开 MSYS2 UCRT64 终端（从开始菜单）
   - 运行以下命令：
   ```bash
   pacman -Syu
   pacman -S mingw-w64-ucrt-x86_64-gcc
   pacman -S mingw-w64-ucrt-x86_64-cmake
   pacman -S mingw-w64-ucrt-x86_64-make
   ```

3. **添加到 PATH**
   - 将 `C:\msys64\ucrt64\bin` 添加到系统 PATH
   - 或者在 PowerShell 中临时设置：
   ```powershell
   $env:Path += ";C:\msys64\ucrt64\bin"
   ```

4. **验证安装**
   ```powershell
   g++ --version
   ```

## 方法2: 使用 Visual Studio (推荐，适合 Windows 原生开发)

Visual Studio 提供了 MSVC 编译器，CMake 也支持。

### 安装步骤：

1. **下载 Visual Studio**
   - 访问 https://visualstudio.microsoft.com/
   - 下载 Visual Studio Community（免费）

2. **安装 C++ 工作负载**
   - 运行安装程序
   - 选择 "使用 C++ 的桌面开发" 工作负载
   - 确保包含 "CMake 工具"

3. **使用 Visual Studio 构建**
   ```powershell
   # 在项目根目录
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022" -A x64
   cmake --build . --config Release
   ```

## 方法3: 使用 MinGW-w64 (独立安装)

### 安装步骤：

1. **下载 MinGW-w64**
   - 访问 https://www.mingw-w64.org/downloads/
   - 或使用预编译版本：https://winlibs.com/
   - 下载并解压到某个目录（如 `C:\mingw64`）

2. **添加到 PATH**
   - 将 `C:\mingw64\bin` 添加到系统 PATH
   - 或在 PowerShell 中：
   ```powershell
   $env:Path += ";C:\mingw64\bin"
   ```

3. **验证安装**
   ```powershell
   g++ --version
   ```

## 方法4: 使用 Chocolatey (包管理器)

如果你有 Chocolatey 包管理器：

```powershell
# 以管理员身份运行 PowerShell
choco install mingw
```

## 推荐配置

对于这个项目，我推荐使用 **MSYS2** 或 **Visual Studio**，因为：

- MSYS2: 提供完整的 Unix-like 环境，g++ 支持良好
- Visual Studio: Windows 原生，与 CMake 集成好，调试工具强大

## 验证安装

安装完成后，在 PowerShell 中运行：

```powershell
# 检查 g++ (MinGW)
g++ --version

# 或检查 MSVC (Visual Studio)
cl
```

## 使用 CMake 构建（推荐方式）

无论使用哪种编译器，都建议使用 CMake 而不是直接使用 g++：

```powershell
# 创建构建目录
mkdir build
cd build

# 配置（CMake 会自动检测编译器）
cmake .. -DBUILD_TESTING=ON

# 构建
cmake --build .

# 运行测试
ctest --output-on-failure
```

## 常见问题

### Q: 为什么不能直接编译单个文件？
**A:** 这个项目依赖多个库（rime, darts等），需要链接这些库。使用 CMake 可以自动处理依赖关系。

### Q: 安装后仍然找不到 g++？
**A:** 
1. 确保已添加到 PATH
2. 重启 PowerShell/终端
3. 检查安装路径是否正确

### Q: 应该使用哪个编译器？
**A:** 
- 如果项目主要针对 Windows: 使用 Visual Studio (MSVC)
- 如果需要跨平台兼容: 使用 MinGW-w64 (g++)
- 如果熟悉 Linux 环境: 使用 MSYS2

