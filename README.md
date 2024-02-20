# CCMS

Collection of C Memory Structures, a lightweight header-only C library.

## Features

- **Cross-platform support:** Designed to work seamlessly on Windows, Linux, and macOS.
- **Header-only implementation:** No compilation or linking required; just include the necessary headers in your project.

## Building

### Dependencies

### Using Xmake (PPA)

Add the following lines to your `xmake.lua`

```lua
add_repositories("hendrikboeck-ppa https://github.com/hendrikboeck/xmake-ppa.git main")
add_requires("ccms")

-- ...

target("<your-target>")
  -- ...
  add_packages("ccms")
```

### Using Xmake (local repository)

```sh
git clone https://github.com/hendrikboeck/ccms.git
xmake package -P ccms -D -o ./build
rm -rf ccms
```

### Barebones

```sh
git clone https://github.com/hendrikboeck/ccms.git
cp -r ccms/include/* <your-include-folder>/
rm -rf ccms
```

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests.

## License

`ccms` is licensed under the GNU General Public License v3.0 or later (GPLv3+). A copy of the license can be found here: https://www.gnu.org/licenses/gpl-3.0.en.html
