#this file will be included to some target, so every target get the target_defaults settings
{
	'target_defaults':{
		'target_conditions':[
			['_type=="shared_library" and OS=="linux"',{'cflags':['-fPIC']}]
		],

		'conditions':[
			['OS=="win"',{'defines':[ '__WINDOWS_GCC__' ]}]
		],
		'configurations':{
			'Debug':{
				#'defines':[
					#'DEBUG'
				#],
				'cflags':[
					'-O2',
					'-Wall'
				]
			},
			'Release':{

			}
		}
	}
}
