import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "iweb"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icomp" }

	Export{
		Depends{ name: "icomp" }
	}
}
