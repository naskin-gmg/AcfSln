import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iprm" }
	Depends{ name: "icomp" }
	Depends{ name: "ibase" }

	Export{
		Depends{ name: "iprm" }
		Depends{ name: "icomp" }
		Depends{ name: "ibase" }
	}
}
