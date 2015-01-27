#pragma once


// Qt includes
#include <QtCore/QString>
#include <QtCore/QList>


namespace iblob
{


class CBlobDescriptorInfo
{
public:
	enum BlobDescriptorType
	{
		BDT_PERIMETER,
		BDT_AREA,
		BDT_CIRCULARITY,
		BDT_USER = 1000
	};

	CBlobDescriptorInfo(
				int descriptorType, 
				const QString& fullName, 
				const QString& shortName,
				const QString& description = QString());

	QString GetFullName() const;
	QString GetShortName() const;
	QString GetDescription() const;
	int GetDescriptorType() const;

private:
	int m_descriptorType;
	QString m_fullName;
	QString m_shortName;
	QString m_description;
};


// public inline methods

inline QString CBlobDescriptorInfo::GetFullName() const
{
	return m_fullName;
}


inline QString CBlobDescriptorInfo::GetShortName() const
{
	return m_shortName;
}


inline QString CBlobDescriptorInfo::GetDescription() const
{
	return m_description;
}


inline int CBlobDescriptorInfo::GetDescriptorType() const
{
	return m_descriptorType;
}


typedef QList<CBlobDescriptorInfo> BlobDescriptorInfoList;


} // namespace iblob





