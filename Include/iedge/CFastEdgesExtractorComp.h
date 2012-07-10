#ifndef iedge_CFastEdgesExtractorComp_included
#define iedge_CFastEdgesExtractorComp_included


// ACF includes
#include "iimg/IBitmap.h"
#include "iimg/CScanlineMask.h"
#include "iproc/TSyncProcessorCompBase.h"

// ACF-Solutions includes
#include "imeas/INumericParams.h"
#include "imeas/INumericConstraints.h"

// QSF includes
#include "iedge/IEdgesExtractor.h"



namespace iedge
{



class CFastEdgesExtractorComp:
			public iproc::TSyncProcessorCompBase<IEdgesExtractor>,
			public imeas::INumericConstraints
{
public:
	typedef iproc::TSyncProcessorCompBase<IEdgesExtractor> BaseClass;

	I_BEGIN_COMPONENT(CFastEdgesExtractorComp);
		I_REGISTER_INTERFACE(imeas::INumericConstraints);
		I_ASSIGN(m_defaultThresholdParamCompPtr, "DefaultThresholdParam", "Default threshold parameter, if not taken from parameters", false, "DefaultThresholdParam");
		I_ASSIGN(m_thresholdParamIdAttrPtr, "ThresholdParamId", "Threshold parameter ID (imeas::INumericValue)", false, "EdgeThreshold");
		I_ASSIGN(m_defaultAoiCompPtr, "DefaultAoi", "Area of interest used if not specified in parameters", false, "DefaultAoi");
		I_ASSIGN(m_aoiParamIdAttrPtr, "AoiParamId", "ID of area of interest in parameter set", false, "AoiParams");
		I_ASSIGN(m_maxNodesCountAttrPtr, "MaxNodesCount", "Maximal number of nodes in container", true, 50000);
	I_END_COMPONENT;

	//	reimplemented (iedge::IEdgesExtractor)
	virtual bool DoContourExtraction(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& bitmap,
				CEdgeLine::Container& result) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (imeas::INumericConstraints)
	virtual int GetNumericValuesCount() const;
	virtual QString GetNumericValueName(int index) const;
	virtual QString GetNumericValueDescription(int index) const;
	virtual const imeas::IUnitInfo& GetNumericValueUnitInfo(int index) const;

private:
	enum
	{
		THRESHOLD_FACTOR = 510 // Maximum value of derivative strenghtness
	};


	struct ExtNode
	{
		i2d::CVector2d position;
		i2d::CVector2d derivative;
		double weight;

		ExtNode* prevPtr;
		ExtNode* nextPtr;

		double prevWeight;
		double nextWeight;

		mutable bool isExtracted;
	};


	struct PixelDescriptor
	{
		qint16 dx;
		qint16 dy;
		quint32 dirLength2;
		ExtNode* listReference;
	};


	class InternalContainer
	{
	public:
		InternalContainer(int size);

		bool IsContainerFull() const;

		void ExtractContours(CEdgeLine::Container& result);

		ExtNode* AddElementToList();

	private:
		QVector<ExtNode> m_buffer;
		int m_freeIndex;

		bool m_isContainerFull;
	};

	// static methods
	static void TryConnectElements(
					ExtNode* neightborNodePtr,
					ExtNode* nodePtr);

	static void AddPointToContour(
				double posX,
				double posY,
				double derivativeX,
				double derivativeY,
				double weight,
				PixelDescriptor* destLine1,
				PixelDescriptor* destLine2,
				int x,
				int /*y*/,
				InternalContainer& container);

	static void CalcFullDerivative(
				const quint8* sourceLine1,
				const quint8* sourceLine2,
				PixelDescriptor* destLine,
				int x);

	static void CalcFullDerivativeLine(
				const quint8* sourceLine1,
				const quint8* sourceLine2,
				PixelDescriptor* destLine,
				int sourceWidth);

	static void CalcPoint(
				const quint8* sourceLine1,
				const quint8* sourceLine2,
				PixelDescriptor* destLine1,
				PixelDescriptor* destLine2,
				PixelDescriptor* destLine3,
				int x,
				int y,
				quint32 threshold2Factor,
				InternalContainer& container);

	static void CalcLine(
				const quint8* sourceLine1,
				const quint8* sourceLine2,
				PixelDescriptor* destLine1,
				PixelDescriptor* destLine2,
				PixelDescriptor* destLine3,
				int y,
				int sourceWidth,
				quint32 threshold2Factor,
				InternalContainer& container);

	I_REF(imeas::INumericParams, m_defaultThresholdParamCompPtr);
	I_ATTR(QByteArray, m_thresholdParamIdAttrPtr);
	I_REF(i2d::IObject2d, m_defaultAoiCompPtr);
	I_ATTR(QByteArray, m_aoiParamIdAttrPtr);
	I_ATTR(int, m_maxNodesCountAttrPtr);
};


} // namespace iedge


#endif // !iedge_CFastEdgesExtractorComp_included


