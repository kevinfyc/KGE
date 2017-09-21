
cmd_path=`dirname $0`
echo cmd path: $cmd_path

pushd $cmd_path/../ #1

cd $cmd_path/../

mkdir build.ios
cd build.ios

echo "---------------------------------"
echo "-------------cmake---------------"
echo "---------------------------------"
cmake -DCMAKE_TOOLCHAIN_FILE=../CMake/Toolchains/iOS.cmake -GXcode ../

popd #1
