#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/asio/detail/socket_ops.hpp>

#include "record_data/record_data_factory.h"
#include "resource_record.h"

ResourceRecord::ResourceRecord(const std::string &name, RecordType recordType,
                               RecordClass recordClass, std::uint32_t timeToLive,
                               const std::shared_ptr<RecordData> &data) :
    m_name(name),
    m_recordType(recordType),
    m_recordClass(recordClass),
    m_timeToLive(timeToLive),
    m_data(data)
{
}

ResourceRecord::ResourceRecord(const std::vector<uint8_t> &buffer)
{
	if (buffer.size() == 0)
	{
		throw std::runtime_error("Buffer was empty");
	}

	auto iterator = buffer.cbegin();

	// Extract domain name
	m_name.clear();
	while(*iterator != 0)
	{
		if (!m_name.empty())
		{
			m_name.push_back('.');
		}

		int segmentSize = *iterator++;
		if (std::distance(iterator, buffer.cend()) < segmentSize)
		{
			throw std::out_of_range(
			            "Domain segment size was larger than actual segment");
		}

		for (int i = 0; i < segmentSize; ++i)
		{
			m_name.push_back(*iterator++);
		}
	}
	++iterator;

	if (std::distance(iterator, buffer.cend()) < 10)
	{
		throw std::out_of_range("Buffer doesn't have room for rest of resource "
		                        "record");
	}

	using boost::asio::detail::socket_ops::network_to_host_short;

	// Extract record type
	std::uint8_t high = *iterator++;
	std::uint8_t low = *iterator++;
	m_recordType = static_cast<RecordType>(network_to_host_short(
	                                           (high << 8) | low));

	// Extract record class
	high = *iterator++;
	low = *iterator++;
	m_recordClass = static_cast<RecordClass>(network_to_host_short(
	                                             (high << 8) | low));

	using boost::asio::detail::socket_ops::network_to_host_long;

	high = *iterator++;
	std::uint8_t highMiddle = *iterator++;
	std::uint8_t lowMiddle = *iterator++;
	low = *iterator++;
	m_timeToLive = network_to_host_long((high << 24) | (highMiddle << 16) |
	                                    (lowMiddle << 8) | low);

	high = *iterator++;
	low = *iterator++;
	// Ignore length, the data creation will error if the length is wrong
	// anyway.
	// std::uint16_t length = network_to_host_short((high << 8) | low);

	m_data = RecordDataFactory::create(m_recordType, iterator, buffer.cend());
}

void ResourceRecord::toBuffer(std::vector<std::uint8_t> &buffer) const
{
	std::vector<std::string> nameSections;
	boost::split(nameSections, m_name, boost::is_any_of("."));

	// Push the domain name octets onto the buffer
	for(const auto &section : nameSections)
	{
		buffer.push_back(section.size());
		for(const auto &character : section)
		{
			buffer.push_back(character);
		}
	}
	buffer.push_back(0); // End of domain name

	// Now push the record type
	auto recordType = boost::asio::detail::socket_ops::host_to_network_short(
	                      m_recordType);
	buffer.push_back((recordType & 0xff00) >> 8);
	buffer.push_back(recordType & 0x00ff);

	// Now push the record class
	auto recordClass = boost::asio::detail::socket_ops::host_to_network_short(
	                       m_recordClass);
	buffer.push_back((recordClass & 0xff00) >> 8);
	buffer.push_back(recordClass & 0x00ff);

	// Now push the time to live (TTL)
	auto timeToLive = boost::asio::detail::socket_ops::host_to_network_long(
	                      m_timeToLive);
	buffer.push_back((timeToLive & 0xff000000) >> 24);
	buffer.push_back((timeToLive & 0x00ff0000) >> 16);
	buffer.push_back((timeToLive & 0x0000ff00) >> 8);
	buffer.push_back(timeToLive & 0x000000ff);

	auto length = boost::asio::detail::socket_ops::host_to_network_short(
	                  dataLength());
	buffer.push_back((length & 0xff00) >> 8);
	buffer.push_back(length & 0x00ff);

	m_data->toBuffer(buffer);
}

const std::string &ResourceRecord::name() const
{
	return m_name;
}

RecordType ResourceRecord::recordType() const
{
	return m_recordType;
}

RecordClass ResourceRecord::recordClass() const
{
	return m_recordClass;
}

std::uint32_t ResourceRecord::timeToLive() const
{
	return m_timeToLive;
}

std::uint16_t ResourceRecord::dataLength() const
{
	return m_data->size();
}

std::shared_ptr<RecordData> ResourceRecord::data() const
{
	return m_data;
}

bool ResourceRecord::operator==(const ResourceRecord &other) const
{
	return (name() == other.name() &&
	        recordType() == other.recordType() &&
	        recordClass() == other.recordClass() &&
	        timeToLive() == other.timeToLive() &&
	        dataLength() == other.dataLength() &&
	        *data() == *other.data());
}
