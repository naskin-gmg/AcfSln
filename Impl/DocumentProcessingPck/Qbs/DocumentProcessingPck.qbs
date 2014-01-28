import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "DocumentProcessingPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqtgui" }
	Depends{ name: "idocproc" }

	Export{
		Depends{ name: "iqtgui" }
		Depends{ name: "idocproc" }
	}
}
