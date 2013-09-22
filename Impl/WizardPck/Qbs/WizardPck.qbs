import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "WizardPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iwiz" }

	Export{
		Depends{ name: "iwiz" }
	}
}
