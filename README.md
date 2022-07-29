# rastrum

A toy software rasterizer.

## Code

rastrum is written in C++ with the C++20 standard enabled.

Code is linted automatically during builds with `clang-tidy` when installed.

Code is formatted with `clang-format` by running the *clangformat* target
when installed.

## Build

rastrum can be built with cmake:

```
mkdir build
cd build
cmake ..
make
```

## Run

After building the example apps in *apps* can be run:

```
cd build
apps/<app>
```

## Examples

Wireframe rendering of the centurion helmet model:

![Centurion helmet wireframe](./data/centurion_helmet/centurion_wireframe.jpg)

Unshaded rendering of the centurion helmet model:

![Centurion helmet unshaded](./data/centurion_helmet/centurion_poly.jpg)

