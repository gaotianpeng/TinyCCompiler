if [ ! -d "build" ]; then
  mkdir build
fi

cp -r tests/test.cc build/
cd build
cmake ..
make
./tinycompiler_tests
