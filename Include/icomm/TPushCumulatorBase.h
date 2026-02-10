// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <istd/IPolymorphic.h>


namespace icomm
{


/**
	Base implementation of message reader using message unalignmented data packages.
	Read packages will be inserted using \c AppendData,
	when message header is completed \c CalculateMessageSize will be called.
	After complete message is completed \c ProcessMessage will be called.
*/
template <class Header>
class TPushCumulatorBase: virtual public istd::IPolymorphic
{
public:
	TPushCumulatorBase();

	virtual void ResetBuffer();
	/**
		Fill the buffer using push mechanism.
	*/
	virtual bool AppendData(const quint8* dataPtr, int length);

protected:
	// abstract methods
	/**
		It calculates size of buffer using header.
	*/
	virtual quint32 CalculateMessageSize(const Header& header) = 0;
	/**
		It is called when buffer is read.
	*/
	virtual bool ProcessMessage(const Header& header, const quint8* dataPtr, int length) = 0;

	Header m_header;

	quint32 m_messageSize;
	int m_loadedBytes;

private:
	typedef QVector<quint8> Buffer;
	Buffer m_buffer;
};


template <class Header>
TPushCumulatorBase<Header>::TPushCumulatorBase()
{
	ResetBuffer();
}


template <class Header>
void TPushCumulatorBase<Header>::ResetBuffer()
{
	m_loadedBytes = -int(sizeof(m_header));
	Buffer emptyBuffer;
	m_buffer.swap(emptyBuffer);
}


template <class Header>
bool TPushCumulatorBase<Header>::AppendData(const quint8* dataPtr, int length)
{
	while (length > 0){
		if (m_loadedBytes < 0){
			int copyLength = qMin(-m_loadedBytes, length);

			memcpy((quint8*)&m_header + m_loadedBytes + sizeof(m_header), dataPtr, copyLength);
			
			dataPtr += copyLength;
			length -= copyLength;
			m_loadedBytes += copyLength;

			if (m_loadedBytes == 0){
				m_messageSize = CalculateMessageSize(m_header);
				m_buffer.resize(m_messageSize);
			}
		}

		if (m_loadedBytes >= 0){
			int copyLength = qMin(int(m_messageSize) - m_loadedBytes, length);
			
			if (copyLength > 0){
				memcpy(&m_buffer[m_loadedBytes], dataPtr, copyLength);

				dataPtr += copyLength;
				length -= copyLength;
				m_loadedBytes += copyLength;
			}

			if (quint32(m_loadedBytes) >= m_messageSize){
				Q_ASSERT(quint32(m_loadedBytes) == m_messageSize);
				
				bool processOk = ProcessMessage(m_header, &m_buffer[0], m_loadedBytes);
				ResetBuffer();
				
				if (!processOk){
					return false;
				}
			}
		}
	}

	return true;
}


} // namespace icomm


