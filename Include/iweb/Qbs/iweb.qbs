import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "iweb"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icomp" }

	Export{
		Depends{ name: "icomp" }
	}
}
