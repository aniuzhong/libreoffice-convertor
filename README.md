# LibreOffice Convertor

Document convertor based on LibreOffice.

## Overview

**autogen.input**: LibreOffice configurations

**findsofficepath.c.diff**: Patch for LibreOffice UNO API (modify the way of finding soffice.bin executable)

**.gitignore**: No more tracking `instdir` and `include` (UNO API headers)

**cmdline**: Third party command line parser libraries

## Build

```CMake
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

## Usage

```
LOConvertor.exe --infile=string --outfile=string [options] ...
```

## How to Release

Copy `LOConvertor.exe` into directory `instdir\program`. The release directory will be

```
instdir/
|-- program/
|    |-- LOConvertor.exe
│    |-- soffice.bin
│    \-- cppuhelper3MSC.dll
|-- sdk/
|-- share/
\-- presets/
```