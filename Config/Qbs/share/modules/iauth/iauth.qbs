import qbs 1.0

Module{
	Depends{ name: 'icomp' }
	Depends{ name: 'imod' }
	Depends{ name: 'iser' }
	Depends{ name: 'istd' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/iauth.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
		
	]
}
