import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "iproc"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iprm" }
	Depends{ name: "icomp" }
	Depends{ name: "ibase" }

	Export{
		Depends{ name: "iprm" }
		Depends{ name: "icomp" }
		Depends{ name: "ibase" }
	}
}
