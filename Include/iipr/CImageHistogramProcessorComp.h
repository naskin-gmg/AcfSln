// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iipr_CImageHistogramProcessorComp_included
#define iipr_CImageHistogramProcessorComp_included


// Qt includes
#include <QtCore/QStringList>

// ACF-Solutions includes
#include <imeas/IDiscreteDataSequence.h>
#include <iipr/CImageRegionProcessorCompBase.h>


namespace iipr
{


/**	
	Implementation of a processor for the image histogram calculation.
*/
class CImageHistogramProcessorComp: public CImageRegionProcessorCompBase
{
public:
	typedef CImageRegionProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CImageHistogramProcessorComp);
	I_END_COMPONENT;

	static bool CalculateHistogram(
				const iimg::IBitmap& inputBitmap,
				const i2d::IObject2d* aoiPtr,
				istd::IChangeable* outputPtr,
				QString& errorString);

protected:
	// reimplemented (CImageRegionProcessorCompBase)
	virtual bool ProcessImageRegion(
				const iimg::IBitmap& inputBitmap,
				const iprm::IParamsSet* paramsPtr,
				const i2d::IObject2d* aoiPtr,
				istd::IChangeable* outputPtr) const;
};


} // namespace iipr


#endif // !iipr_CImageHistogramProcessorComp_included

