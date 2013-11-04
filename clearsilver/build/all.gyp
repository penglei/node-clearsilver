{
#	'target_defaults':{#this only affect targets in this file
#		'target_conditions':[
#			['_type=="shared_library" and OS=="linux"',{'cflags':['-fPIC']}]
#		]
#	},
	'targets': [
		{
			'target_name': 'All',
			'type': 'none',
			'dependencies': [
				'../cs/run.gyp:*',
				#'../src/neolib.gyp:*',
			],
		}
	]
}
