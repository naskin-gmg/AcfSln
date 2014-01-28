import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "imm"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icomp" }

	Export{
		Depends{ name: "icomp" }
	}
}
