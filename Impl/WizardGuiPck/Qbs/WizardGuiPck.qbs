import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "WizardGuiPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iwizgui" }

	Export{
		Depends{ name: "iwizgui" }
	}
}
