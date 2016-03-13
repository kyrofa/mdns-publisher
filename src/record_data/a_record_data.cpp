#include <array>
#include <sstream>

#include "record_data/a_record_data.h"

namespace
{
	const int IP_ADDRESS_BYTE_COUNT = 4;
}

ARecordData::ARecordData(const std::string &ipAddress) :
    m_ipAddress(boost::asio::ip::address_v4::from_string(ipAddress))
{
}

ARecordData::ARecordData(const std::vector<std::uint8_t>::const_iterator &begin,
                         const std::vector<std::uint8_t>::const_iterator &end)
{
	auto bufferSize = std::distance(begin, end);
	if (bufferSize != IP_ADDRESS_BYTE_COUNT)
	{
		std::stringstream stream;
		stream << "Expected " << IP_ADDRESS_BYTE_COUNT << " bytes for A Record,"
		       << "got " << bufferSize;
		throw std::runtime_error(stream.str());
	}

	std::array<std::uint8_t, IP_ADDRESS_BYTE_COUNT> bytes;
	std::copy(begin, begin+IP_ADDRESS_BYTE_COUNT, bytes.begin());

	m_ipAddress = boost::asio::ip::address_v4(bytes);
}

RecordType ARecordData::type() const
{
	return RecordType::A;
}

std::uint16_t ARecordData::size() const
{
	return IP_ADDRESS_BYTE_COUNT;
}

void ARecordData::toBuffer(std::vector<std::uint8_t> &buffer) const
{
	std::array<std::uint8_t, IP_ADDRESS_BYTE_COUNT> bytes = m_ipAddress.to_bytes();

	buffer.insert(buffer.end(), bytes.cbegin(), bytes.cend());
}

boost::asio::ip::address_v4 ARecordData::ipAddress() const
{
	return m_ipAddress;
}

bool ARecordData::operator==(const ARecordData &other) const
{
	return ipAddress() == other.ipAddress();
}
