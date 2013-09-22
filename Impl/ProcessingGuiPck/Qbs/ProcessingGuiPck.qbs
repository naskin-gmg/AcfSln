import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "ProcessingGuiPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iprocgui" }

	Export{
		Depends{ name: "iprocgui" }
	}
}
