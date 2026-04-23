#!/bin/bash
# 运行单元测试的便捷脚本

set -e

# 获取脚本所在目录
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# 默认构建目录
BUILD_DIR="${PROJECT_ROOT}/build"

# 检查是否提供了构建目录参数
if [ "$1" != "" ]; then
    BUILD_DIR="$1"
fi

echo "项目根目录: $PROJECT_ROOT"
echo "构建目录: $BUILD_DIR"

# 检查构建目录是否存在
if [ ! -d "$BUILD_DIR" ]; then
    echo "错误: 构建目录不存在: $BUILD_DIR"
    echo "请先运行: mkdir build && cd build && cmake .. -DBUILD_TESTING=ON && cmake --build ."
    exit 1
fi

# 进入构建目录
cd "$BUILD_DIR"

# 检查测试可执行文件是否存在
TEST_EXEC="tests/rime_octagram_tests"
if [ ! -f "$TEST_EXEC" ]; then
    echo "错误: 测试可执行文件不存在: $TEST_EXEC"
    echo "请先构建测试: cmake --build . --target rime_octagram_tests"
    exit 1
fi

# 运行测试
echo "运行单元测试..."
if command -v ctest &> /dev/null; then
    echo "使用 CTest 运行测试..."
    ctest --output-on-failure -R rime_octagram_tests
else
    echo "直接运行测试可执行文件..."
    ./$TEST_EXEC
fi

echo "测试完成!"

