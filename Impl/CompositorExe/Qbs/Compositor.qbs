import qbs.base 1.0
import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

Project{
	StaticProduct{
		name: "_Compositor"
		type: ["staticlibrary"]

		files: ["../*.arx", "../*.qrc"]

//		acf.acfConfigurationFile: "../../../Config/Full.xpc"

		Depends{ name: "Arxc" }

		Depends{ name: "AcfSlnConfig" }
		Depends{ name: "CompositorPck" }
		Depends{ name: "QtGuiPck" }
		Depends{ name: "QtPck" }
		Depends{ name: "FilePck" }
		Depends{ name: "BasePck" }
		Depends{ name: "PackagePck" }

		Export{
			cpp.includePaths: product.buildDirectory + "/" + product.name
		}
	}

	ApplicationProduct{
		name: "Compositor"

		files: ["../*.cpp"]

		Depends{ name: "_Compositor" }

		Depends{ name: "AcfSlnLoc" }
		Depends{ name: "AcfLoc" }

		Depends{ name: "icmpstr" }
		Depends{ name: "iqtgui" }
		Depends{ name: "ifile" }
		Depends{ name: "ipackage" }

		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.gui" }
	}
}
