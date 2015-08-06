import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "imeas"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iproc" }
	Depends{ name: "iimg" }
	Depends{ name: "icomp" }
	Depends{ name: "imath" }
	Depends{ name: "iinsp" }

	Export{
		Depends{ name: "iproc" }
		Depends{ name: "iimg" }
		Depends{ name: "icomp" }
		Depends{ name: "imath" }
		Depends{ name: "iinsp" }
	}
}
