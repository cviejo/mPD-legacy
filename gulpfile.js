var gulp  = require("gulp");
var spawn = require("child_process").spawn;
var exec  = require("child_process").exec;

var run  = process.argv.indexOf("-run") + 1;
var opts = {
	stdio : "inherit",
	cwd   : process.cwd()
};

var make;


gulp.task('default', function(){

	gulp.watch('./src/**/*.*', function(e) {

		if (make){
			make.kill();
			exec("ps -a | grep mPD | cut -f 1 -d \" \" | xargs kill");
		}

		make = spawn("make", opts);

		make.on('close', function(err) {

			if (!err && run){
				make = spawn("make", ["RunRelease"], opts);
			}
		});
	});
});

