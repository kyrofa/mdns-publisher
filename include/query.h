#ifndef QUERY_H
#define QUERY_H

#include <cstdint>
#include <string>
#include <vector>

#include "record_class.h"
#include "record_type.h"

class Query
{
	public:
		Query(const std::string &name, RecordType recordType,
		      RecordClass recordClass);
		Query(const std::vector<std::uint8_t> &buffer);

		void toBuffer(std::vector<std::uint8_t> &buffer);

		std::string name() const;
		RecordType recordType() const;
		RecordClass recordClass() const;

	private:
		std::string m_name;
		RecordType m_recordType;
		RecordClass m_recordClass;
};

#endif // QUERY_H
