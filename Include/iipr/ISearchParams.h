#ifndef iipr_IISearchParams_included
#define iipr_IISearchParams_included


// ACF includes
#include <iser/ISerializable.h>
#include <i2d/CRectangle.h>
#include <iimg/IBitmap.h>


namespace iipr
{


class ISearchConstraints;


/**	
	Common interface for a set of parameters for a model search.
*/
class ISearchParams: virtual public iser::ISerializable
{
public:
	/**
		Get constraints for the search parameters.
	*/
	virtual const ISearchConstraints* GetSearchConstraints() const = 0;

	/**
		Get minimum score for a succefull search.
	*/
	virtual double GetMinScore() const = 0;

	/**
		Set minimum score for a succefull search.
	*/
	virtual void SetMinScore(double minScore) = 0;

	/**
		Get rotation range for the model search.
	*/
	virtual const istd::CRange& GetRotationRange() const = 0;

	/**
		Set rotation range for the model search.
		Range [0, 0] means the rotation invariance is off.
	*/
	virtual void SetRotationRange(const istd::CRange& angleRange) = 0;

	/**
		Get scale range for the model search.
		\sa SetScaleRange()
	*/
	virtual const istd::CRange& GetScaleRange() const = 0;

	/**
		Set scale range for the model search. Range [0, 0] means the scaling invariance is off.
	*/
	virtual void SetScaleRange(const istd::CRange& scaleRange) = 0;

	/**
		Get the number of models that should be found for a successfull search result.
	*/
	virtual int GetNominalModelsCount() const = 0;

	/**
		Set the number of models that should be found for a successfull search result.
		\param nominalModelsCount number of models. The negative value means any number of models can be found.
	*/
	virtual void SetNominalModelsCount(int nominalModelsCount) = 0;
};


} // namespace iipr


#endif // !iipr_IISearchParams_included

