#include <stdint.h>
#include <string.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "TinyIPFixLib.h"

#define TQUEUE_SIZE 20


typedef struct Tiny_IPFIX_Fields {
  uint16_t type;
  TinyIPFix_Data_Field_4 field;
  int is_enterprise;
  uint32_t enterprise_number;
};

//queue
Tiny_IPFIX_Fields tiny_fields[TQUEUE_SIZE];
//uint16_t fields_types[TQUEUE_SIZE];
//IPFix_Data_Field_4 tiny_fields[TQUEUE_SIZE];
int tqueue_count = 0;
int tenterprise_count = 0;

struct TinyIPFix_Data_Field_4 Create_TinyIPFIX_Data_Field_4_number(uint32_t _datafield)
{
  TinyIPFix_Data_Field_4 ipfixdata_data_f;
  memset(&ipfixdata_data_f, 0, sizeof (TinyIPFix_Data_Field_4));
  ipfixdata_data_f.DataField.number = _datafield;

  tiny_swap_bytes_4(&ipfixdata_data_f.DataField);

  return ipfixdata_data_f;
}

struct TinyIPFix_Data_Field_4 Create_TinyIPFIX_Data_Field_4_bytes(unsigned char byte0, unsigned char byte1, unsigned char byte2, unsigned char byte3)
{
  TinyIPFix_Data_Field_4 ipfixdata_data_f;
  memset(&ipfixdata_data_f, 0, sizeof (TinyIPFix_Data_Field_4));

  ipfixdata_data_f.DataField.b[3] = byte3;
  ipfixdata_data_f.DataField.b[2] = byte2;
  ipfixdata_data_f.DataField.b[1] = byte1;
  ipfixdata_data_f.DataField.b[0] = byte0;

  tiny_swap_bytes_4(&ipfixdata_data_f.DataField);

  return ipfixdata_data_f;
}


void enqueue_TinyIPFix_data(uint16_t type, TinyIPFix_Data_Field_4 _value)
{

  if (tqueue_count < (TQUEUE_SIZE - 1))
  {
    tiny_fields[tqueue_count].type = type;
    tiny_fields[tqueue_count].field = _value;
    tiny_fields[tqueue_count].is_enterprise = 0;
    tiny_fields[tqueue_count].enterprise_number = 0;
    tqueue_count++;
  }

}

void enqueue_TinyIPFix_data_enterprise(uint16_t type, TinyIPFix_Data_Field_4 _value, uint32_t enterprise_number)
{

  if (tqueue_count < (TQUEUE_SIZE - 1))
  {
    tiny_fields[tqueue_count].type = type;
    tiny_fields[tqueue_count].field = _value;
    tiny_fields[tqueue_count].is_enterprise = 1;
    tiny_fields[tqueue_count].enterprise_number = enterprise_number;
    tqueue_count++;
    tenterprise_count++;
  }

}

void tclear_queue()
{
  tqueue_count = 0;
  tenterprise_count = 0;
}


void send_TinyIPFix_packet_template(WiFiUDP *udp, const char *udpAddress, const int udpPort, uint8_t template_id)
{

  int countlen = tqueue_count;

  TinyIPFix_Header tinyheader = Get_TinyIPFix_Header(tiny_header_length_calc(tqueue_count, tenterprise_count, 1), 1);

  TinyIPFix_Set_Header settemplate = Get_TinyIPFix_Set_Header(template_id, tqueue_count);


  udp->beginPacket(udpAddress, udpPort);
  udp->write((byte*)&tinyheader, sizeof (TinyIPFix_Header));
  udp->write((byte*)&settemplate, sizeof (TinyIPFix_Set_Header));

  TinyIPFix_Data_Template_Description tinyipfixdata_temp_desc;

  TinyIPFix_Data_Template_Description_Enterprise tinyipfixdata_temp_desc_ent;


  for (int i = 0; i < countlen; i++)
  {

    //uint16_t _data_type, uint16_t _data_length
    if (tiny_fields[i].is_enterprise == 1)
    {
      tinyipfixdata_temp_desc_ent = Get_TinyIPFIX_Template_Description_Enterprise(tiny_fields[i].type, 4, tiny_fields[i].enterprise_number, 1);
      udp->write((byte*)&tinyipfixdata_temp_desc_ent, sizeof (TinyIPFix_Data_Template_Description_Enterprise));
    } else {
      tinyipfixdata_temp_desc = Get_TinyIPFIX_Template_Description(tiny_fields[i].type, 4);
      udp->write((byte*)&tinyipfixdata_temp_desc, sizeof (TinyIPFix_Data_Template_Description));
    }

  }

  udp->endPacket();

  tclear_queue();

}


void send_TinyIPFix_packet_data(WiFiUDP *udp, const char *udpAddress, const int udpPort, uint8_t template_id)
{

  int countlen = tqueue_count;

  TinyIPFix_Header tinyheader = Get_TinyIPFix_Header(tiny_header_length_calc(tqueue_count, tenterprise_count, 0), 2);

  TinyIPFix_Set_Header setdata = Get_TinyIPFix_Set_Header(template_id, tiny_data_length_calc(tqueue_count));

  udp->beginPacket(udpAddress, udpPort);
  udp->write((byte*)&tinyheader, sizeof (TinyIPFix_Header));


  udp->write((byte*)&setdata, sizeof (TinyIPFix_Set_Header));


  for (int i = 0; i < countlen; i++)
  {
    udp->write((byte*)&tiny_fields[i].field, sizeof (TinyIPFix_Data_Field_4));
  }


  udp->endPacket();

  tiny_increment_seq_num();
  tclear_queue();

}
