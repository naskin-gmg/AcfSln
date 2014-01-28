import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

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
