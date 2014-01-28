import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "iqtmeas"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "imeas" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "imeas" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
	}
}
