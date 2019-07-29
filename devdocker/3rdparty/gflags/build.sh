mkdir -p build-out
cd build-out
MYCFLAGS="-g -O3 $CFLAGS"
cmake .. -DBUILD_SHARED_LIBS=ON \
         -DBUILD_STATIC_LIBS=ON \
         -DCMAKE_BUILD_TYPE=RelWithDebInfo \
         -DCMAKE_CXX_FLAGS_RELWITHDEBINFO="$MYCFLAGS"
make -j $PARALLELISM VERBOSE=1
sudo make install
