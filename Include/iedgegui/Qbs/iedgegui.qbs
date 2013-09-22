import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "iedgegui"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iedge" }
	Depends{ name: "iview" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iedge" }
		Depends{ name: "iview" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
	}
}
