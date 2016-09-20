#include <ihotfgui/CFileNamingParamsGuiComp.h>

// ACF includes
#include <istd/CChangeGroup.h>


namespace ihotfgui
{


// reimplemented (imod::IModelEditor)

void CFileNamingParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	ifileproc::IFileNamingParams* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	ifileproc::IFileNamingParams::OverwriteStrategy overwriteStrategy = ifileproc::IFileNamingParams::RM_NUMBERING;
	if (OverwriteRadio->isChecked()){
		overwriteStrategy = ifileproc::IFileNamingParams::RM_OVERWRITE;
	}

	istd::CChangeGroup changeGroup(objectPtr);

	objectPtr->SetOverwriteStrategy(overwriteStrategy);
	objectPtr->SetPrefix(PrefixEdit->text());
	objectPtr->SetSuffix(SuffixEdit->text());
}


// protected methods

// reimplemenented (iqtgui::TGuiObserverWrap)

void CFileNamingParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	ifileproc::IFileNamingParams* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int overwriteStrategy = objectPtr->GetOverwriteStrategy();
		switch (overwriteStrategy){
			case ifileproc::IFileNamingParams::RM_NUMBERING:
				AutoNumberingRadio->setChecked(true);
				break;
			case ifileproc::IFileNamingParams::RM_OVERWRITE:
				OverwriteRadio->setChecked(true);
				break;
			default:
				I_CRITICAL();
				break;
		}

		PrefixEdit->setText(objectPtr->GetPrefix());
		SuffixEdit->setText(objectPtr->GetSuffix());
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CFileNamingParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(PrefixEdit, SIGNAL(textChanged(const QString&)), this, SLOT(OnModelChanged()));
	connect(SuffixEdit, SIGNAL(textChanged(const QString&)), this, SLOT(OnModelChanged()));
	connect(OverwriteRadio, SIGNAL(toggled(bool)), this, SLOT(OnModelChanged()));
	connect(AutoNumberingRadio, SIGNAL(toggled(bool)), this, SLOT(OnModelChanged()));
}


// protected slots

void CFileNamingParamsGuiComp::OnModelChanged()
{
	BaseClass::DoUpdateModel();
}


} // namespace ihotfgui


