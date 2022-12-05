#pragma once

class BitFlag
{
	unsigned char value;
public:
	constexpr BitFlag(unsigned char value) :value(value) {}
	constexpr BitFlag operator|(const BitFlag& t)const { return value | t.value; }
	constexpr bool operator&(const BitFlag& t)const { return value | t.value; }
	constexpr bool operator()(unsigned char byte)const { return value & (1 << byte); }
	constexpr operator unsigned char()const { return value; }
};

