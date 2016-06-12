#ifndef RESOURCE_RECORD_H
#define RESOURCE_RECORD_H

#include <string>
#include <vector>
#include <memory>

#include "record_class.h"
#include "record_type.h"
#include "record_data/record_data.h"

class ResourceRecord
{
	public:
		ResourceRecord(const std::string &name, RecordType recordType,
		               RecordClass recordClass, std::uint32_t timeToLive,
		               const std::shared_ptr<RecordData> &data);
		ResourceRecord(const std::vector<std::uint8_t>::const_iterator &begin,
		               const std::vector<std::uint8_t>::const_iterator &end);

		void toBuffer(std::vector<std::uint8_t> &buffer) const;

		const std::string &name() const;
		RecordType recordType() const;
		RecordClass recordClass() const;
		std::uint32_t timeToLive() const;
		std::uint16_t dataLength() const;
		std::shared_ptr<RecordData> data() const;

		bool operator==(const ResourceRecord &other) const;

	private:
		std::string m_name;
		RecordType m_recordType;
		RecordClass m_recordClass;
		std::uint32_t m_timeToLive;
		std::shared_ptr<RecordData> m_data;
};

#endif // RESOURCE_RECORD_H
