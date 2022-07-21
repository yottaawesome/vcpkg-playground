# Vcpkg Playground

## Introduction

This is a repo housing projects to expriment with vcpkg packages.

## Building

You'll need to setup vcpkg on your machine. Follow the [official instructions here](https://vcpkg.io/en/getting-started.html) (make sure you run `integrate install`). Once you've done this, you'll need to install the relevant packages to build the samples.

* SqlLite3: `.\vcpkg.exe install sqlite3:x64-windows`
* OpenSSL: `.\vcpkg.exe install openssl:x64-windows`
* Boost: `.\vcpkg.exe install boost:x64-windows`
