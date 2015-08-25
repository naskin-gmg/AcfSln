import qbs 1.0

Module{
	Depends{ name: 'i2d' }
	Depends{ name: 'ibase' }
	Depends{ name: 'icmm' }
	Depends{ name: 'icomp' }
	Depends{ name: 'idoc' }
	Depends{ name: 'ifile' }
	Depends{ name: 'iimg' }
	Depends{ name: 'iinsp' }
	Depends{ name: 'ilog' }
	Depends{ name: 'imath' }
	Depends{ name: 'imeas' }
	Depends{ name: 'imod' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'iprm' }
	Depends{ name: 'iproc' }
	Depends{ name: 'iqt' }
	Depends{ name: 'iqtgui' }
	Depends{ name: 'iqtsig' }
	Depends{ name: 'iser' }
	Depends{ name: 'isig' }
	Depends{ name: 'istd' }
	Depends{ name: 'iwidgets' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }

	readonly property path componentFilePath: path + '/../../../../../Bin/' + acf.compilerDir + '/QtSignalPck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
