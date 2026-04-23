# 单元测试说明

本项目使用 Google Test (gtest) 作为单元测试框架。

## 构建测试

### 方法1: 使用 CMake 选项

```bash
mkdir build
cd build
cmake .. -DBUILD_TESTING=ON
cmake --build .
```

### 方法2: 直接构建测试

```bash
mkdir build
cd build
cmake ..
cmake --build . --target rime_octagram_tests
```

## 运行测试

### 使用 CTest (推荐)

```bash
cd build
ctest --output-on-failure
```

### 直接运行测试可执行文件

```bash
cd build
./tests/rime_octagram_tests
```

### 运行特定测试

```bash
cd build
./tests/rime_octagram_tests --gtest_filter=GramEncodingTest.*
```

## 测试覆盖范围

### gram_encoding 模块测试 (`test_gram_encoding.cc`)
- ASCII字符编码
- 中文字符编码
- 空字符串处理
- Unicode长度计算
- 各种Unicode范围的编码

### GramDb 模块测试 (`test_gram_db.cc`)
- 数据库构建
- 数据库加载和保存
- 数据查找功能
- 空数据处理
- 常量验证

### Octagram 模块测试 (`test_octagram.cc`)
- Octagram对象构造
- Query方法测试
- 配置参数测试
- OctagramComponent测试

## 添加新测试

1. 在 `tests/` 目录下创建新的测试文件，例如 `test_new_feature.cc`
2. 在 `tests/CMakeLists.txt` 的 `TEST_SOURCES` 中添加新文件
3. 使用 Google Test 宏编写测试：
   - `TEST(TestSuiteName, TestName)` - 普通测试
   - `TEST_F(TestFixture, TestName)` - 使用测试夹具

## 测试最佳实践

1. **独立性**: 每个测试应该独立运行，不依赖其他测试的状态
2. **可重复性**: 测试结果应该一致，不依赖外部环境
3. **快速执行**: 单元测试应该快速完成
4. **清晰命名**: 测试名称应该清楚描述测试的内容
5. **断言明确**: 使用适当的断言宏（EXPECT_*, ASSERT_*）

## 示例测试结构

```cpp
#include "your_header.h"
#include <gtest/gtest.h>

class YourTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // 测试前的初始化
  }

  void TearDown() override {
    // 测试后的清理
  }
};

TEST_F(YourTest, TestCaseName) {
  // 测试代码
  EXPECT_EQ(expected, actual);
}
```

## 故障排除

### 找不到 Google Test

如果 CMake 无法下载 Google Test，可以手动安装：

```bash
# Ubuntu/Debian
sudo apt-get install libgtest-dev

# 然后修改 tests/CMakeLists.txt 使用 find_package(GTest)
```

### 链接错误

确保所有依赖库都已正确链接，检查 `tests/CMakeLists.txt` 中的 `target_link_libraries`。

### 运行时错误

- 检查测试数据文件路径
- 确保临时目录有写权限
- 检查依赖的服务是否已初始化

