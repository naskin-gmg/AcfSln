import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "iprop"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtgui" }
	Depends{ name: "iser" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iqtgui" }
		Depends{ name: "iser" }
		Depends{ name: "Qt.widgets" }
	}
}
