import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
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
