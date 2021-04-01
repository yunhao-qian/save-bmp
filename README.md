# save-bmp

A simple C/C++ header-only library for saving RGB images to BMP files.

## Usage

Do this:
```c
#define SAVE_BMP_IMPLEMENT
```
before you include this file in a `.c`/`.cpp` file to create the implementation. Make sure you define the macro in *only one* file which includes this header *only once*. Otherwise, you might get function redefinition errors.

Use `save_bmp()` function to create a BMP file and save the image to it:
```c
enum save_bmp_result save_bmp(const char *filename,
                              uint32_t width, uint32_t height,
                              const uint8_t *image);
```
Currently, the function overwrites the file with the given filename if it has already existed.

The result of the operation is encoded in the returned enum of type `save_bmp_result`. It has the following possible values:
- `SAVE_BMP_SUCCESS = 0`: The operation was successful.
- `SAVE_BMP_SIZE_IS_ZERO = 1`: Either `width` or `height` argument (or both) is zero.
- `SAVE_BMP_SIZE_TOO_BIG = 2`: Due to the header limit of the BMP file format, the size in bytes of a BMP file must be expressible by a 32-bit unsigned integer. This means that the following constraint must be met:
  ```
  54 + (width * 3 + width % 4) * height < 2^32
  ```
  Otherwise, this error code is returned and no disk writing is performed.
- `SAVE_BMP_CANT_OPEN_FILE = 3`: The function failed create the file with the given filename.
- `SAVE_BMP_WRITE_ERROR = 4`: The file was successfully created, but the function was unable to write data to it.

Function `save_bmp_str_result()` is helpful if you want a string that describes the result condition:
```c
const char *save_bmp_str_result(enum save_bmp_result result);
```

The function uses the conventional coordinate system, with the horizontal axis as the x-axis, the vertical axis as the y-axis, and the origin at the upper-left corner. For a pixel at the given `row` and `column`, the R, G, B components should be stored at `image[(row * width + column) * 3]`, `image[(row * width + column) * 3 + 1]`, and `image[(row * width + column) * 3] + 2` respectively.

## Example

The following example is also contained in [example.cpp](example.cpp):
```c++
int main() {
    const uint32_t width{1018}, height{822};
    std::vector<uint8_t> image(width * height * 3);
    auto channel{image.data()};
    for (uint32_t row{}; row < height; ++row)
        for (uint32_t column{}; column < width; ++column) {
            const auto r{static_cast<double>(row) / height};
            const auto g{static_cast<double>(column) / width};
            const auto b{0.5 * (r + g)};
            *channel++ = static_cast<uint8_t>(r * 255);
            *channel++ = static_cast<uint8_t>(g * 255);
            *channel++ = static_cast<uint8_t>(b * 255);
        }
    const auto result{save_bmp("example.bmp", width, height, image.data())};
    puts(save_bmp_str_result(result));
    return 0;
}
```

No dependency is needed, so compile and run the code with:
```
g++ example.cpp && ./a.out
```

Output:

![](example.bmp)

## Acknowledgement

The implementation of this library is greatly inspired by [stb](https://github.com/nothings/stb) and [svpng](https://github.com/miloyip/svpng).
