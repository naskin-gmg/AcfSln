#ifndef icalib_CSimpleLensCorrectionComp_included
#define icalib_CSimpleLensCorrectionComp_included


// ACF includes
#include <icomp/CComponentBase.h>

// ACF-Solutions includes
#include <icalib/CSimpleLensCorrection.h>


namespace icalib
{


/**
	Attributable component for simple lens correction.
*/
class CSimpleLensCorrectionComp: public icomp::CComponentBase, public CSimpleLensCorrection
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CSimpleLensCorrection BaseClass2;

	I_BEGIN_COMPONENT(CSimpleLensCorrectionComp);
		I_REGISTER_INTERFACE(ICalibration2d);
		I_REGISTER_INTERFACE(ITransformation2d);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(CSimpleLensCorrection);
		I_ASSIGN(m_defaultDistortionFactorAttrPtr, "DefaultDistortionFactor", "Default value of lens distortion factor", true, 0);
		I_ASSIGN(m_defaultOpticalCenterXAttrPtr, "DefaultOpticalCenterX", "Default value of lens optical center X", true, 0);
		I_ASSIGN(m_defaultOpticalCenterYAttrPtr, "DefaultOpticalCenterY", "Default value of lens optical center Y", true, 0);
		I_ASSIGN(m_doDistortionOnlyAttrPtr, "DoDistortionOnly", "If this is true the optical center moved in order to its value, else the optical center has (0, 0) coord and result vector translates to optical center", true, false);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(double, m_defaultDistortionFactorAttrPtr);
	I_ATTR(double, m_defaultOpticalCenterXAttrPtr);
	I_ATTR(double, m_defaultOpticalCenterYAttrPtr);
	I_ATTR(bool, m_doDistortionOnlyAttrPtr);
};


} // namespace icalib


#endif // !icalib_CSimpleLensCorrectionComp_included


