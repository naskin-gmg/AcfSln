import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ihotfgui" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "ihotfgui" }
		Depends{ name: "icomp" }
	}
}
