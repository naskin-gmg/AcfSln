import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "iwizgui"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iwiz" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iwiz" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
	}
}
