import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "ProcessingGuiPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iprocgui" }

	Export{
		Depends{ name: "iprocgui" }
	}
}
