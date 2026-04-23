@echo off
REM Windows 批处理脚本 - 运行单元测试

setlocal

REM 获取脚本所在目录
set "SCRIPT_DIR=%~dp0"
set "PROJECT_ROOT=%SCRIPT_DIR%.."

REM 默认构建目录
set "BUILD_DIR=%PROJECT_ROOT%\build"

REM 检查是否提供了构建目录参数
if not "%1"=="" set "BUILD_DIR=%1"

echo 项目根目录: %PROJECT_ROOT%
echo 构建目录: %BUILD_DIR%

REM 检查构建目录是否存在
if not exist "%BUILD_DIR%" (
    echo 错误: 构建目录不存在: %BUILD_DIR%
    echo 请先运行: mkdir build ^&^& cd build ^&^& cmake .. -DBUILD_TESTING=ON ^&^& cmake --build .
    exit /b 1
)

REM 进入构建目录
cd /d "%BUILD_DIR%"

REM 检查测试可执行文件是否存在
set "TEST_EXEC=tests\rime_octagram_tests.exe"
if not exist "%TEST_EXEC%" (
    echo 错误: 测试可执行文件不存在: %TEST_EXEC%
    echo 请先构建测试: cmake --build . --target rime_octagram_tests
    exit /b 1
)

REM 运行测试
echo 运行单元测试...
if exist "CTestTestfile.cmake" (
    echo 使用 CTest 运行测试...
    ctest --output-on-failure -R rime_octagram_tests
) else (
    echo 直接运行测试可执行文件...
    "%TEST_EXEC%"
)

echo 测试完成!

endlocal

