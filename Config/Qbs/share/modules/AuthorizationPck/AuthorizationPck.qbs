import qbs 1.0

Module{
	Depends{ name: 'iauth' }
	Depends{ name: 'icomp' }
	Depends{ name: 'ilog' }
	Depends{ name: 'imod' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'iprm' }
	Depends{ name: 'iser' }
	Depends{ name: 'istd' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }

	readonly property path componentFilePath: path + '/../../../../../Bin/' + acf.compilerDir + '/AuthorizationPck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
