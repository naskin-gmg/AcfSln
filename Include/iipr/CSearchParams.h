#ifndef iipr_CSearchParams_included
#define iipr_CSearchParams_included


// ACF includes
#include <iimg/CGeneralBitmap.h>

// ACF-Solutions includes
#include <iipr/ISearchParams.h>


namespace iipr
{


/**
	Definition of standard search parameters.
*/
class CSearchParams: virtual public iipr::ISearchParams
{
public:
	CSearchParams();
	
	// reimplemented (iipr::ISearchParams)
	virtual const ISearchConstraints* GetSearchConstraints() const override;
	virtual double GetMinScore() const override;
	virtual void SetMinScore(double minScore) override;
	virtual const istd::CRange& GetRotationRange() const override;
	virtual void SetRotationRange(const istd::CRange& angleRange) override;
	virtual const istd::CRange& GetScaleRange() const override;
	virtual void SetScaleRange(const istd::CRange& scaleRange) override;
	virtual int GetNominalModelsCount() const override;
	virtual void SetNominalModelsCount(int nominalModelsCount) override;
	virtual bool IsRotationEnabled() const; 
	virtual void SetRotationEnabled(bool isRotationEnabled);
	virtual bool IsScaleEnabled() const;
	virtual void SetScaleEnabled(bool isScaleEnabled);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive & archive) override;

protected:
	istd::CRange m_angleRange;
	istd::CRange m_scaleRange;
	int m_nominalModelsCount;
	double m_minScore;
	bool m_isRotationEnabled;
	bool m_isScaleEnabled;
};


} // namespace iipr


#endif // !iipr_CSearchParams_included


