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
		               std::uint16_t dataLength,
		               const std::vector<std::shared_ptr<RecordData>> &data);
};

#endif // RESOURCE_RECORD_H
