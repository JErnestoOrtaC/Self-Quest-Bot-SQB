String lora_band = "915000000";
String lora_networkid = "2";
String lora_address = "1";
String lora_bandwidth = "10, 7, 1, 7";
String lora_RX_address = "1";

void LoraConfig(){
  Serial2.println("AT+BAND=" + lora_band);
  delay(500);
  Serial2.println("AT+ADDRESS=" + lora_address);
  delay(500);
  Serial2.println("AT+NETWORKID=" + lora_networkid);
  delay(500);
  Serial2.println("AT+PARAMETER=" + lora_bandwidth);
  delay(1500);
}

void LoraSend(String msg){
  String cmd = "AT+SEND=0,"+String(msg.length())+","+msg;
  Serial2.println(cmd);
}