var fs = require("fs"),
    path = require("path"),
    ClearSilver = require('clearsilver').ClearSilver;

process.chdir(__dirname);

var hdffile = "test.hdf", csfile = "test.cs";

var cs = new ClearSilver(csfile, hdffile);

//console.log(cs.render());

cs.render(function(str){
    console.log(str);
});
