import qbs 1.0

Module{
	Depends{ name: 'i2d' }
	Depends{ name: 'ibase' }
	Depends{ name: 'icomp' }
	Depends{ name: 'ifile' }
	Depends{ name: 'ilog' }
	Depends{ name: 'imath' }
	Depends{ name: 'imod' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'iprm' }
	Depends{ name: 'iqt' }
	Depends{ name: 'iqtex' }
	Depends{ name: 'iqtgui' }
	Depends{ name: 'iser' }
	Depends{ name: 'iservice' }
	Depends{ name: 'istd' }
	Depends{ name: 'iwidgets' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }

	readonly property path componentFilePath: path + '/../../../../../Bin/' + acf.compilerDir + '/QtExPck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
