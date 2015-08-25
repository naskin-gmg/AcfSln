import qbs 1.0

Module{
	Depends{ name: 'i2d' }
	Depends{ name: 'ibase' }
	Depends{ name: 'icomp' }
	Depends{ name: 'ilog' }
	Depends{ name: 'imath' }
	Depends{ name: 'imod' }
	Depends{ name: 'iprm' }
	Depends{ name: 'iproc' }
	Depends{ name: 'iser' }
	Depends{ name: 'istd' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/ihotf.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
		
	]
}
