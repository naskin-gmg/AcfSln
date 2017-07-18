// Qt includes
#include <QtCore/QCoreApplication>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

// ACF includes
#include <ibase/IApplication.h>

#include <GeneratedFiles/XpcEditor/CXpcEditor.h>


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqtgui);
	Q_INIT_RESOURCE(icmpstr);
	Q_INIT_RESOURCE(AcfLoc);
	Q_INIT_RESOURCE(AcfSlnLoc);
	Q_INIT_RESOURCE(XpcEditor);

	QApplication::addLibraryPath("./");

	CXpcEditor instance(NULL, true);

	ibase::IApplication* applicationPtr = instance.GetInterface<ibase::IApplication>();
	if (applicationPtr != NULL){
		applicationPtr->InitializeApplication(argc, argv);

		instance.EnsureAutoInitComponentsCreated();

		return applicationPtr->Execute(argc, argv);
	}

	return -1;
}


