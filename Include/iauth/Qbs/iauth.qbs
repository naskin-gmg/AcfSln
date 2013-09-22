import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
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
