import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

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
