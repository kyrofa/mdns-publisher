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
		Query(const std::vector<std::uint8_t>::const_iterator &begin,
		      const std::vector<std::uint8_t>::const_iterator &end);

		void toBuffer(std::vector<std::uint8_t> &buffer) const;

		std::string name() const;
		RecordType recordType() const;
		RecordClass recordClass() const;

		bool operator==(const Query &other) const;

	private:
		std::string m_name;
		RecordType m_recordType;
		RecordClass m_recordClass;
};

#endif // QUERY_H
