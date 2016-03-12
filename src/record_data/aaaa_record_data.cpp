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

AAAARecordData::AAAARecordData(const std::vector<std::uint8_t> &buffer)
{
	if (buffer.size() != IP_ADDRESS_BYTE_COUNT)
	{
		std::stringstream stream;
		stream << "Buffer isn't " << IP_ADDRESS_BYTE_COUNT << " bytes long";
		throw std::runtime_error(stream.str());
	}

	std::array<std::uint8_t, IP_ADDRESS_BYTE_COUNT> bytes;
	std::copy(buffer.cbegin(), buffer.cend(), bytes.begin());

	m_ipAddress = boost::asio::ip::address_v6(bytes);
}

RecordType AAAARecordData::type() const
{
	return RecordType::AAAA;
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
