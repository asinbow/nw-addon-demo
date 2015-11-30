nw-addon-demo
---
support rename PE import library name from nw.exe to *.exe

### install dependencies
```
npm install
npm install -g nw-gyp
npm install -g nwjs
```

### build

nw-gyp configure --target=0.12.3 [--arch=ia32|amd64]
nw-gyp build
node build.js 0.12.3

### rename_import_lib

* use [libpebliss](https://github.com/asinbow/libpebliss)
* source code [rename_import_lib/rename_import_lib.cpp](rename_import_lib/rename_import_lib.cpp)

### TODO

* support win32 and win64 at the same time
  * currently you should specify --arch manually

