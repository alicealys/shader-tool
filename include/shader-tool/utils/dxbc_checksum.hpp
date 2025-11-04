#pragma once

namespace alys::utils::dxbc
{
	bool CalculateDXBCChecksum(std::uint8_t* pData, std::uint32_t dwSize, std::uint32_t dwHash[4]);
}
