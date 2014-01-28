import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
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
