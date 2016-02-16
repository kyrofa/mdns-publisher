#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/asio/detail/socket_ops.hpp>

#include "query.h"

Query::Query(const std::string &name, RecordType recordType,
             RecordClass recordClass) :
    m_name(name),
    m_recordType(recordType),
    m_recordClass(recordClass)
{
}

Query::Query(const std::vector<std::uint8_t> &buffer)
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

	if (std::distance(iterator, buffer.cend()) < 4)
	{
		throw std::out_of_range("Buffer doesn't have room for type and class");
	}

	using boost::asio::detail::socket_ops::network_to_host_short;

	// Extract record type
	uint8_t high = *iterator++;
	uint8_t low = *iterator++;
	m_recordType = static_cast<RecordType>(network_to_host_short(
	                                           (high << 8) | low));

	// Extract record class
	high = *iterator++;
	low = *iterator;
	m_recordClass = static_cast<RecordClass>(network_to_host_short(
	                                             (high << 8) | low));
}

void Query::toBuffer(std::vector<uint8_t> &buffer)
{
	buffer.clear();

	std::vector<std::string> domainNameSections;
	boost::split(domainNameSections, m_name, boost::is_any_of("."));

	// Push the domain name octets onto the buffer
	for(const auto &section : domainNameSections)
	{
		buffer.push_back(section.size());
		for(const auto &character : section)
		{
			buffer.push_back(character);
		}
	}
	buffer.push_back(0); // End of domain name

	// Now push the record type
	auto recordType = boost::asio::detail::socket_ops::host_to_network_short(m_recordType);
	buffer.push_back((recordType & 0xff00) >> 8);
	buffer.push_back(recordType & 0x00ff);

	// And finally push the record class
	auto recordClass = boost::asio::detail::socket_ops::host_to_network_short(m_recordClass);
	buffer.push_back((recordClass & 0xff00) >> 8);
	buffer.push_back(recordClass & 0x00ff);
}

std::string Query::name() const
{
	return m_name;
}

RecordType Query::recordType() const
{
	return m_recordType;
}

RecordClass Query::recordClass() const
{
	return m_recordClass;
}
