# Building via Conan | Ubuntu-18.04

The instructions below can be used to build and test EOSIO on Ubuntu-18.04.

**NOTE**: This requires the conan-poc-v2 branch.

## Environment Steps

```
apt-get install -y clang-7 llvm-7-dev python3 python3-dev python3-pip git curl automake jq python2.7 python2.7-dev

pip3 install conan

curl -LO https://github.com/Kitware/CMake/releases/download/v3.15.3/cmake-3.15.3-Linux-x86_64.sh

mkdir -p /usr/local/cmake

chmod +x cmake-3.15.3-Linux-x86_64.sh

./cmake-3.15.3-Linux-x86_64.sh --prefix=/usr/local/cmake --skip-license

export PATH=$PATH:/usr/local/cmake/bin

git clone --single-branch --branch release_80 https://git.llvm.org/git/llvm.git clang8 && cd clang8 && git checkout 18e41dc && cd tools && git clone --single-branch --branch release_80 https://git.llvm.org/git/lld.git && cd lld && git checkout d60a035 && cd ../ && git clone --single-branch --branch release_80 https://git.llvm.org/git/polly.git && cd polly && git checkout 1bc06e5 && cd ../ && git clone --single-branch --branch release_80 https://git.llvm.org/git/clang.git clang && cd clang && git checkout a03da8b && cd tools && mkdir extra && cd extra && git clone --single-branch --branch release_80 https://git.llvm.org/git/clang-tools-extra.git && cd clang-tools-extra && git checkout 6b34834 && cd /clang8/projects && git clone --single-branch --branch release_80 https://git.llvm.org/git/libcxx.git && cd libcxx && git checkout 1853712 && cd ../ && git clone --single-branch --branch release_80 https://git.llvm.org/git/libcxxabi.git && cd libcxxabi && git checkout d7338a4 && cd ../ && git clone --single-branch --branch release_80 https://git.llvm.org/git/libunwind.git && cd libunwind && git checkout 57f6739 && cd ../ && git clone --single-branch --branch release_80 https://git.llvm.org/git/compiler-rt.git && cd compiler-rt && git checkout 5bc7979 && mkdir /clang8/build && cd /clang8/build && cmake -G 'Unix Makefiles' -DCMAKE_INSTALL_PREFIX='/usr/local' -DLLVM_BUILD_EXTERNAL_COMPILER_RT=ON -DLLVM_BUILD_LLVM_DYLIB=ON -DLLVM_ENABLE_LIBCXX=ON -DLLVM_ENABLE_RTTI=ON -DLLVM_INCLUDE_DOCS=OFF -DLLVM_OPTIMIZED_TABLEGEN=ON -DLLVM_TARGETS_TO_BUILD=X86 -DCMAKE_BUILD_TYPE=Release .. && make -j $(nproc) && make install && cd / && rm -rf /clang8

git clone --depth 1 --single-branch --branch release_80 https://github.com/llvm-mirror/llvm.git llvm && cd llvm && mkdir build && cd build && cmake -G 'Unix Makefiles' -DLLVM_TARGETS_TO_BUILD=host -DLLVM_BUILD_TOOLS=false -DLLVM_ENABLE_RTTI=1 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_TOOLCHAIN_FILE='/tmp/pinned_toolchain.cmake' -DCMAKE_EXE_LINKER_FLAGS=-pthread -DCMAKE_SHARED_LINKER_FLAGS=-pthread .. && make -j$(nproc) && make install && cd / && rm -rf /llvm
```

## Build Steps

```
git clone https://github.com/EOSIO/eos.git

cd eos/

git checkout conan-poc-v2

git submodule update --init --recursive

cmake -DCMAKE_BUILD_TYPE='Release' -DCORE_SYMBOL_NAME='SYS' -DPKG_CONFIG_USE_CMAKE_PREFIX_PATH=ON -DCMAKE_TOOLCHAIN_FILE='/workdir/scripts/pinned_toolchain.cmake' -DUSE_CONAN=true -Bbuild

cd build/

make -j$(getconf _NPROCESSORS_ONLN)
```

## Test Steps

```
ctest -j$(getconf _NPROCESSORS_ONLN) -LE _tests --output-on-failure -T Test

ctest -L nonparallelizable_tests --output-on-failure -T Test
```

## License

[MIT](../LICENSE)

## Important

See [LICENSE](../LICENSE) for copyright and license terms.

All repositories and other materials are provided subject to the terms of this [IMPORTANT](../IMPORTANT.md) notice and you must familiarize yourself with its terms.  The notice contains important information, limitations and restrictions relating to our software, publications, trademarks, third-party resources, and forward-looking statements.  By accessing any of our repositories and other materials, you accept and agree to the terms of the notice.
