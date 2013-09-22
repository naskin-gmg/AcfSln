import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "ihotf"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iproc" }
	Depends{ name: "i2d" }
	Depends{ name: "ilog" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "iproc" }
		Depends{ name: "i2d" }
		Depends{ name: "ilog" }
		Depends{ name: "icomp" }
	}
}
