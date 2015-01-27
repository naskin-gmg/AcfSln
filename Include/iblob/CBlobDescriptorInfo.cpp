#include "iblob/CBlobDescriptorInfo.h"


namespace iblob
{


// public methods

CBlobDescriptorInfo::CBlobDescriptorInfo(
			int descriptorType, 
			const QString& fullName, 
			const QString& shortName,
			const QString& description)
{
	m_descriptorType = descriptorType;
	m_fullName = fullName;
	m_shortName = shortName;
	m_description = description;
}


} // namespace iblob


