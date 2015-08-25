import qbs 1.0

Module{
	Depends{ name: 'i2d' }
	Depends{ name: 'ibase' }
	Depends{ name: 'icomp' }
	Depends{ name: 'ifile' }
	Depends{ name: 'ilog' }
	Depends{ name: 'imath' }
	Depends{ name: 'imod' }
	Depends{ name: 'iprm' }
	Depends{ name: 'iproc' }
	Depends{ name: 'iqt' }
	Depends{ name: 'iqtgui' }
	Depends{ name: 'iser' }
	Depends{ name: 'istd' }
	Depends{ name: 'iwidgets' }
	Depends{ name: 'iwiz' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'Qt.widgets' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/iwizgui.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
		
	]
}
