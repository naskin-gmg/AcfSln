import qbs 1.0

Module{
	Depends{ name: 'ibase' }
	Depends{ name: 'icomp' }
	Depends{ name: 'ilog' }
	Depends{ name: 'imod' }
	Depends{ name: 'iprm' }
	Depends{ name: 'iproc' }
	Depends{ name: 'iser' }
	Depends{ name: 'istd' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/iinsp.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
