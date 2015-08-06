import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
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
