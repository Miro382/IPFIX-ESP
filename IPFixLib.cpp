#include <stdint.h>
#include <string.h>
#include "IPFixLib.h"

uint32_t seq_num = 0;

void swap_bytes_2(union Byte_2 *by)
{
  unsigned char temp = by->b[0];
  by->b[0] = by->b[1];
  by->b[1] = temp;
}

void swap_bytes_4(union Byte_4 *by)
{
  unsigned char temp = by->b[0];
  by->b[0] = by->b[3];
  by->b[3] = temp;

  temp = by->b[1];
  by->b[1] = by->b[2];
  by->b[2] = temp;
}

struct IPFix_Data_Header Get_IPFIX_Header(uint16_t _length, uint32_t _export_time, uint32_t _observ_dom_id)
{
  IPFix_Data_Header ipfixdata_header;
  memset(&ipfixdata_header, 0, sizeof (IPFix_Data_Header));
  ipfixdata_header.Version_Number.number = 10;
  ipfixdata_header.Length.number = _length;//(16 + 20);
  ipfixdata_header.Export_Time.number = _export_time;
  ipfixdata_header.Sequence_Number.number = seq_num;
  ipfixdata_header.Observation_Domain_ID.number = _observ_dom_id;

  swap_bytes_2(&ipfixdata_header.Version_Number);
  swap_bytes_2(&ipfixdata_header.Length);
  swap_bytes_4(&ipfixdata_header.Export_Time);
  swap_bytes_4(&ipfixdata_header.Sequence_Number);
  swap_bytes_4(&ipfixdata_header.Observation_Domain_ID);

  return ipfixdata_header;
}

struct IPFix_Data_Template Get_IPFIX_Template(uint16_t _set_id, uint16_t _length, uint16_t _template_id, uint16_t _fieldcount)
{
  IPFix_Data_Template ipfixdata_temp;
  memset(&ipfixdata_temp, 0, sizeof (IPFix_Data_Template));
  ipfixdata_temp.SetID.number = _set_id;
  ipfixdata_temp.Length.number = _length;
  ipfixdata_temp.TemplateID.number = _template_id;
  ipfixdata_temp.FieldCount.number = _fieldcount;

  swap_bytes_2(&ipfixdata_temp.SetID);
  swap_bytes_2(&ipfixdata_temp.Length);
  swap_bytes_2(&ipfixdata_temp.TemplateID);
  swap_bytes_2(&ipfixdata_temp.FieldCount);

  return ipfixdata_temp;
}

struct IPFix_Data_Template_Description Get_IPFIX_Template_Description(uint16_t _data_type, uint16_t _data_length)
{
  IPFix_Data_Template_Description ipfixdata_temp_desc;
  memset(&ipfixdata_temp_desc, 0, sizeof (IPFix_Data_Template_Description));
  ipfixdata_temp_desc.Data_Type.number = _data_type;
  ipfixdata_temp_desc.Data_Length.number = _data_length;

  swap_bytes_2(&ipfixdata_temp_desc.Data_Type);
  swap_bytes_2(&ipfixdata_temp_desc.Data_Length);

  return ipfixdata_temp_desc;
}

struct IPFix_Data_Template_Description_Enterprise Get_IPFIX_Template_Description_Enterprise(uint16_t _data_type, uint16_t _data_length, uint32_t _enterprise, int enterpriseBit)
{
  IPFix_Data_Template_Description_Enterprise ipfixdata_temp_desc;
  memset(&ipfixdata_temp_desc, 0, sizeof (IPFix_Data_Template_Description_Enterprise));

  if (enterpriseBit > 0)
  {
    _data_type += 32768;
  }
  
  ipfixdata_temp_desc.Data_Type.number = _data_type;
  ipfixdata_temp_desc.Data_Length.number = _data_length;
  ipfixdata_temp_desc.Enterprise_Number.number = _enterprise;

  swap_bytes_2(&ipfixdata_temp_desc.Data_Type);
  swap_bytes_2(&ipfixdata_temp_desc.Data_Length);
  swap_bytes_4(&ipfixdata_temp_desc.Enterprise_Number);

  return ipfixdata_temp_desc;
}


struct IPFix_Data Get_IPFIX_Data(uint16_t _set_id, uint16_t _length)
{
  IPFix_Data ipfixdata_data;
  memset(&ipfixdata_data, 0, sizeof (IPFix_Data));
  ipfixdata_data.SetID.number = _set_id;
  ipfixdata_data.Length.number = _length;

  swap_bytes_2(&ipfixdata_data.SetID);
  swap_bytes_2(&ipfixdata_data.Length);

  return ipfixdata_data;
}

struct IPFix_Data_Field_4 Get_IPFIX_Data_Field_4(uint32_t _datafield)
{
  IPFix_Data_Field_4 ipfixdata_data_f;
  memset(&ipfixdata_data_f, 0, sizeof (IPFix_Data_Field_4));
  ipfixdata_data_f.DataField.number = _datafield;

  swap_bytes_4(&ipfixdata_data_f.DataField);

  return ipfixdata_data_f;
}

uint16_t header_length_calc(int fieldcount, int enterprisefields, int type_calc)
{
  if(type_calc == 1)
  {
    return 16 + template_length_calc(fieldcount, enterprisefields) + data_length_calc(fieldcount);
  }else if(type_calc == 2){
    return 16 + template_length_calc(fieldcount, enterprisefields);
  }else{
    return 16 + data_length_calc(fieldcount);
  }
}

uint16_t template_length_calc(int fieldcount, int enterprisefields)
{
  return 8 + (4 * fieldcount) + (enterprisefields * 4);
}

uint16_t data_length_calc(int fieldcount)
{
  return 4 + (4 * fieldcount);
}

void increment_seq_num()
{
  if (seq_num >= 4294967295)
  {
    seq_num = 0;
  }else{
    seq_num++;
  }
}
