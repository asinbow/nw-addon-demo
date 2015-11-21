
const NwBuilder = require('nw-builder');

var nw = new NwBuilder({
  version: '0.12.3',
  files: [
    'index.html',
    'package.json',
    'node_modules/bindings',
    'build/Release/*.node',
  ],
  //platforms: ['win32', 'win64'],
  platforms: ['osx64'],
  buildDir: '.',
  appName: "MyApp", // auto detect from package.json
  appVersion: "0.0.1",
  zip: false,
});

nw.on('log', console.log);

nw.build().then(() => {
console.log('package created');
}).catch((err) => {
console.log('Build failed !!!', err);
});
