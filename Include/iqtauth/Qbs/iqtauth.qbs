import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "iqtauth"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iauth" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iauth" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
	}
}
