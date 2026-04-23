# 构建说明

## ⚠️ 重要提示

**不能直接使用 `g++ test_gram_db.cc` 编译单个测试文件**，因为：

1. 测试文件依赖多个库：
   - Google Test (gtest)
   - librime 库
   - darts 库
   - 项目中的其他源文件

2. 需要正确的包含路径和链接库

3. 需要 CMake 来管理这些依赖

## ✅ 正确的构建方法

### 方法1: 使用 CMake（推荐）

```powershell
# 在项目根目录（不是 tests 目录）
cd D:\vscode\rime_projs\librime-octagram

# 创建构建目录
mkdir build
cd build

# 配置项目（CMake 会自动检测编译器）
cmake .. -DBUILD_TESTING=ON

# 构建测试
cmake --build . --target rime_octagram_tests

# 运行测试
ctest --output-on-failure
# 或直接运行
.\tests\rime_octagram_tests.exe
```

### 方法2: 如果必须手动编译（不推荐）

如果你真的需要手动编译，需要：

```powershell
# 这只是一个示例，实际路径需要根据你的安装位置调整
g++ test_gram_db.cc ^
    ..\src\gram_db.cc ^
    -I..\src ^
    -I<rime_include_path> ^
    -I<gtest_include_path> ^
    -L<rime_lib_path> ^
    -L<gtest_lib_path> ^
    -lrime -lgtest -lgtest_main ^
    -std=c++17 ^
    -o test_gram_db.exe
```

**但这种方法非常复杂且容易出错，强烈建议使用 CMake！**

## 安装编译器

如果你还没有安装编译器，请参考 [INSTALL_COMPILER.md](INSTALL_COMPILER.md)

## 快速开始（完整流程）

```powershell
# 1. 确保已安装编译器（g++ 或 Visual Studio）
# 2. 确保已安装 CMake（https://cmake.org/download/）

# 3. 在项目根目录
cd D:\vscode\rime_projs\librime-octagram

# 4. 配置和构建
mkdir build
cd build
cmake .. -DBUILD_TESTING=ON
cmake --build .

# 5. 运行测试
ctest --output-on-failure
```

## 检查环境

运行以下命令检查你的环境：

```powershell
# 检查 CMake
cmake --version

# 检查编译器（如果安装了 g++）
g++ --version

# 检查编译器（如果安装了 Visual Studio）
where cl
```

## 如果遇到问题

1. **找不到 CMake**: 从 https://cmake.org/download/ 下载安装
2. **找不到编译器**: 参考 INSTALL_COMPILER.md
3. **链接错误**: 确保所有依赖库都已正确安装和配置
4. **CMake 找不到依赖**: 可能需要设置环境变量或修改 CMakeLists.txt

