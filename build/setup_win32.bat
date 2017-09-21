echo off

set cmd_path=%~dp0
set root_path=%cmd_path%..\

echo "root_path: %root_path%"

pushd %root_path%

cd /d %root_path%

if not exist "build.win32" mkdir "build.win32"

cd build.win32

echo "---------------------------------"
echo "----------cmake------------------"
echo "---------------------------------"
cmake -G"Visual Studio 14" ../

popd
pause
