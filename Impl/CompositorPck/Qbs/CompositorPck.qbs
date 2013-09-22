import qbs.base 1.0
import "../../../Config/Qbs/AcfPackage.qbs" as AcfPackage

AcfPackage{
	name: "CompositorPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icmpstr" }

	Export{
		Depends{ name: "icmpstr" }
	}
}
