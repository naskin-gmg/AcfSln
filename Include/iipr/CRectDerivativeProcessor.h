#ifndef iipr_CRectDerivativeProcessor_included
#define iipr_CRectDerivativeProcessor_included


// ACF includes
#include "iproc/TSyncProcessorWrap.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"
#include "imeas/IUnitInfo.h"
#include "imeas/INumericConstraints.h"


namespace iipr
{


/**
	Calculate derivative of projection using rectangular filter kernel.
*/
class CRectDerivativeProcessor:
			public iproc::CSyncProcessorBase,
			virtual public imeas::INumericConstraints,
			virtual protected imeas::IUnitInfo
{
public:
	/**
		Do extremum features analyze.
	*/
	virtual bool DoDerivativeProcessing(const imeas::IDataSequence& source, double filterLength, imeas::IDataSequence& results);

	/**
		Get parameter ID used to extract caliper parameter object from parameter set.
	*/
	const std::string& GetFilterParamsId() const;

	/**
		Set parameter ID used to extract caliper parameter object from parameter set.
		It is only needed while using general processing interface iproc::IProcessor.
	*/
	void SetFilterParamsId(const std::string& id);

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (imeas::INumericConstraints)
	virtual int GetNumericValuesCount() const;
	virtual istd::CString GetNumericValueDescription(int index) const;
	virtual const imeas::IUnitInfo& GetNumericValueUnitInfo(int index) const;

	// reimplemented (imeas::IUnitInfo)
	virtual int GetUnitType() const;
	virtual istd::CString GetUnitName() const;
	virtual double GetDisplayMultiplicationFactor() const;
	virtual istd::CRange GetValueRange() const;
	virtual const imath::IDoubleManip& GetValueManip() const;

private:
	std::string m_filterParamsId;
};


// inline methods

inline const std::string& CRectDerivativeProcessor::GetFilterParamsId() const
{
	return m_filterParamsId;
}


inline void CRectDerivativeProcessor::SetFilterParamsId(const std::string& id)
{
	m_filterParamsId = id;
}


} // namespace iipr


#endif // !iipr_CRectDerivativeProcessor_included


