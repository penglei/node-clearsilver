var fs = require("fs"),
    path = require("path"),
    ClearSilver = require('clearsilver').ClearSilver;

process.chdir(__dirname);

var hdffile = "test3.hdf",
    csfile = "test3.cs";

var cs = new ClearSilver(csfile, hdffile);

cs.setValue("c.d", "CCDD");

cs.render(function(str){
    console.log(str);
});
