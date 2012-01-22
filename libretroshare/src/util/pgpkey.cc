#include <stdint.h>
#include <util/radix64.h>
#include "pgpkey.h"

#include <iostream>
#include <stdexcept>

#define PGP_PACKET_TAG_PUBLIC_KEY  6
#define PGP_PACKET_TAG_USER_ID    13 
#define PGP_PACKET_TAG_SIGNATURE   2 

#define PGP_CRC24_INIT 0xB704CEL
#define PGP_CRC24_POLY 0x1864CFBL

#define PGP_CERTIFICATE_START_STRING "-----BEGIN PGP PUBLIC KEY BLOCK-----"
#define PGP_CERTIFICATE_END_STRING   "-----END PGP PUBLIC KEY BLOCK-----"
//
// All size are big endian
// MPI: 2 bytes size (length in bits) + string of octets
//
bool PGPKeyManagement::createMinimalKey(const std::string& pgp_certificate,std::string& cleaned_certificate) 
{
	try
	{
		// 0 - Extract Radix64 portion of the certificate
		//
		int n = pgp_certificate.length() ;
		int i=0 ;
		std::string version_string = "" ;

		while(i < n && pgp_certificate[i] != '\n') ++i ;	// remove first part -----BEGIN PGP CERTIFICATE-----
		++i ;
		while(i < n && pgp_certificate[i] != '\n') version_string += pgp_certificate[i++] ;	// remove first part Version: [fdfdfdf]
		++i ;
		while(i < n && pgp_certificate[i] != '\n') ++i ;	// remove blank line

		++i ;

		int j=n-1 ;

		while(j>0 && pgp_certificate[j] != '=' && j>=i) --j ;

		std::string radix_cert = pgp_certificate.substr(i,j-i) ;

		std::cerr << "extracted radix cert: " << std::endl;
		std::cerr << radix_cert ;

		// 1 - Convert armored key into binary key
		//

		char *keydata = NULL ;
		size_t len = 0 ;

		Radix64::decode(radix_cert,keydata,len) ;

		unsigned char *data = (unsigned char *)keydata ;

		std::cerr << "Total size: " << len << std::endl;

		uint8_t packet_tag;
		uint32_t packet_length ;

		// 2 - parse key data, only keep public key data, user id and self-signature.

		bool public_key=false ;
		bool own_signature=false ;
		bool user_id=false ;

		while(true) 
		{
			PGPKeyParser::read_packetHeader(data,packet_tag,packet_length) ;

			std::cerr << "Header:" << std::endl;
			std::cerr << "  Packet tag: " << (int)packet_tag << std::endl;
			std::cerr << "  Packet length: " << packet_length << std::endl;

			data += packet_length ;

			if(packet_tag == PGP_PACKET_TAG_PUBLIC_KEY)
				public_key = true ;
			if(packet_tag == PGP_PACKET_TAG_USER_ID)
				user_id = true ;
			if(packet_tag == PGP_PACKET_TAG_SIGNATURE)
				own_signature = true ;

			if(public_key && own_signature && user_id) 
				break ;

			if( (uint64_t)data - (uint64_t)keydata >= len )
				break ;
		}

		std::string outstring ;
		Radix64::encode(keydata,(uint64_t)data - (uint64_t)keydata,outstring) ;

		uint32_t crc = compute24bitsCRC((unsigned char *)keydata,(uint64_t)data - (uint64_t)keydata) ;

		unsigned char tmp[3] = { (crc >> 16) & 0xff, (crc >> 8) & 0xff, crc & 0xff } ;
		std::string crc_string ;
		Radix64::encode((const char *)tmp,3,crc_string) ;

		std::cerr << "After signature pruning: " << std::endl;
		std::cerr << outstring << std::endl;

		cleaned_certificate = std::string(PGP_CERTIFICATE_START_STRING) + "\n" + version_string + "\n\n" ;

		for(uint32_t i=0;i<outstring.length();i+=64)
			cleaned_certificate += outstring.substr(i,64) + "\n" ;

		cleaned_certificate += "=" + crc_string + "\n" ;
		cleaned_certificate += std::string(PGP_CERTIFICATE_END_STRING) + "\n" ;

		return true ;
	}
	catch(std::exception& e)
	{
		cleaned_certificate = "" ;
		std::cerr << "Certificate cleaning failed: " << e.what() << std::endl;
		return false ;
	}
}

uint32_t PGPKeyManagement::compute24bitsCRC(unsigned char *octets, size_t len)
{
	long crc = PGP_CRC24_INIT;
	int i;
	while (len--) {
		crc ^= (*octets++) << 16;
		for (i = 0; i < 8; i++) {
			crc <<= 1;
			if (crc & 0x1000000)
				crc ^= PGP_CRC24_POLY;
		}
	}
	return crc & 0xFFFFFFL;
}

uint64_t PGPKeyParser::read_KeyID(unsigned char *& data)
{
	uint64_t val = 0 ;

	val |= uint64_t( *data ) << 56 ; ++data ;
	val |= uint64_t( *data ) << 48 ; ++data ;
	val |= uint64_t( *data ) << 40 ; ++data ;
	val |= uint64_t( *data ) << 32 ; ++data ;
	val |= uint64_t( *data ) << 24 ; ++data ;
	val |= uint64_t( *data ) << 16 ; ++data ;
	val |= uint64_t( *data ) <<  8 ; ++data ;
	val |= uint64_t( *data ) <<  0 ; ++data ;

	return val ;
}

uint32_t PGPKeyParser::read_125Size(unsigned char *& data)
{
	uint8_t b1 = *data ;
	++data ;

	if(b1 < 192)
		return b1 ;

	uint8_t b2 = *data ;

	if(b1 < 224)
		return ((b1-192) << 8) + b2 + 192 ;

	if(b1 != 0xff) 
		throw std::runtime_error("GPG parsing error") ;

	uint8_t b3 = *data ; ++data ;
	uint8_t b4 = *data ; ++data ;
	uint8_t b5 = *data ; ++data ;

	return (b2 << 24) | (b3 << 16) | (b4 << 8) | b5 ;
}

uint32_t PGPKeyParser::read_partialBodyLength(unsigned char *& data)
{
	uint8_t b1 =*data ;
	++data ;

	return 1 << (b1 & 0x1F) ;
}


void PGPKeyParser::read_packetHeader(unsigned char *& data,uint8_t& packet_tag,uint32_t& packet_length)
{
	uint8_t b1 = *data ;
	++data ;

	bool new_format = b1 & 0x40 ;

	if(new_format)
	{
		std::cerr << "Packet is in new format" << std::endl;
		packet_tag = b1 & 0x3f ;
		packet_length = read_125Size(data) ;
	}
	else
	{
		std::cerr << "Packet is in old format" << std::endl;
		uint8_t length_type = b1 & 0x03 ;
		packet_tag  = (b1 & 0x3c) >> 2 ;

		int length_size ;
		switch(length_type)
		{
			case 0: length_size = 1 ;
					  break ;
			case 1: length_size = 2 ;
					  break ;
			case 2: length_size = 4 ;
					  break ;
			default:
					  throw std::runtime_error("Unhandled length type!") ;
		}

		packet_length = 0 ;
		for(int k=0;k<length_size;++k)
		{
			packet_length <<= 8 ;
			packet_length |= *data ;
			++data ;
		}
	}
}

