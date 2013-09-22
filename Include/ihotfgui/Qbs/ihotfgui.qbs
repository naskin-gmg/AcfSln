import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "ihotfgui"

	files: ["../*.h", "../*.cpp", "../*.ui", "../*.qrc"]

	Depends{ name: "ihotf" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "ihotf" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
	}
}
