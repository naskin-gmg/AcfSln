import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "EdgePck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iedgegui" }
	Depends{ name: "iedge" }

	Export{
		Depends{ name: "iedgegui" }
		Depends{ name: "iedge" }
	}
}
