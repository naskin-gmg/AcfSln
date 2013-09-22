import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "iqtipr"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iipr" }
	Depends{ name: "iview" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iipr" }
		Depends{ name: "iview" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
	}
}
