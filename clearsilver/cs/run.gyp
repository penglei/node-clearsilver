{
	'includes': ['../build/common.gypi'],
	'targets': [
		{
			'type': 'executable',
			'dependencies': [
				'../src/neolib.gyp:neo',
			],
			'target_name': 'cs',
			'sources': [
				'cs.c',
			],
		}
	],
}
