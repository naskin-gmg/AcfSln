import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "WizardPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iwiz" }

	Export{
		Depends{ name: "iwiz" }
	}
}
