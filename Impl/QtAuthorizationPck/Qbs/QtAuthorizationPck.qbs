import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "QtAuthorizationPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtauth" }

	Export{
		Depends{ name: "iqtauth" }
	}
}
