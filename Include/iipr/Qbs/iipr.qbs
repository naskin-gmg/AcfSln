import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "iipr"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "imeas" }
	Depends{ name: "iproc" }
	Depends{ name: "iimg" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "imeas" }
		Depends{ name: "iproc" }
		Depends{ name: "iimg" }
		Depends{ name: "icomp" }
	}
}
