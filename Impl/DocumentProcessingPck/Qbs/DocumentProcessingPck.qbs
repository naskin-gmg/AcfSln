import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "DocumentProcessingPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqtgui" }
	Depends{ name: "idocproc" }

	Export{
		Depends{ name: "iqtgui" }
		Depends{ name: "idocproc" }
	}
}
