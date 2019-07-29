set -x
g++ -g -O2 $CFLAGS -isystem googletest/include -Igoogletest \
    -isystem googlemock/include -Igooglemock \
    -pthread -c googletest/src/gtest-all.cc
g++ -g -O2 $CFLAGS -isystem googletest/include -Igoogletest \
    -isystem googlemock/include -Igooglemock \
    -pthread -c googlemock/src/gmock-all.cc
ar -rv libgoogletest.a gtest-all.o gmock-all.o
sudo cp -r googletest/include/* /usr/local/include
sudo cp -r googlemock/include/* /usr/local/include
sudo cp libgoogletest.a /usr/local/lib
set +x
