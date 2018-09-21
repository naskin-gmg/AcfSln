import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "icomm"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ilog" }
	Depends{ name: "iser" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "ilog" }
		Depends{ name: "iser" }
		Depends{ name: "icomp" }
	}
}
