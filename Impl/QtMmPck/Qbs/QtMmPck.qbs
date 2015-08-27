import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtmm" }

	Export{
		Depends{ name: "iqtmm" }
	}
}
