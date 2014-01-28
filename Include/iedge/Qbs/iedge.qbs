import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "iedge"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iipr" }
	Depends{ name: "iproc" }
	Depends{ name: "iimg" }
	Depends{ name: "imeas" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "iipr" }
		Depends{ name: "iproc" }
		Depends{ name: "iimg" }
		Depends{ name: "imeas" }
		Depends{ name: "icomp" }
	}
}
