#include <map>
#include <functional>

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
	const std::map<RecordType, RecordDataCreationFunction> m = {
	    {RecordType::A, newRecordData<ARecordData>},
	    {RecordType::AAAA, newRecordData<AAAARecordData>},
	};
}

std::shared_ptr<RecordData> RecordDataFactory::create(const std::vector<std::uint8_t> &buffer)
{

}
