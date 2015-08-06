import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "iauth"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iser" }
	Depends{ name: "imod" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "iser" }
		Depends{ name: "imod" }
		Depends{ name: "icomp" }
	}
}
