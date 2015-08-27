import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iprm" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "iprm" }
		Depends{ name: "icomp" }
	}
}
