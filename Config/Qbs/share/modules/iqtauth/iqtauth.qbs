import qbs 1.0

Module{
	Depends{ name: 'i2d' }
	Depends{ name: 'iauth' }
	Depends{ name: 'ibase' }
	Depends{ name: 'icomp' }
	Depends{ name: 'ifile' }
	Depends{ name: 'ilog' }
	Depends{ name: 'imath' }
	Depends{ name: 'imod' }
	Depends{ name: 'iprm' }
	Depends{ name: 'iqt' }
	Depends{ name: 'iqtgui' }
	Depends{ name: 'iser' }
	Depends{ name: 'istd' }
	Depends{ name: 'iwidgets' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'Qt.widgets' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/iqtauth.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
		path + '/../../../../GeneratedFiles'
	]
}
