### Build and Run

```shell
BUILD_TYPE=Debug
BUILD_TYPE=Release
rm -rf build
cmake -Bbuild -DCMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build build --config $BUILD_TYPE 
(cd Game && ../build/Pinball)
```
