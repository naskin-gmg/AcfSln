import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "ifileproc"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ifile" }
	Depends{ name: "iimg" }
	Depends{ name: "iproc" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "ifile" }
		Depends{ name: "iimg" }
		Depends{ name: "iproc" }
		Depends{ name: "icomp" }
	}
}
