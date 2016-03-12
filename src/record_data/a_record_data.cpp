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

ARecordData::ARecordData(const std::vector<std::uint8_t> &buffer)
{
	if (buffer.size() != IP_ADDRESS_BYTE_COUNT)
	{
		std::stringstream stream;
		stream << "Buffer isn't " << IP_ADDRESS_BYTE_COUNT << " bytes long";
		throw std::runtime_error(stream.str());
	}

	std::array<std::uint8_t, IP_ADDRESS_BYTE_COUNT> bytes;
	std::copy(buffer.cbegin(), buffer.cend(), bytes.begin());

	m_ipAddress = boost::asio::ip::address_v4(bytes);
}

RecordType ARecordData::type() const
{
	return RecordType::A;
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
