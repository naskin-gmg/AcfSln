import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "iproc"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iprm" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "iprm" }
		Depends{ name: "icomp" }
	}
}
