import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "WizardGuiPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iwizgui" }

	Export{
		Depends{ name: "iwizgui" }
	}
}
