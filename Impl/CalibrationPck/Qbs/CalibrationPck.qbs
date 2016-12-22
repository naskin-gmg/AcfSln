import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icalib" }
	Depends{ name: "i2d" }

	Export{
		Depends{ name: "icalib" }
		Depends{ name: "i2d" }
	}
}
