import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "ProcessingGuiPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iprocgui" }

	Export{
		Depends{ name: "iprocgui" }
	}
}
