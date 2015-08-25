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
	Depends{ name: 'iipr' }
	Depends{ name: 'ilog' }
	Depends{ name: 'imath' }
	Depends{ name: 'imeas' }
	Depends{ name: 'imod' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'iprm' }
	Depends{ name: 'iproc' }
	Depends{ name: 'iqt' }
	Depends{ name: 'iser' }
	Depends{ name: 'istd' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }

	readonly property path componentFilePath: path + '/../../../../../Bin/' + acf.compilerDir + '/IprPck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
