
const NwBuilder = require('nw-builder');

var nw = new NwBuilder({
  version: '0.12.3',
  files: ['index.html', 'build.js', 'package.json', 'node_modules/xiaoman_node_sys/**/*'],
  platforms: ['win32', 'win64'],
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