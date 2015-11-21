#!/bin/sh

npm install
nw-gyp configure --target=0.12.3
nw-gyp build
node nw-build.js

