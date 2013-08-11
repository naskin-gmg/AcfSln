import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "InspectionPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iinsp" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "iinsp" }
		Depends{ name: "icomp" }
	}
}
