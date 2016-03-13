#ifndef RECORD_DATA_FACTORY_H
#define RECORD_DATA_FACTORY_H

#include <cstdint>
#include <memory>
#include <vector>

#include "record_type.h"
#include "record_data/record_data.h"

namespace RecordDataFactory
{
	std::shared_ptr<RecordData> create(
	        RecordType recordType,
	        const std::vector<std::uint8_t>::const_iterator &begin,
	        const std::vector<std::uint8_t>::const_iterator &end);
}

#endif // RECORD_DATA_FACTORY_H
