import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "WizardPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iwiz" }

	Export{
		Depends{ name: "iwiz" }
	}
}
