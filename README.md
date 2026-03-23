# ZumoHALRP2040 <!-- omit in toc -->

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](http://choosealicense.com/licenses/mit/)
[![Repo Status](https://www.repostatus.org/badges/latest/wip.svg)](https://www.repostatus.org/#wip)

Hardware abstraction layer for the Pololu Zumo2040 robot (see <https://www.pololu.com/category/129/zumo-robots-and-accessories>).

## Table of content <!-- omit in toc -->

- [Architecture](#architecture)
  - [The Principle](#the-principle)
  - [Detail](#detail)
- [How to integrate the library?](#how-to-integrate-the-library)
  - [Example](#example)
- [Interface Description](#interface-description)
- [Requirements to your application](#requirements-to-your-application)
- [Used Libraries](#used-libraries)
- [Issues, Ideas And Bugs](#issues-ideas-and-bugs)
- [License](#license)
- [Contribution](#contribution)

## Architecture

### The Principle

![Principle](https://raw.githubusercontent.com/BlueAndi/ZumoHALRP2040/refs/heads/develop/doc/uml/Principle.plantuml)

### Detail

## How to integrate the library?
```
lib_deps =
    https://github.com/BlueAndi/ZumoHALRP2040.git
```

### Example

See [example](/examples/example/) for more detail.

## Interface Description


## Requirements to your application

- **REQ-1** The application shall use the Arduino framework.
```
framework = arduino
```
- **REQ-2** The application shall use the Raspberry Pi Platform by maxgerhardt.
```
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
```
- **REQ-3** The application shall use the Earle Philhower Arduino Core.
```
board_build.core = earlephilhower
```


## Used Libraries

| Library                                                                 | Description                               | License |
| ----------------------------------------------------------------------- | ----------------------------------------- | ------- |
| [ZumoHALInterfaces](https://github.com/BlueAndi/ZumoHALInterfaces)      | The Zumo C++ HAL interfaces.              | MIT     |

## Issues, Ideas And Bugs

If you have further ideas or you found some bugs, great! Create a [issue](https://github.com/BlueAndi/ZumoHALATmega32u4/issues) or if you are able and willing to fix it by yourself, clone the repository and create a pull request.

## License

The whole source code is published under the [MIT license](http://choosealicense.com/licenses/mit/).
Consider the different licenses of the used third party libraries too!

## Contribution

Unless you explicitly state otherwise, any contribution intentionally submitted for inclusion in the work by you, shall be licensed as above, without any
additional terms or conditions.