#ifndef TINYIPFIXLIB_H
#define TINYIPFIXLIB_H

union TByte_4
{
  uint32_t number;
  unsigned char b[4];
};

union TByte_2
{
  uint16_t number;
  unsigned char b[2];
};


typedef struct __attribute__((__packed__)) TinyIPFix_Header {
  union TByte_2 Flags_Length;
  unsigned char SequenceNumber;
};

typedef struct __attribute__((__packed__)) TinyIPFix_Set_Header {
  unsigned char SetID;
  unsigned char Length;
};

typedef struct TinyIPFix_Data_Template_Description {
  union TByte_2 Data_Type;
  union TByte_2 Data_Length;
};

typedef struct TinyIPFix_Data_Template_Description_Enterprise {
  union TByte_2 Data_Type;
  union TByte_2 Data_Length;
  union TByte_4 Enterprise_Number;
};

typedef struct TinyIPFix_Data_Field_4 {
  union TByte_4 DataField;
};

void tiny_swap_bytes_2(union TByte_2 *by);

void tiny_swap_bytes_4(union TByte_4 *by);

struct TinyIPFix_Header Get_TinyIPFix_Header(uint16_t _length, uint8_t message_type);

struct TinyIPFix_Set_Header Get_TinyIPFix_Set_Header(uint8_t _set_id, uint8_t _length);

struct TinyIPFix_Data_Template_Description Get_TinyIPFIX_Template_Description(uint16_t _data_type, uint16_t _data_length);

struct TinyIPFix_Data_Template_Description_Enterprise Get_TinyIPFIX_Template_Description_Enterprise(uint16_t _data_type, uint16_t _data_length, uint32_t _enterprise, int enterpriseBit);

struct TinyIPFix_Data_Field_4 Get_TinyIPFIX_Data_Field_4(uint32_t _datafield);

uint16_t tiny_header_length_calc(int fieldcount, int enterprisefields, int type_calc);

uint16_t tiny_template_length_calc(int fieldcount, int enterprisefields);

uint16_t tiny_data_length_calc(int fieldcount);

void tiny_increment_seq_num();


#endif
