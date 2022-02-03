#ifndef IPFIXLIB_H
#define IPFIXLIB_H

union Byte_4
{
  uint32_t number;
  unsigned char b[4];
};

union Byte_2
{
  uint16_t number;
  unsigned char b[2];
};


typedef struct IPFix_Data_Header {
  union Byte_2 Version_Number;
  union Byte_2 Length;
  union Byte_4 Export_Time;
  union Byte_4 Sequence_Number;
  union Byte_4 Observation_Domain_ID;
};

typedef struct IPFix_Data_Template {
  union Byte_2 SetID;
  union Byte_2 Length;
  union Byte_2 TemplateID;
  union Byte_2 FieldCount;
};

typedef struct IPFix_Data_Template_Description {
  union Byte_2 Data_Type;
  union Byte_2 Data_Length;
};

typedef struct IPFix_Data_Template_Description_Enterprise {
  union Byte_2 Data_Type;
  union Byte_2 Data_Length;
  union Byte_4 Enterprise_Number;
};

typedef struct IPFix_Data {
  union Byte_2 SetID;
  union Byte_2 Length;
};

typedef struct IPFix_Data_Field_4 {
  union Byte_4 DataField;
};

void swap_bytes_2(union Byte_2 *by);

void swap_bytes_4(union Byte_4 *by);

struct IPFix_Data_Header Get_IPFIX_Header(uint16_t _length, uint32_t _export_time, uint32_t _observ_dom_id);

struct IPFix_Data_Template Get_IPFIX_Template(uint16_t _set_id, uint16_t _length, uint16_t _template_id, uint16_t _fieldcount);

struct IPFix_Data_Template_Description Get_IPFIX_Template_Description(uint16_t _data_type, uint16_t _data_length);

struct IPFix_Data_Template_Description_Enterprise Get_IPFIX_Template_Description_Enterprise(uint16_t _data_type, uint16_t _data_length, uint32_t _enterprise, int enterpriseBit);

struct IPFix_Data Get_IPFIX_Data(uint16_t _set_id, uint16_t _length);

struct IPFix_Data_Field_4 Get_IPFIX_Data_Field_4(uint32_t _datafield);

uint16_t header_length_calc(int fieldcount, int enterprisefields, int type_calc);

uint16_t template_length_calc(int fieldcount, int enterprisefields);

uint16_t data_length_calc(int fieldcount);

void increment_seq_num();

#endif
