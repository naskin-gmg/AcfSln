import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "imm"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icomp" }

	Export{
		Depends{ name: "icomp" }
	}
}
