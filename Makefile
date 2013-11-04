.PHONY:test test1 test2 test3 clean

version ?= 0.6.21
ifdef version
  node_version += "--target=$(version)"
endif

config=release
ifeq ($(config), "release")
  target_dir = Release
  build_config = "--release"
else
  target_dir = Debug
  build_config = "--debug"
endif

all:
	@if [[ ! -d test/node_modules ]]; then \
		mkdir test/node_modules; \
	fi;
	@node-gyp configure $(node_version)&& \
	node-gyp build $(build_config) && \
	cp -rf build/$(target_dir)/clearsilver.node test/node_modules;

#dev test
test:
	@cd test && ./cs -c test.cs -h test.hdf

#test read cs and hdf from local file
test1:
	@node test/test1.js

#test read cs from local file but set hdf data from a string.
test2:
	@node test/test2.js

#test setValue
test3:
	@node test/test3.js

#test read cs from local file and set any value by setValue
test4:
	@node test/test4.js


clean:
	rm -rf build/*
