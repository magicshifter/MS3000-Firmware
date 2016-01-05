var cp = require('child_process');
var watch = require('watch');
var join = require('path').join;

var assetDir = join(__dirname, 'src', 'assets');
var cssDir = join(__dirname, 'src', 'css');

console.log("start watching assets and css on mac os x");

watch.watchTree(assetDir, function (f, curr, prev) {
  if (typeof f === "object" && prev === null && curr === null) {
    // Finished walking the tree
    console.log('no changes');
  } else {
    //if (prev === null) {
      // f is a new file
      //console.log('new file added');
    //} else if (curr.nlink === 0) {
      // f was removed
      //console.log('file removed');
    //} else {
      // f was changed
    console.log('assets changed');
    cp.exec('make build-assets resources', function(err, stdout, stderr) {
      var stdOut = stdout.split('\n');
      stdOut.forEach(function(so) {
        console.log(so);
      });
    });
  }
});


watch.watchTree(cssDir, function (f, curr, prev) {
  if (typeof f === "object" && prev === null && curr === null) {
    // Finished walking the tree
    console.log('no changes');
  } else {
    console.log('css changed');
    cp.exec('make stylus', function(err, stdout, stderr) {
      var stdOut = stdout.split('\n');
      stdOut.forEach(function(so) {
        console.log(so);
      });
    });
  }
});
