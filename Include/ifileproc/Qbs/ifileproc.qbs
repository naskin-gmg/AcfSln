import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ifileproc"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ifile" }
	Depends{ name: "iimg" }
	Depends{ name: "iproc" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "ifile" }
		Depends{ name: "iimg" }
		Depends{ name: "iproc" }
		Depends{ name: "icomp" }
	}
}
