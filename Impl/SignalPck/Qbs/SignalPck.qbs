import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "SignalPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "isig" }

	Export{
		Depends{ name: "isig" }
	}
}
