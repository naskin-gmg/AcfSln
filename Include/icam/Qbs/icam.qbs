import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "icam"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iproc" }
	Depends{ name: "ifile" }
	Depends{ name: "iimg" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "iproc" }
		Depends{ name: "ifile" }
		Depends{ name: "iimg" }
		Depends{ name: "icomp" }
	}
}
