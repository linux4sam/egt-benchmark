![Ensemble Graphics Toolkit](docs/logo.png)

# EGT Benchmark Application

This set of applications are used to run various benchmarks of EGT.  This is
useful for testing different hardware combinations and configurations of EGT
itself.  The EGT developers use this to find both optimizations and regressions
when it comes to performance.

# Compiling

First compile and install libegt to a directory.

```sh
git clone --recursive https://github.com/linux4sam/egt.git
cd egt
./autogen.sh
./configure --prefix=/opt/egt
make
make install
```

Then, configure and build this samples project.

```sh
git clone --recursive https://github.com/linux4sam/egt-benchmark.git
./autogen.sh
CXXFLAGS="-I/opt/egt/include/" \
LDFLAGS="-L/opt/egt/lib/" \
PKG_CONFIG_PATH=/opt/egt/lib/pkgconfig \
./configure
make
```

To run an example, set the LD_LIBRARY_PATH environment variable to the library.

```sh
LD_LIBRARY_PATH=/opt/egt/lib ./benchmark
```

## License

Released under the terms of the `Apache 2` license. See the [COPYING](COPYING)
file for more information.
