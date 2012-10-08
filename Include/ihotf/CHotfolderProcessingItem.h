#ifndef ihotf_CHotfolderProcessingItem_included
#define ihotf_CHotfolderProcessingItem_included


// Qt includes
#include <QtCore/QDateTime>

// ACF includes
#include "imod/TModelWrap.h"

// AcfSln includes
#include "ihotf/IHotfolderProcessingItem.h"


namespace ihotf
{


/**
	Interface for a single processing item in the hotfolder.
*/
class CHotfolderProcessingItem: virtual public ihotf::IHotfolderProcessingItem
{
public:
	CHotfolderProcessingItem();

	// reimplemented (ihotf::IHotfolderProcessingItem)
	virtual QByteArray GetItemUuid() const;
	virtual int GetProcessingState() const;
	virtual void SetProcessingState(int processingState);
	virtual QString GetInputFile() const;
	virtual void SetInputFile(const QString& inputFile);
	virtual QString GetOutputFile() const;
	virtual void SetOutputFile(const QString& outputFile);
	virtual double GetProcessingTime() const;
	virtual void SetProcessingTime(double processingTime);
	virtual const QDateTime& GetStartTime() const;
	virtual void SetStartTime(const QDateTime& startTime);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	QString m_inputFile;
	QString m_outputFile;
	int m_processingState;
	double m_processingTime;
	QDateTime m_startTime;

	mutable QByteArray m_itemId;
};


} // namespace ihotf


#endif // !ihotf_CHotfolderProcessingItem_included


