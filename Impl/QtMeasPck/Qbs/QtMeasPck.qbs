import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "QtMeasPck"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtmeas" }

	Export{
		Depends{ name: "iqtmeas" }
	}
}
