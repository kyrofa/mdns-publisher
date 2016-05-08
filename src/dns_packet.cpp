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
