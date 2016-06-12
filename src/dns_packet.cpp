#include <boost/asio/detail/socket_ops.hpp>

#include "dns_packet.h"

DnsPacket::DnsPacket(std::uint16_t identification, QueryResponse queryResponse,
                     Opcode opcode, AuthoritativeAnswer authoritativeAnswer,
                     Truncated truncated, RecursionDesired recursionDesired,
                     RecursionAvailable recursionAvailable,
                     AuthenticatedData authenticatedData,
                     CheckingDisabled checkingDisabled, ReturnCode returnCode,
                     const std::vector<Query> &questions,
                     const std::vector<ResourceRecord> &answerRecords,
                     const std::vector<ResourceRecord> &authorityRecords,
                     const std::vector<ResourceRecord> &additionalRecords) :
    m_id(identification),
    m_queryResponse(queryResponse),
    m_opCode(opcode),
    m_authoritativeAnswer(authoritativeAnswer),
    m_truncated(truncated),
    m_recursionDesired(recursionDesired),
    m_recursionAvailable(recursionAvailable),
    m_authenticatedData(authenticatedData),
    m_checkingDisabled(checkingDisabled),
    m_returnCode(returnCode),
    m_questions(questions),
    m_answerRecords(answerRecords),
    m_authorityRecords(authorityRecords),
    m_additionalRecords(additionalRecords)
{
}

void DnsPacket::toBuffer(std::vector<std::uint8_t> &buffer)
{
	using boost::asio::detail::socket_ops::host_to_network_short;

	// Push the ID
	auto idShort = host_to_network_short(id());
	buffer.push_back((idShort & 0xff00) >> 8);
	buffer.push_back(idShort & 0x00ff);

	std::uint8_t byte = 0;

	// Add the query/response bit
	byte |= (queryResponse() & 0x01) << 7;

	// Add the opcode
	byte |= (opCode() & 0x0f) << 3;

	// Add the authoritate answer
	byte |= (authoritativeAnswer() & 0x01) << 2;

	// Add truncation
	byte |= (truncated() & 0x01) << 1;

	// Add recurion desired
	byte |= recursionDesired() & 0x01;
	buffer.push_back(byte);
	byte = 0;

	// Add recursion available
	byte |= (recursionAvailable() & 0x01) << 6;

	// Add authenticated data
	byte |= (authenticatedData() & 0x01) << 5;

	// Add checking disabled
	byte |= (checkingDisabled() & 0x01) << 4;

	// Add return code
	byte |= returnCode() & 0x0f;
	buffer.push_back(byte);

	// Add total questions
	auto totalQuestions = host_to_network_short(questions().size());
	buffer.push_back((totalQuestions & 0xff00) >> 8);
	buffer.push_back(totalQuestions & 0x00ff);

	// Add total answer records
	auto totalAnswerRecords = host_to_network_short(answerRecords().size());
	buffer.push_back((totalAnswerRecords & 0xff00) >> 8);
	buffer.push_back(totalAnswerRecords & 0x00ff);

	// Add total authority records
	auto totalAuthorityRecords = host_to_network_short(authorityRecords().size());
	buffer.push_back((totalAuthorityRecords & 0xff00) >> 8);
	buffer.push_back(totalAuthorityRecords & 0x00ff);

	// Add total additional records
	auto totalAdditionalRecords = host_to_network_short(additionalRecords().size());
	buffer.push_back((totalAdditionalRecords & 0xff00) >> 8);
	buffer.push_back(totalAdditionalRecords & 0x00ff);

	for (const auto &query : questions())
	{
		query.toBuffer(buffer);
	}

	for (const auto &record : answerRecords())
	{
		record.toBuffer(buffer);
	}

	for (const auto &record : authorityRecords())
	{
		record.toBuffer(buffer);
	}

	for (const auto &record : additionalRecords())
	{
		record.toBuffer(buffer);
	}
}

std::uint16_t DnsPacket::id() const
{
	return m_id;
}

DnsPacket::QueryResponse DnsPacket::queryResponse() const
{
	return m_queryResponse;
}

DnsPacket::Opcode DnsPacket::opCode() const
{
	return m_opCode;
}

DnsPacket::AuthoritativeAnswer DnsPacket::authoritativeAnswer() const
{
	return m_authoritativeAnswer;
}

DnsPacket::Truncated DnsPacket::truncated() const
{
	return m_truncated;
}

DnsPacket::RecursionDesired DnsPacket::recursionDesired() const
{
	return m_recursionDesired;
}

DnsPacket::RecursionAvailable DnsPacket::recursionAvailable() const
{
	return m_recursionAvailable;
}

DnsPacket::AuthenticatedData DnsPacket::authenticatedData() const
{
	return m_authenticatedData;
}

DnsPacket::CheckingDisabled DnsPacket::checkingDisabled() const
{
	return m_checkingDisabled;
}

DnsPacket::ReturnCode DnsPacket::returnCode() const
{
	return m_returnCode;
}

const std::vector<Query>& DnsPacket::questions() const
{
	return m_questions;
}

const std::vector<ResourceRecord>& DnsPacket::answerRecords() const
{
	return m_answerRecords;
}

const std::vector<ResourceRecord>& DnsPacket::authorityRecords() const
{
	return m_authorityRecords;
}

const std::vector<ResourceRecord>& DnsPacket::additionalRecords() const
{
	return m_additionalRecords;
}
