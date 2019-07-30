mkdir -p build
cd build
MYCFLAGS="-O2 -g $CFLAGS"
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_CXX_FLAGS_RELWITHDEBINFO="$MYCFLAGS" \
    -DCMAKE_C_FLAGS_RELWITHDEBINFO="$MYCFLAGS" \
    ../
VERBOSE=1 make -j $PARALLELISM
sudo make install
