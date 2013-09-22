import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "HotfolderGuiPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ihotfgui" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "ihotfgui" }
		Depends{ name: "icomp" }
	}
}
