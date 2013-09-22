import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "IprPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iipr" }

	Export{
		Depends{ name: "iipr" }
	}
}
