import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "iqtmm"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "imm" }
	Depends{ name: "iimg" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.svg" }

	Export{
		Depends{ name: "imm" }
		Depends{ name: "iimg" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.svg" }
	}
}
