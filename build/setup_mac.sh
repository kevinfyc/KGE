
echo current path: $pwd
echo current file: $0
cmd_path=`dirname $0`
echo cmd path: $cmd_path

cd $cmd_path/../

mkdir build.mac
cd build.mac

cmake -GXcode ../
