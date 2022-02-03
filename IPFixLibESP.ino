#include <stdint.h>
#include <string.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "IPFixLib.h"

#define QUEUE_SIZE 20

typedef struct IPFIX_Fields {
  uint16_t type;
  IPFix_Data_Field_4 field;
  int is_enterprise;
  uint32_t enterprise_number;
};

//queue
IPFIX_Fields fields[QUEUE_SIZE];
//uint16_t fields_types[QUEUE_SIZE];
//IPFix_Data_Field_4 fields[QUEUE_SIZE];
int queue_count = 0;
int enterprise_count = 0;

struct IPFix_Data_Field_4 Create_IPFIX_Data_Field_4_number(uint32_t _datafield)
{
  IPFix_Data_Field_4 ipfixdata_data_f;
  memset(&ipfixdata_data_f, 0, sizeof (IPFix_Data_Field_4));
  ipfixdata_data_f.DataField.number = _datafield;

  swap_bytes_4(&ipfixdata_data_f.DataField);

  return ipfixdata_data_f;
}

struct IPFix_Data_Field_4 Create_IPFIX_Data_Field_4_bytes(unsigned char byte0, unsigned char byte1, unsigned char byte2, unsigned char byte3)
{
  IPFix_Data_Field_4 ipfixdata_data_f;
  memset(&ipfixdata_data_f, 0, sizeof (IPFix_Data_Field_4));

  ipfixdata_data_f.DataField.b[3] = byte3;
  ipfixdata_data_f.DataField.b[2] = byte2;
  ipfixdata_data_f.DataField.b[1] = byte1;
  ipfixdata_data_f.DataField.b[0] = byte0;

  swap_bytes_4(&ipfixdata_data_f.DataField);

  return ipfixdata_data_f;
}


void enqueue_IPFix_data(uint16_t type, IPFix_Data_Field_4 _value)
{

  if (queue_count < (QUEUE_SIZE - 1))
  {
    fields[queue_count].type = type;
    fields[queue_count].field = _value;
    fields[queue_count].is_enterprise = 0;
    fields[queue_count].enterprise_number = 0;
    queue_count++;
  }

}

void enqueue_IPFix_data_enterprise(uint16_t type, IPFix_Data_Field_4 _value, uint32_t enterprise_number)
{

  if (queue_count < (QUEUE_SIZE - 1))
  {
    fields[queue_count].type = type;
    fields[queue_count].field = _value;
    fields[queue_count].is_enterprise = 1;
    fields[queue_count].enterprise_number = enterprise_number;
    queue_count++;
    enterprise_count++;
  }

}

void clear_queue()
{
  queue_count = 0;
  enterprise_count = 0;
}


//WiFiUDP udp, char *udpAddress, const int udpPort, uint32_t epochtime, uint32_t observ_dom_id, uint16_t set_id, uint16_t template_id
void send_IPFix_packet(WiFiUDP *udp, const char *udpAddress, const int udpPort, uint32_t epochtime, uint32_t observ_dom_id, uint16_t template_id)
{

  int countlen = queue_count;

  //uint16_t _length, uint32_t _export_time, uint32_t _seq_number, uint32_t _observ_dom_id
  IPFix_Data_Header ipfixdata_header = Get_IPFIX_Header(header_length_calc(countlen, enterprise_count, 1), epochtime, observ_dom_id);


  //uint16_t _set_id, uint16_t _length, uint16_t _template_id, uint16_t _fieldcount
  IPFix_Data_Template ipfixdata_temp = Get_IPFIX_Template(2, template_length_calc(countlen, enterprise_count), template_id, countlen);

  //uint16_t _set_id, uint16_t _length
  IPFix_Data ipfixdata = Get_IPFIX_Data(template_id, data_length_calc(countlen));

  IPFix_Data_Template_Description ipfixdata_temp_desc;

  IPFix_Data_Template_Description_Enterprise ipfixdata_temp_desc_ent;

  udp->beginPacket(udpAddress, udpPort);
  udp->write((byte*)&ipfixdata_header, sizeof (IPFix_Data_Header));
  udp->write((byte*)&ipfixdata_temp, sizeof (IPFix_Data_Template));


  for (int i = 0; i < countlen; i++)
  {

    //uint16_t _data_type, uint16_t _data_length
    if (fields[i].is_enterprise == 1)
    {
      ipfixdata_temp_desc_ent = Get_IPFIX_Template_Description_Enterprise(fields[i].type, 4, fields[i].enterprise_number, 1);
      udp->write((byte*)&ipfixdata_temp_desc_ent, sizeof (IPFix_Data_Template_Description_Enterprise));
    } else {
      ipfixdata_temp_desc = Get_IPFIX_Template_Description(fields[i].type, 4);
      udp->write((byte*)&ipfixdata_temp_desc, sizeof (IPFix_Data_Template_Description));
    }

  }

  udp->write((byte*)&ipfixdata, sizeof (IPFix_Data));



  for (int i = 0; i < countlen; i++)
  {
    udp->write((byte*)&fields[i].field, sizeof (IPFix_Data_Field_4));
  }


  udp->endPacket();

  increment_seq_num();
  clear_queue();

}



void send_IPFix_packet_only_template(WiFiUDP *udp, const char *udpAddress, const int udpPort, uint32_t epochtime, uint32_t observ_dom_id, uint16_t template_id)
{

  int countlen = queue_count;

  //uint16_t _length, uint32_t _export_time, uint32_t _seq_number, uint32_t _observ_dom_id
  IPFix_Data_Header ipfixdata_header = Get_IPFIX_Header(header_length_calc(countlen, enterprise_count, 2), epochtime, observ_dom_id);


  //uint16_t _set_id, uint16_t _length, uint16_t _template_id, uint16_t _fieldcount
  IPFix_Data_Template ipfixdata_temp = Get_IPFIX_Template(2, template_length_calc(countlen, enterprise_count), template_id, countlen);

  //uint16_t _set_id, uint16_t _length
  IPFix_Data ipfixdata = Get_IPFIX_Data(template_id, data_length_calc(countlen));

  IPFix_Data_Template_Description ipfixdata_temp_desc;

  IPFix_Data_Template_Description_Enterprise ipfixdata_temp_desc_ent;

  udp->beginPacket(udpAddress, udpPort);
  udp->write((byte*)&ipfixdata_header, sizeof (IPFix_Data_Header));
  udp->write((byte*)&ipfixdata_temp, sizeof (IPFix_Data_Template));


  for (int i = 0; i < countlen; i++)
  {

    //uint16_t _data_type, uint16_t _data_length
    if (fields[i].is_enterprise == 1)
    {
      ipfixdata_temp_desc_ent = Get_IPFIX_Template_Description_Enterprise(fields[i].type, 4, fields[i].enterprise_number, 1);
      udp->write((byte*)&ipfixdata_temp_desc_ent, sizeof (IPFix_Data_Template_Description_Enterprise));
    } else {
      ipfixdata_temp_desc = Get_IPFIX_Template_Description(fields[i].type, 4);
      udp->write((byte*)&ipfixdata_temp_desc, sizeof (IPFix_Data_Template_Description));
    }

  }

  udp->endPacket();

  clear_queue();

}


void send_IPFix_packet_only_data(WiFiUDP *udp, const char *udpAddress, const int udpPort, uint32_t epochtime, uint32_t observ_dom_id, uint16_t template_id)
{

  int countlen = queue_count;

  //uint16_t _length, uint32_t _export_time, uint32_t _seq_number, uint32_t _observ_dom_id
  IPFix_Data_Header ipfixdata_header = Get_IPFIX_Header(header_length_calc(countlen, enterprise_count, 0), epochtime, observ_dom_id);

  //uint16_t _set_id, uint16_t _length
  IPFix_Data ipfixdata = Get_IPFIX_Data(template_id, data_length_calc(countlen));

  udp->beginPacket(udpAddress, udpPort);
  udp->write((byte*)&ipfixdata_header, sizeof (IPFix_Data_Header));


  udp->write((byte*)&ipfixdata, sizeof (IPFix_Data));


  for (int i = 0; i < countlen; i++)
  {
    udp->write((byte*)&fields[i].field, sizeof (IPFix_Data_Field_4));
  }


  udp->endPacket();

  increment_seq_num();
  clear_queue();

}
