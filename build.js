
const NWJS_VERSION = process.argv[2];
console.log("build for", NWJS_VERSION);

const cp = require('child_process');
const path = require('path');
const os = require('os');
const NwBuilder = require('nw-builder');

const IS_WINDOWS = os.platform() == 'win32';

const nwBuild = function () {
	console.log('build nwjs');
	var nw = new NwBuilder({
		version: NWJS_VERSION,
		files: [
			'index.html',
			'package.json',
			'node_modules/bindings/*',
			'build/Release/*.node',
	    ],
	    platforms: IS_WINDOWS ? ['win32', 'win64'] : ['osx64'],
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
};

if (IS_WINDOWS) {
	const lib = path.join(__dirname, '');
	cp.spawn(path.join(__dirname, 'rename_import_lib/rename_import_lib.exe'), [
		'-i', 'build/Release/nw_simple_addon.node',
		'-o', 'build/Release/nw_simple_addon.node',
		'-s', 'NW_RENAMED',
		'-r', 'nw.exe', 'MyApp.exe',
	], {
		cwd: __dirname,
		stdio: [0, 1, 2],
	}).on('close', function (ret) {
		if (ret !== 0) {
			throw new Error('rename_import_lib failed: ' + ret);
		}
		nwBuild();
	});
} else {
	nwBuild();
}



