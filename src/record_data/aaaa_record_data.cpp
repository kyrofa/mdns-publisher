#include <array>
#include <sstream>

#include "record_data/aaaa_record_data.h"

namespace
{
	const int IP_ADDRESS_BYTE_COUNT = 16;
}

AAAARecordData::AAAARecordData(const std::string &ipAddress) :
    m_ipAddress(boost::asio::ip::address_v6::from_string(ipAddress))
{
}

AAAARecordData::AAAARecordData(
        const std::vector<std::uint8_t>::const_iterator &begin,
        const std::vector<std::uint8_t>::const_iterator &end)
{
	auto bufferSize = std::distance(begin, end);
	if (bufferSize != IP_ADDRESS_BYTE_COUNT)
	{
		std::stringstream stream;
		stream << "Expected " << IP_ADDRESS_BYTE_COUNT << " bytes for AAAA "
		       << "Record, got " << bufferSize;
		throw std::runtime_error(stream.str());
	}

	std::array<std::uint8_t, IP_ADDRESS_BYTE_COUNT> bytes;
	std::copy(begin, begin+IP_ADDRESS_BYTE_COUNT, bytes.begin());

	m_ipAddress = boost::asio::ip::address_v6(bytes);
}

RecordType AAAARecordData::type() const
{
	return RecordType::AAAA;
}

std::uint16_t AAAARecordData::size() const
{
	return IP_ADDRESS_BYTE_COUNT;
}

void AAAARecordData::toBuffer(std::vector<std::uint8_t> &buffer) const
{
	std::array<std::uint8_t, IP_ADDRESS_BYTE_COUNT> bytes = m_ipAddress.to_bytes();

	buffer.insert(buffer.end(), bytes.cbegin(), bytes.cend());
}

boost::asio::ip::address_v6 AAAARecordData::ipAddress() const
{
	return m_ipAddress;
}

bool AAAARecordData::operator==(const AAAARecordData &other) const
{
	return ipAddress() == other.ipAddress();
}
