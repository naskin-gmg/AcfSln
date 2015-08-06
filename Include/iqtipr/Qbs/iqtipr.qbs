import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
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
