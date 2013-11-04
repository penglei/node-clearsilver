var fs = require("fs"),
    path = require("path"),
    ClearSilver = require('clearsilver').ClearSilver;

process.chdir(__dirname);

var hdffile = "test2.hdf",
    csfile = "test2.cs";

var cs = new ClearSilver(csfile);

var hdfStr = fs.readFileSync(hdffile);

cs.setHdfString(hdfStr);

cs.render(function(str){
    console.log(str);
});
