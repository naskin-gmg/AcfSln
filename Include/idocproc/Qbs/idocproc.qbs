import qbs.base 1.0
import "../../../Config/Qbs/AcfStaticLibrary.qbs" as AcfStaticLibrary

AcfStaticLibrary{
	name: "idocproc"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iproc" }
	Depends{ name: "iimg" }
	Depends{ name: "idoc" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "iproc" }
		Depends{ name: "iimg" }
		Depends{ name: "idoc" }
		Depends{ name: "icomp" }
	}
}
