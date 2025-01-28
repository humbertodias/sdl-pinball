### Build and Run

```shell
BUILD_TYPE=Debug
#BUILD_TYPE=Release
rm -rf build
cmake -Bbuild -DCMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build build --config $BUILD_TYPE 
(cd Game && ../build/Pinball)
```


### Play

* Arrow Down: Throw the ball
* Left/Right Arrows: Move the flaps
* Space: Restart the game