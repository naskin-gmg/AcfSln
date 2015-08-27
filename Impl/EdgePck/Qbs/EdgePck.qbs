import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iedgegui" }
	Depends{ name: "iedge" }

	Export{
		Depends{ name: "iedgegui" }
		Depends{ name: "iedge" }
	}
}
