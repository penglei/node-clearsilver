var fs = require("fs"),
    path = require("path"),
    ClearSilver = require('clearsilver').ClearSilver;

process.chdir(__dirname);

var hdffile = "test4.hdf",
    csfile = "test4.cs";

var hdfStr = fs.readFileSync(hdffile);
var csStr = fs.readFileSync(csfile);

var newWithFilePath = 0;

if (newWithFilePath) {
	var cs = new ClearSilver(csfile);
} else {
	//console.log("init ClearSilver with no cs file path");
	var cs = new ClearSilver();
	cs.setCsString(csStr);
}

cs.setHdfString(hdfStr);

cs.setValue("c.d", "cd was setted in js");

cs.render(function(str){
    console.log(str);
});

//console.log("use new data render cs template\n");

cs.setHdfString("a.b=AA\na.c=BB");
cs.setValue("c.d", "cd was setted again in js");
cs.render(function(str){
    console.log(str);
});
