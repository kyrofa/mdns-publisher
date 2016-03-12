#include <map>
#include <functional>
#include <sstream>

#include "record_type.h"
#include "record_data/a_record_data.h"
#include "record_data/aaaa_record_data.h"
#include "record_data/record_data_factory.h"

namespace
{
	template<typename RecordDataClass>
	std::shared_ptr<RecordData> newRecordData(const std::vector<std::uint8_t> &buffer)
	{
		return std::make_shared<RecordDataClass>(buffer);
	}

	typedef std::function<std::shared_ptr<RecordData>(const std::vector<std::uint8_t> &buffer)> RecordDataCreationFunction;
	const std::map<RecordType, RecordDataCreationFunction> RECORD_DATA_TYPES = {
	    {RecordType::A, newRecordData<ARecordData>},
	    {RecordType::AAAA, newRecordData<AAAARecordData>},
	};
}

std::shared_ptr<RecordData> RecordDataFactory::create(
        RecordType recordType,
        const std::vector<std::uint8_t> &buffer)
{
	try
	{
		return RECORD_DATA_TYPES.at(recordType)(buffer);
	}
	catch(const std::out_of_range &)
	{
		std::stringstream stream;
		stream << "Unsupported record type: " << recordType;
		throw std::out_of_range(stream.str());
	}
}
