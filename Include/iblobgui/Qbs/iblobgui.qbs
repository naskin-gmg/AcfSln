import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "iblobgui"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iblob" }
	Depends{ name: "iview" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iblob" }
		Depends{ name: "iview" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
	}
}
