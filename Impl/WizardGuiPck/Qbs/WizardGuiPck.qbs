import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "WizardGuiPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iwizgui" }

	Export{
		Depends{ name: "iwizgui" }
	}
}
