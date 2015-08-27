import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iproc" }
	Depends{ name: "ifile" }
	Depends{ name: "iimg" }
	Depends{ name: "icomp" }
	Depends{ name: "iinsp" }

	Export{
		Depends{ name: "iproc" }
		Depends{ name: "ifile" }
		Depends{ name: "iimg" }
		Depends{ name: "icomp" }
		Depends{ name: "iinsp" }
	}
}
