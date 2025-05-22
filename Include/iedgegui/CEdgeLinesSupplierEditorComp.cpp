#include <iedgegui/CEdgeLinesSupplierEditorComp.h>


// Qt includes
#include <QtCore/QTimer>

// ACF includes
#include <imath/CVarVector.h>

// ACF-Solutions includes
#include <iedge/IEdgeLinesProvider.h>
#include <iedge/CEdgeLineContainer.h>
#include <iedgegui/CEdgeLineContainerShape.h>


namespace iedgegui
{


CEdgeLinesSupplierEditorComp::CEdgeLinesSupplierEditorComp()
{
	m_edgesColorSchema.SetPen(iview::IColorSchema::SP_NORMAL, QPen(Qt::blue));
	connect(this, SIGNAL(OnSupplierParamsChangedSignal()), SLOT(on_ProcessButton_clicked()), Qt::QueuedConnection);
}


// protected methods

// reimplemented (iqtinsp::TSupplierGuiCompBase)

QWidget* CEdgeLinesSupplierEditorComp::GetParamsWidget() const
{
	Q_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


void CEdgeLinesSupplierEditorComp::OnSupplierParamsChanged()
{
	if (IsGuiCreated() && AutoUpdateButton->isChecked()){
		emit OnSupplierParamsChangedSignal();
	}
}


// reimplemented (iqt2d::TViewExtenderCompBase)

void CEdgeLinesSupplierEditorComp::CreateShapes(int /*sceneId*/, Shapes& result)
{
	iedgegui::CEdgeLineContainerShape* shapePtr = new iedgegui::CEdgeLineContainerShape(); 

	if (shapePtr != NULL){
		if (m_contoursColorSchemaCompPtr.IsValid()){
			shapePtr->SetUserColorSchema(m_contoursColorSchemaCompPtr.GetPtr());
		}
		else{
			shapePtr->SetUserColorSchema(&m_edgesColorSchema);
		}

		shapePtr->AssignToLayer(iview::IViewLayer::LT_INACTIVE);

		m_container.AttachObserver(shapePtr);

		result.PushBack(shapePtr);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CEdgeLinesSupplierEditorComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());
	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


void CEdgeLinesSupplierEditorComp::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(IsGuiCreated());

	BaseClass::UpdateGui(changeSet);

	UpdateAllViews();
}


// reimplemented (iqtgui::IGuiObject)

void CEdgeLinesSupplierEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	SaveDataButton->setVisible(m_resultsSaverCompPtr.IsValid());
}


void CEdgeLinesSupplierEditorComp::OnGuiHidden()
{
	AutoUpdateButton->setChecked(false);

	BaseClass::OnGuiHidden();
}


// reimplemented (imod::IObserver)

void CEdgeLinesSupplierEditorComp::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	m_container.Reset();

	iedge::IEdgeLinesProvider* providerPtr = CompCastPtr<iedge::IEdgeLinesProvider>(GetObservedObject());
	if (providerPtr != NULL ){
		const iedge::CEdgeLineContainer* resultContainerPtr = providerPtr->GetEdgesContainer();	

		if (resultContainerPtr != NULL){
			m_container.CopyFrom(*resultContainerPtr, istd::IChangeable::CM_CONVERT);
		}
	}

	BaseClass::AfterUpdate(modelPtr, changeSet);
}


// protected slots

void CEdgeLinesSupplierEditorComp::on_ProcessButton_clicked()
{
	DoTest();
}


void CEdgeLinesSupplierEditorComp::on_SaveDataButton_clicked()
{
	if (m_resultsSaverCompPtr->SaveToFile(m_container, "") == ifile::IFilePersistence::OS_FAILED){
		QMessageBox::warning(
					BaseClass::GetQtWidget(),
					QObject::tr("Error"),
					QObject::tr("Cannot save edge lines"));
	}
}


void CEdgeLinesSupplierEditorComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CEdgeLinesSupplierEditorComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


}//Namespace iedgegui





