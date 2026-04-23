# GitHub Actions 工作流解析

## 工作流结构概览

这个工作流包含**两个独立的构建任务**，用于在不同的 Windows 环境下编译项目：

1. **`build`** - 使用 Windows 原生编译器（MSVC 和 Clang）
2. **`build-mingw`** - 使用 MSYS2 + MinGW-w64（GCC）

---

## Job 1: `build` (Windows 原生构建)

### 使用的编译器：
- **MSVC** (`cl`) - Microsoft Visual C++ 编译器
- **Clang** (`clang++`) - LLVM Clang 编译器

### 安装的依赖：

```yaml
boost_version: 1.89.0
BOOST_ROOT: ${{ github.workspace }}\deps\boost-1.89.0
```

**主要依赖项**（通过 `build.bat deps` 安装）：
- Boost C++ 库
- 其他 Rime 项目所需的依赖（从 `build.bat` 脚本安装）

### 构建工具：
- **Ninja** - 快速构建系统
- **CMake** - 构建配置工具

### 构建步骤：
1. 配置 MSVC 或 Clang 环境
2. 安装 Boost 库
3. 构建依赖项
4. 安装 Rime 插件
5. 构建和测试项目
6. 创建分发包

---

## Job 2: `build-mingw` (MSYS2 + MinGW-w64 构建)

### 使用的编译器：
- **MinGW-w64 GCC** (`g++`) - 通过 MSYS2 安装

### 安装的依赖（通过 MSYS2 包管理器）：

```yaml
install: >-
  git
  base-devel
  mingw-w64-ucrt-x86_64-boost      # Boost 库
  mingw-w64-ucrt-x86_64-glog        # Google Logging 库
  mingw-w64-ucrt-x86_64-gtest       # Google Test 框架
  mingw-w64-ucrt-x86_64-yaml-cpp    # YAML 解析库
  mingw-w64-ucrt-x86_64-leveldb      # LevelDB 数据库
  mingw-w64-ucrt-x86_64-marisa      # MARISA trie 库
  mingw-w64-ucrt-x86_64-opencc       # OpenCC 中文转换库
pacboy: >-
  toolchain:p                        # GCC 工具链
  cmake:p                            # CMake
  ninja:p                            # Ninja
```

### 构建环境：
- **MSYS2** - Unix-like 环境（提供包管理器 `pacman`）
- **UCRT64** - MSYS2 环境类型（使用 UCRT 运行时）

---

## 为什么既有 MSYS2 也有 MinGW？

### 关键概念：

1. **MSYS2** ≠ **MinGW**
   - **MSYS2** 是一个**环境/包管理器**，提供：
     - Unix-like 命令行环境
     - `pacman` 包管理器（类似 Arch Linux）
     - 开发工具和库的预编译包
   
   - **MinGW-w64** 是一个**编译器工具链**，提供：
     - GCC 编译器（`g++`）
     - Windows 上的 GNU 工具链
     - 链接器和运行时库

2. **它们的关系**：
   ```
   MSYS2 (环境)
   └── 提供包管理器 (pacman)
       └── 可以安装 MinGW-w64 工具链
           └── 包含 g++ 编译器
   ```

3. **类比理解**：
   - **MSYS2** = 像 Linux 发行版（如 Ubuntu）
   - **MinGW-w64** = 像编译器（如 GCC）
   - MSYS2 可以安装和管理 MinGW-w64

---

## 为什么需要两个构建任务？

### 原因 1: 编译器兼容性
- **MSVC/Clang**: Windows 原生，与 Windows API 集成好
- **MinGW-w64**: 跨平台兼容，使用 GCC，适合 Unix/Linux 开发者

### 原因 2: 依赖管理方式不同
- **`build` job**: 手动构建依赖（通过 `build.bat`）
- **`build-mingw` job**: 使用 MSYS2 包管理器自动安装预编译包

### 原因 3: 目标用户不同
- **MSVC/Clang 构建**: 面向 Windows 原生开发者
- **MinGW 构建**: 面向需要跨平台兼容性的开发者

### 原因 4: 测试覆盖
- 确保代码在不同编译器下都能正常工作
- 发现编译器特定的 bug

---

## 依赖项详细说明

### 在 `build-mingw` 中安装的库：

| 库名 | 用途 |
|------|------|
| `boost` | C++ 扩展库，提供各种工具和数据结构 |
| `glog` | Google 日志库，用于日志记录 |
| `gtest` | Google Test 框架，用于单元测试 |
| `yaml-cpp` | YAML 配置文件解析库 |
| `leveldb` | 键值存储数据库 |
| `marisa` | 静态 trie 数据结构库 |
| `opencc` | 中文简繁体转换库 |

### 工具：
- `toolchain` - GCC 编译器工具链（g++, gcc, ld 等）
- `cmake` - 构建系统生成器
- `ninja` - 快速构建工具

---

## 构建流程对比

### `build` (MSVC/Clang):
```
1. 配置编译器环境
2. 安装 Ninja
3. 手动安装 Boost
4. 运行 build.bat 构建依赖
5. 构建和测试项目
```

### `build-mingw` (MSYS2):
```
1. 使用 MSYS2 安装所有依赖（一键安装）
2. 配置 Git
3. 使用 CMake + Ninja 构建
4. 安装和测试
```

---

## 总结

- **MSYS2** 是环境/包管理器，**MinGW-w64** 是编译器工具链
- MSYS2 可以安装和管理 MinGW-w64
- 两个构建任务确保项目在不同编译器下都能正常工作
- `build-mingw` 使用 MSYS2 的包管理器，依赖安装更简单
- `build` 使用 Windows 原生编译器，需要手动管理依赖

