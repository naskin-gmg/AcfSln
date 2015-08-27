import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iblobgui" }
	Depends{ name: "iblob" }

	Export{
		Depends{ name: "iblobgui" }
		Depends{ name: "iblob" }
	}
}
