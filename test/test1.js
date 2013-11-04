var fs = require("fs"),
    path = require("path"),
    ClearSilver = require('clearsilver').ClearSilver;

process.chdir(__dirname);

var hdffile = "test1.hdf",
    csfile = "test1.cs";

var cs = new ClearSilver(csfile, hdffile);

cs.render(function(str){
    console.log(str);
});
