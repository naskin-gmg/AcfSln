#ifndef iipr_CCaliperParams_included
#define iipr_CCaliperParams_included


#include <iipr/ICaliperParams.h>


namespace iipr
{


class CCaliperParams: virtual public ICaliperParams
{
public:
	CCaliperParams();

	// reimplemented (iipr::ICaliperParams)
	virtual double GetWeightThreshold() const override;
	virtual void SetWeightThreshold(double threshold) override;
	virtual int GetPolarityMode() const override;
	virtual void SetPolarityMode(int mode) override;
	virtual int GetDirectionMode() const override;
	virtual void SetDirectionMode(int mode) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	double m_weightThreshold;
	int m_polarityMode;
	int m_directionMode;
};


} // namespace iipr


#endif // !iipr_CCaliperParams_included


