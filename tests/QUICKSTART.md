# 单元测试快速开始指南

## 快速开始

### 1. 构建测试

```bash
# 创建构建目录
mkdir build
cd build

# 配置并构建（启用测试）
cmake .. -DBUILD_TESTING=ON
cmake --build .
```

### 2. 运行测试

**方法1: 使用 CTest (推荐)**
```bash
cd build
ctest --output-on-failure
```

**方法2: 直接运行可执行文件**
```bash
cd build
./tests/rime_octagram_tests        # Linux/Mac
tests\rime_octagram_tests.exe     # Windows
```

**方法3: 使用便捷脚本**
```bash
# Linux/Mac
./tests/run_tests.sh

# Windows
tests\run_tests.bat
```

### 3. 运行特定测试

```bash
cd build
./tests/rime_octagram_tests --gtest_filter=GramEncodingTest.*
./tests/rime_octagram_tests --gtest_filter=GramDbTest.Build
```

## 测试输出示例

```
[==========] Running 15 tests from 3 test suites.
[----------] Global test environment set-up.
[----------] 5 tests from GramEncodingTest
[ RUN      ] GramEncodingTest.EncodeASCII
[       OK ] GramEncodingTest.EncodeASCII (0 ms)
[ RUN      ] GramEncodingTest.EncodeCJK
[       OK ] GramEncodingTest.EncodeCJK (0 ms)
...
[==========] 15 tests from 3 test suites ran. (10 ms total)
[  PASSED  ] 15 tests.
```

## 常见问题

### Q: CMake 无法下载 Google Test
**A:** 可以手动安装 Google Test，然后修改 `tests/CMakeLists.txt`:
```cmake
find_package(GTest REQUIRED)
# 移除 FetchContent 相关代码
```

### Q: 编译错误 - 找不到 std::filesystem
**A:** 确保使用支持 C++17 的编译器（GCC 8+, Clang 7+, MSVC 2017+）

### Q: 链接错误
**A:** 检查 `tests/CMakeLists.txt` 中的链接库设置，确保所有依赖都已正确链接

### Q: 测试失败 - 找不到文件
**A:** 检查临时目录权限，确保测试可以创建临时文件

## 下一步

- 查看 [README.md](README.md) 了解详细的测试说明
- 查看测试源代码了解如何编写新测试
- 使用 `--gtest_list_tests` 列出所有可用测试

