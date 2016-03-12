#include <array>

#include "record_data/a_record_data.h"

ARecordData::ARecordData(const std::string &ipAddress) :
    m_ipAddress(boost::asio::ip::address_v4::from_string(ipAddress))
{
}

ARecordData::ARecordData(const std::vector<std::uint8_t> &buffer)
{
	if (buffer.size() != 4)
	{
		throw std::runtime_error("Buffer isn't 4 bytes long");
	}

	std::array<std::uint8_t, 4> bytes;
	std::copy(buffer.cbegin(), buffer.cend(), bytes.begin());

	m_ipAddress = boost::asio::ip::address_v4(bytes);
}

RecordType ARecordData::type() const
{
	return RecordType::A;
}

void ARecordData::toBuffer(std::vector<std::uint8_t> &buffer) const
{
	std::array<std::uint8_t, 4> bytes = m_ipAddress.to_bytes();

	buffer.insert(buffer.end(), bytes.cbegin(), bytes.cend());
}

boost::asio::ip::address_v4 ARecordData::ipAddress() const
{
	return m_ipAddress;
}
