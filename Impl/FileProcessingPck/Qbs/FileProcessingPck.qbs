import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "FileProcessingPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ifileproc" }

	Export{
		Depends{ name: "ifileproc" }
	}
}
