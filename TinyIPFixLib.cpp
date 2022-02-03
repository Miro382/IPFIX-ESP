#include <stdint.h>
#include <string.h>
#include "TinyIPFixLib.h"

uint16_t t_seq_num = 0;

void tiny_swap_bytes_2(union TByte_2 *by)
{
  unsigned char temp = by->b[0];
  by->b[0] = by->b[1];
  by->b[1] = temp;
}

void tiny_swap_bytes_4(union TByte_4 *by)
{
  unsigned char temp = by->b[0];
  by->b[0] = by->b[3];
  by->b[3] = temp;

  temp = by->b[1];
  by->b[1] = by->b[2];
  by->b[2] = temp;
}

//message_type = 1 template ; 2 = data
struct TinyIPFix_Header Get_TinyIPFix_Header(uint16_t _length, uint8_t message_type)
{
  TinyIPFix_Header tinyipfix_header;
  memset(&tinyipfix_header, 0, sizeof (TinyIPFix_Header));
  tinyipfix_header.Flags_Length.number = _length;
  tinyipfix_header.SequenceNumber = (unsigned char)t_seq_num;

  if(message_type == 1)
  {
    tinyipfix_header.Flags_Length.number += 1024;
  }else if(message_type == 2)
  {
    tinyipfix_header.Flags_Length.number += 3072;
  }

  tiny_swap_bytes_2(&tinyipfix_header.Flags_Length);

  return tinyipfix_header;
}

struct TinyIPFix_Set_Header Get_TinyIPFix_Set_Header(uint8_t _set_id, uint8_t _length)
{
  TinyIPFix_Set_Header ipfixdata_set;
  memset(&ipfixdata_set, 0, sizeof (TinyIPFix_Set_Header));
  ipfixdata_set.SetID = _set_id;
  ipfixdata_set.Length = _length;

  return ipfixdata_set;
}

struct TinyIPFix_Data_Template_Description Get_TinyIPFIX_Template_Description(uint16_t _data_type, uint16_t _data_length)
{
  TinyIPFix_Data_Template_Description ipfixdata_temp_desc;
  memset(&ipfixdata_temp_desc, 0, sizeof (TinyIPFix_Data_Template_Description));
  ipfixdata_temp_desc.Data_Type.number = _data_type;
  ipfixdata_temp_desc.Data_Length.number = _data_length;

  tiny_swap_bytes_2(&ipfixdata_temp_desc.Data_Type);
  tiny_swap_bytes_2(&ipfixdata_temp_desc.Data_Length);

  return ipfixdata_temp_desc;
}

struct TinyIPFix_Data_Template_Description_Enterprise Get_TinyIPFIX_Template_Description_Enterprise(uint16_t _data_type, uint16_t _data_length, uint32_t _enterprise, int enterpriseBit)
{
  TinyIPFix_Data_Template_Description_Enterprise ipfixdata_temp_desc;
  memset(&ipfixdata_temp_desc, 0, sizeof (TinyIPFix_Data_Template_Description_Enterprise));

  if (enterpriseBit > 0)
  {
    _data_type += 32768;
  }
  
  ipfixdata_temp_desc.Data_Type.number = _data_type;
  ipfixdata_temp_desc.Data_Length.number = _data_length;
  ipfixdata_temp_desc.Enterprise_Number.number = _enterprise;

  tiny_swap_bytes_2(&ipfixdata_temp_desc.Data_Type);
  tiny_swap_bytes_2(&ipfixdata_temp_desc.Data_Length);
  tiny_swap_bytes_4(&ipfixdata_temp_desc.Enterprise_Number);

  return ipfixdata_temp_desc;
}


struct TinyIPFix_Data_Field_4 Get_TinyIPFIX_Data_Field_4(uint32_t _datafield)
{
  TinyIPFix_Data_Field_4 ipfixdata_data_f;
  memset(&ipfixdata_data_f, 0, sizeof (TinyIPFix_Data_Field_4));
  ipfixdata_data_f.DataField.number = _datafield;

  tiny_swap_bytes_4(&ipfixdata_data_f.DataField);

  return ipfixdata_data_f;
}

uint16_t tiny_header_length_calc(int fieldcount, int enterprisefields, int type_calc)
{
  if(type_calc == 1){
    return 3 + tiny_template_length_calc(fieldcount, enterprisefields);
  }else{
    return 3 + tiny_data_length_calc(fieldcount);
  }
}

uint16_t tiny_template_length_calc(int fieldcount, int enterprisefields)
{
  return 2 + (4 * fieldcount) + (enterprisefields * 4);
}

uint16_t tiny_data_length_calc(int fieldcount)
{
  return 2 + (4 * fieldcount);
}

void tiny_increment_seq_num()
{
  if (t_seq_num >= 255)
  {
    t_seq_num = 0;
  }else{
      t_seq_num++;
  }
}
