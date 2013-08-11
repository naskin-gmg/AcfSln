import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iproc"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iprm" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "iprm" }
		Depends{ name: "icomp" }
	}
}
