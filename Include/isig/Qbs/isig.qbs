import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "isig"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iprm" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "iprm" }
		Depends{ name: "icomp" }
	}
}
