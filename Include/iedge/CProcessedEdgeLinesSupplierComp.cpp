#include <iedge/CProcessedEdgeLinesSupplierComp.h>

// Qt includes
#include <QtCore/QFile>
#include <QtCore/QTextStream>


namespace iedge
{


// reimplemented (iedge::IEdgeLinesProvider)

const CEdgeLineContainer* CProcessedEdgeLinesSupplierComp::GetEdgesContainer() const
{
	return GetWorkProduct();
}


// protected methods


void CProcessedEdgeLinesSupplierComp::SerializeContours(const CEdgeLineContainer& result) const
{
	QFile outputFile;
	QTextStream outputFileStream(&outputFile);
	outputFile.setFileName("C:\\Work\\Develop\\Products\\RTVision\\Testing\\EdgesData\\EdgesData.txt");
	bool ok = outputFile.open(QIODevice::Text | QIODevice::WriteOnly |QIODevice::Append);
	if (!ok){
		return;
	}

	const int count = result.GetItemsCount();
	for (int i = 0; i < count; ++i){
		const iedge::CEdgeLine& oneLine = result.GetAt(i);
		const int nodesCount = oneLine.GetNodesCount();
		for (int j = 0; j < nodesCount; ++j){
			const iedge::CEdgeNode& oneNode = oneLine.GetNode(j);
			const i2d::CVector2d& nodePosition = oneNode.GetPosition();

			outputFileStream << i << " " << j << " " << nodePosition.GetX() << " " << nodePosition.GetY() << " " << oneNode.GetWeight() << "\n";
		}
	}
	if (outputFile.isOpen()){
		outputFileStream.flush();
		outputFile.close();
	}
}


// reimplemented (iinsp::TSupplierCompWrap)

iinsp::ISupplier::WorkStatus CProcessedEdgeLinesSupplierComp::ProduceObject(CEdgeLineContainer& result) const
{
	if (!m_edgeLinesProviderCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component architecture, 'EdgeLinesProvider' component reference is not set");

		return WS_FAILED;
	}

	if (!m_edgesProcessorCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component architecture, 'EdgesProcessor' component reference is not set");

		return WS_FAILED;
	}

	const CEdgeLineContainer* containerPtr = m_edgeLinesProviderCompPtr->GetEdgesContainer();
	if (containerPtr != NULL){
		Timer performanceTimer(this, "Edge processing");
		Q_UNUSED(performanceTimer);

		if (m_edgesProcessorCompPtr->DoLinesProcessing(GetModelParametersSet(), *containerPtr, result)){
			istd::TDelPtr<i2d::ICalibration2d> newCalibration;

			// copy calibration from original
			const i2d::ICalibration2d* calibrationPtr = containerPtr->GetCalibration();
			if (calibrationPtr != NULL){
				newCalibration.SetCastedOrRemove(calibrationPtr->CloneMe());
			}

			result.SetCalibration(newCalibration.PopPtr(), true);

			return WS_OK;
		}
	}

	return WS_FAILED;
}


// reimplemented (icomp::CComponentBase)

void CProcessedEdgeLinesSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_edgeLinesProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_edgeLinesProviderModelCompPtr.GetPtr(), m_edgeLinesSupplierCompPtr.GetPtr());
	}

	// Force components initialization
	m_edgeLinesProviderCompPtr.EnsureInitialized();
	m_edgesProcessorCompPtr.EnsureInitialized();
}


} // namespace iedge


