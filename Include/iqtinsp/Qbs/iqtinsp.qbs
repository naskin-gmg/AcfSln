import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "iqtinsp"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iinsp" }
	Depends{ name: "iview" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iinsp" }
		Depends{ name: "iview" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
	}
}
