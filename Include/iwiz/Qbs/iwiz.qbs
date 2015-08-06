import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "iwiz"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iproc" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "iproc" }
		Depends{ name: "icomp" }
	}
}
