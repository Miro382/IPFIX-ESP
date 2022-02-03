/*
    This sketch sends random data over UDP on a ESP32 device

*/
#include "IPFixLib.h"
#include "TinyIPFixLib.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// WiFi network name and password:
const char * networkName = "Jahodova zmrzlina";
const char * networkPswd = "AnanaS.123.+";

#define NTP_OFFSET   60 * 60      // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "europe.pool.ntp.org"

//IP address to send UDP data to:
// either use the ip address of the server or
// a network broadcast address
const char * udpAddress = "192.168.0.151";//"192.168.0.106";
const int udpPort = 4739;//51049;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);


int firsttime = 0;

void setup() {
  // Initilize hardware serial:
  Serial.begin(115200);

  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);

  timeClient.begin();
}

void loop() {
  //only send data when connected
  if (connected) {
    timeClient.update();

    unsigned long epochTime =  timeClient.getEpochTime();



    /*
        IPFix_Data_Field_4 f1 = Create_IPFIX_Data_Field_4_bytes(50,10,168,192);
        IPFix_Data_Field_4 f2 = Create_IPFIX_Data_Field_4_bytes(80,10,168,192);
        IPFix_Data_Field_4 f3 = Create_IPFIX_Data_Field_4_number(12876);
        enqueue_IPFix_data(8, f1);
        enqueue_IPFix_data(12, f2);
        enqueue_IPFix_data(148, f3);


        IPFix_Data_Field_4 f4 = Create_IPFIX_Data_Field_4_number(530);
        IPFix_Data_Field_4 f5 = Create_IPFIX_Data_Field_4_number(380);
        enqueue_IPFix_data_enterprise(10, f4, 5);
        enqueue_IPFix_data_enterprise(9, f5, 12);

        //WiFiUDP udp, char *udpAddress, const int udpPort, uint32_t epochtime, uint32_t observ_dom_id, uint16_t template_id
        //send_IPFix_packet(&udp, udpAddress, udpPort, epochTime, 0, 260);


        if(firsttime == 0)
        {
          send_IPFix_packet_only_template(&udp, udpAddress, udpPort, epochTime, 0, 257);
          firsttime = 1;
        }else{
          send_IPFix_packet_only_data(&udp, udpAddress, udpPort, epochTime, 0, 257);
        }
    */




    TinyIPFix_Data_Field_4 f1 = Create_TinyIPFIX_Data_Field_4_bytes(50, 10, 168, 192);
    TinyIPFix_Data_Field_4 f2 = Create_TinyIPFIX_Data_Field_4_bytes(80, 10, 168, 192);
    TinyIPFix_Data_Field_4 f3 = Create_TinyIPFIX_Data_Field_4_number(12876);
    enqueue_TinyIPFix_data(8, f1);
    enqueue_TinyIPFix_data(12, f2);
    enqueue_TinyIPFix_data(148, f3);


    TinyIPFix_Data_Field_4 f4 = Create_TinyIPFIX_Data_Field_4_number(530);
    TinyIPFix_Data_Field_4 f5 = Create_TinyIPFIX_Data_Field_4_number(380);
    enqueue_TinyIPFix_data_enterprise(10, f4, 5);
    enqueue_TinyIPFix_data_enterprise(9, f5, 12);

    if (firsttime == 0)
    {
      send_TinyIPFix_packet_template(&udp, udpAddress, udpPort, 130);
      firsttime = 1;
    } else {
      send_TinyIPFix_packet_data(&udp, udpAddress, udpPort, 130);
    }


    /*

            //uint16_t _length, uint32_t _export_time, uint32_t _seq_number, uint32_t _observ_dom_id
            IPFix_Data_Header ipfixdata_header = Get_IPFIX_Header(header_length_calc(1), epochTime, 0, 0);


            //uint16_t _set_id, uint16_t _length, uint16_t _template_id, uint16_t _fieldcount
            IPFix_Data_Template ipfixdata_temp = Get_IPFIX_Template(2, template_length_calc(1), 256, 1);


            //uint16_t _data_type, uint16_t _data_length
            IPFix_Data_Template_Description ipfixdata_temp_desc = Get_IPFIX_Template_Description(8, 4);

            //uint16_t _set_id, uint16_t _length
            IPFix_Data ipfixdata = Get_IPFIX_Data(256, data_length_calc(1));

            //uint32_t _datafield
            IPFix_Data_Field_4 ipfixdatafield1 = Get_IPFIX_Data_Field_4(109);
    */

    /*
        int countlen = 3;

        //uint16_t _length, uint32_t _export_time, uint32_t _seq_number, uint32_t _observ_dom_id
        IPFix_Data_Header ipfixdata_header = Get_IPFIX_Header(header_length_calc(countlen, 1), epochTime, 0);


        //uint16_t _set_id, uint16_t _length, uint16_t _template_id, uint16_t _fieldcount
        IPFix_Data_Template ipfixdata_temp = Get_IPFIX_Template(2, template_length_calc(countlen, 1), 259, 3);


        //uint16_t _data_type, uint16_t _data_length
        IPFix_Data_Template_Description ipfixdata_temp_desc = Get_IPFIX_Template_Description(8, 4);
        IPFix_Data_Template_Description ipfixdata_temp_desc2 = Get_IPFIX_Template_Description(12, 4);
        IPFix_Data_Template_Description_Enterprise ipfixdata_temp_desc3 = Get_IPFIX_Template_Description_Enterprise(3, 4, 3, 1);

        //uint16_t _set_id, uint16_t _length
        IPFix_Data ipfixdata = Get_IPFIX_Data(259, data_length_calc(countlen));

        //uint32_t _datafield
        IPFix_Data_Field_4 ipfixdatafield1 = Get_IPFIX_Data_Field_4(200);

        IPFix_Data_Field_4 ipfixdatafield2 = Get_IPFIX_Data_Field_4(0);

        ipfixdatafield2.DataField.b[3] = 192;
        ipfixdatafield2.DataField.b[2] = 168;
        ipfixdatafield2.DataField.b[1] = 10;
        ipfixdatafield2.DataField.b[0] = 6;


        swap_bytes_4(&ipfixdatafield2.DataField);

        IPFix_Data_Field_4 ipfixdatafield3 = Get_IPFIX_Data_Field_4(233856);


        increment_seq_num();
    */
    /*

        udp.beginPacket(udpAddress, udpPort);
        udp.write((byte*)&ipfixdata_header, sizeof (IPFix_Data_Header));
        udp.write((byte*)&ipfixdata_temp, sizeof (IPFix_Data_Template));
        udp.write((byte*)&ipfixdata_temp_desc, sizeof (IPFix_Data_Template_Description));
        udp.write((byte*)&ipfixdata, sizeof (IPFix_Data));
        udp.write((byte*)&ipfixdatafield1, sizeof (IPFix_Data_Field_4));
        udp.endPacket();
    */

    /*
        udp.beginPacket(udpAddress, udpPort);
        udp.write((byte*)&ipfixdata_header, sizeof (IPFix_Data_Header));
        udp.write((byte*)&ipfixdata_temp, sizeof (IPFix_Data_Template));
        udp.write((byte*)&ipfixdata_temp_desc, sizeof (IPFix_Data_Template_Description));
        udp.write((byte*)&ipfixdata_temp_desc2, sizeof (IPFix_Data_Template_Description));
        udp.write((byte*)&ipfixdata_temp_desc3, sizeof (IPFix_Data_Template_Description_Enterprise));
        udp.write((byte*)&ipfixdata, sizeof (IPFix_Data));
        udp.write((byte*)&ipfixdatafield1, sizeof (IPFix_Data_Field_4));
        udp.write((byte*)&ipfixdatafield2, sizeof (IPFix_Data_Field_4));
        udp.write((byte*)&ipfixdatafield3, sizeof (IPFix_Data_Field_4));
        udp.endPacket();
    */



    /*
        ipfixdata_header.Length.number = (16 + 8);
        ipfixdata_header.Sequence_Number.number = 1;

        swap_bytes_2(&ipfixdata_header.Length);
        swap_bytes_4(&ipfixdata_header.Sequence_Number);

        udp.beginPacket(udpAddress, udpPort);
        //udp.printf("Seconds since boot: %lu", millis()/1000);
        udp.write((byte*)&ipfixdata_header, sizeof (IPFix_Data_Header));
        udp.write((byte*)&ipfixdata, sizeof (IPFix_Data));
        udp.write((byte*)&ipfixdatafield1, sizeof (IPFix_Data_Field_4));
        udp.endPacket();
    */

    /*
        ipfixdata_header.Length.number = (16 + 12);
        ipfixdata_header.Sequence_Number.number = 0;

        swap_bytes_2(&ipfixdata_header.Length);
        swap_bytes_4(&ipfixdata_header.Sequence_Number);
    */

    /*
        udp.beginPacket(udpAddress, udpPort);
        udp.write((byte*)&ipfixdata_header, sizeof (IPFix_Data_Header));
        udp.write((byte*)&ipfixdata, sizeof (IPFix_Data));
        udp.write((byte*)&ipfixdatafield1, sizeof (IPFix_Data_Field_4));
        udp.write((byte*)&ipfixdatafield2, sizeof (IPFix_Data_Field_4));
        udp.write((byte*)&ipfixdatafield3, sizeof (IPFix_Data_Field_4));
        udp.endPacket();
    */
    Serial.println("Sended!");
  }

  delay(10000);
}

void connectToWiFi(const char * ssid, const char * pwd) {
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);

  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      //When connected set
      Serial.print("WiFi connected! IP address: ");
      Serial.println(WiFi.localIP());
      //initializes the UDP state
      //This initializes the transfer buffer
      udp.begin(WiFi.localIP(), udpPort);
      connected = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      connected = false;
      break;
    default: break;
  }
}
