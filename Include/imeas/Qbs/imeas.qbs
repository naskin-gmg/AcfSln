import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "imeas"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iproc" }
	Depends{ name: "iimg" }
	Depends{ name: "icomp" }
	Depends{ name: "imath" }

	Export{
		Depends{ name: "iproc" }
		Depends{ name: "iimg" }
		Depends{ name: "icomp" }
		Depends{ name: "imath" }
	}
}
