
String sV="uh160529due_esp01_12_70a";
String s1="AT+CIPSTART=\"TCP\",\"192.168.70.100\",3696";
long msd;
long ID99;
long ms0=-100000;
int ooLi;

String ooIn="";

int ooL20=60;
int Ln0=0;

char Ad='a';
byte Gno=1;

char cmd='c';

long Ln=0;
String cmd_echo;
int setups=0;
int slen;

HardwareSerial & mySerial = Serial3;

void serialEvent()
{
  delay(100);
  if (Serial.available())
  if (char cc=(Serial.read())=='!')
  ooIn="!"+Serial.readString();
  Serial.println("serialEvent");loop11();
  Serial.println(ooIn);
}


String wifi_send0="";
String wifi_send0s="";
byte wifi_send_f=0;

void wifi_send(int inByte)
{
  if (inByte=='L')     wifi_send0=""; //160519
  if (inByte==13){
    if ((wifi_send0[0]=='L') && (wifi_send0[1]==','))
    {
      ID99++;
      wifi_send0 += ":"+String(millis()/1000%28800); //160619 
      wifi_send0 += char(inByte); //160619

      if (wifi_send0.indexOf("RSSI")!=-1){
        wifi_send0.replace("RSSI",String(msd/1000)+":RI");
        String ws="";
        ws=wifi_send0.substring(0,5)+"9";
        if (wifi_send0[6]==',') ws+="0";
        ws+=wifi_send0.substring(5);
        wifi_send0=ws;
      }
      wifi_send0s+=wifi_send0;
      wifi_send0s+="\n";
      wifi_send_f=1;
    }
    else wifi_send0 += char(inByte); //160619
    wifi_send0="";
  }
  else 
  if (inByte!=10)   wifi_send0 += char(inByte); //160519
  
  if (inByte=='?'){
    long ms=millis();
    String oo;
    
    if (Serial.available()) ooIn=Serial.readString();
    ms=ms-ms0;
    msd=ms;
    ms0=millis();

    Serial.print("ms: ");
    Serial.println(ms);

    if (ms<8000) ooLi+=1;
    if (ms>(10000*ooLi+8000)) ooLi=0;//160529
    if (ms>(8000*ooL20-8000)) ooLi=0;//???

    Serial.print("ooLi: ");
    Serial.println(ooLi);

    long ooL;

    ooL=ooL20+ooLi; //160517
    Serial.println(ooL);
    oo=String(ooL);
    
    if      (ooL<10) oo="0000000"+oo;
    else if (ooL<100) oo="000000"+oo;
    else if (ooL<1000) oo="00000"+oo;
    else if (ooL<10000) oo="0000"+oo;
    else if (ooL<100000) oo="000"+oo;
    else if (ooL<1000000) oo="00"+oo;
    else if (ooL<10000000) oo="0"+oo;

    oo="!"+oo+"#";

    if (Serial.available()) ooIn=Serial.readString();
    if (ooIn[0]=='!') oo=ooIn;
    
    Serial.println(oo);
    Serial2.print(oo);    //
    Serial.println(oo);
    Serial.println(ms);

    if (wifi_send_f==1){
      wifi_send_f=0;
      Serial.println("wifi_sends");    Serial.println(wifi_send0s);      
      
      String sOut=wifi_send0s;
      data_send(s1,sOut);  
      wifi_send0s="";
      F_ln0();
    } 
  }
}

void setup(){
  Serial.begin(115200); 
  Serial.println (sV);  Serial.print ("ch = [");  Serial.write (Ad);  Serial.print ("]");  Serial.println (byte(Ad));
  delay(3000);
  
  Serial3.begin(115200);
  mySerial.begin(115200);
  Serial2.begin(115200);
  cmd_send("AT+RST",2000);

  while (mySerial.available())
    Serial.write(mySerial.read());

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("setup1 in");  
  setup1();
  Serial.println("setup1 out");  
}

void F_ln0()
{
  int x,y;
  int xy500=(Ln0/2)%500;
  x=xy500%20+1;
  y=xy500/20+1;

  String s2;
  if (Ln0%2==0){
    s2="G,"+String(x)+","+String(y)+",0,0,0,"+"255,0,0,"+"255,255,255,"+"0,10";
    data_send(s1,s2);  
  }
  else{
    s2="G,"+String(x)+","+String(y)+",0,0,255,"+"0,255,0,"+"0,0,0,"+"0,10";
    data_send(s1,s2);  
  }
  Serial.println(s2);
  Ln0++;
}

void F_ln0_c()
{
  int x,y;

  for (y=1; y<26; y++){
    String s2="";
    for (x=1; x<21; x++){
      String s2="";
      s2+="G,"+String(x)+","+String(y)+",0,0,255,"+"0,255,0,"+"0,0,0,"+"0,10";//x5=167
    }
    data_send(s1,s2);  
    Serial.println(s2);
  }
}

int Sno;
int pCn; 
int inByte;

String sOut="";
String sOutA[5]={};
byte Hset=0;

void loop()
{  

  loop11();

  if (cmd    !='s'  ) goto SKIP_LOOP;
  if (inByte != '_' ) goto SKIP_LOOP;
  if (Hset    < 8   ) goto SKIP_LOOP;
 
  Serial.print("Hset -> ");
  Serial.println(Hset);
  while (!(Serial2.available())) ;
  
  Sno = Serial2.read();

  Serial.print("L,");
  Serial.print(Gno);
  Serial.print(",");
  Serial.print(Sno-48);
  Serial.print(",");

  sOut+="L,"+String(Gno)+","+String(Sno-48)+",";
  pCn=0;  
  
  for (int i=0; i<6; ){
    while (!(Serial2.available())){ }
    inByte = Serial2.read();
    if (inByte==','){
      pCn++;
      if (pCn>2){
        pCn=0;i++;
        while (inByte!=13){
          while (!(Serial2.available())) {}
          inByte = Serial2.read();
          if (inByte!=13) {    Serial.write(inByte);    sOut+=char(inByte);}
        }//<>13
        Serial.write(';'); 
        sOut+=":";
      }
    }  
  }//for 6*3

  Serial.print(millis());    sOut+=String(millis());
  Serial.print("-");    sOut+="-";
  Ln++;
  Serial.println(Ln);    sOut+=String(Ln);

  sOut+=String("\r\n");

  if ((Sno-48)==4)
  {
    String s1;
    String s2;
    int j=0;

    setups=0;
    s2="";
    Serial.println(sOut);
    data_send(s1,sOut);  

    F_ln0();

    sOut="";
    Hset=0;
  }
SKIP_LOOP:
  pCn=0;  
}

void loop11(){ //160519

 if (Serial2.available()) {
    inByte = Serial2.read();

  Serial.write(inByte);  
  wifi_send(inByte);
  }
}


void loop1() {
  // read from port 1, send to port 0:
  if (Serial3.available()) {
    int inByte = Serial3.read();
    Serial.write(inByte);
  }

  // read from port 0, send to port 1:
  if (Serial.available()) {
    int inByte = Serial.read();
    Serial3.write(inByte);
    Serial.write(inByte);//160513
  }
}

void cmd_send(String cmd_s, int wait)
{
  go_s1:
  mySerial.println(cmd_s);

  wait=wait/2;
  
  while (!(mySerial.available())){delay(100);Serial.print(".");}
  
  for (int i=0; i<50; i++){loop11(); delay(10);} //160519

  cmd_echo = "";
  while (mySerial.available())
  {
    cmd_echo += char(mySerial.read());
    loop11();//160521
  }
  Serial.print("[");
  Serial.print(cmd_echo);
  Serial.print("]");

  if (cmd_echo.indexOf("busy inet")!=-1)
  {
    Serial.println("busy inet delay 1000");
    for (int i=0; i<100; i++){loop11(); delay(10);} //160519
    goto go_s1;
  }
  
  if (cmd_echo.indexOf("link is not")!=-1) setup1();
}

void data_send(String s1,String sOut)
{
  slen = 2+sOut.length();
  cmd_send("AT+CIPSEND="+String(slen),2000);      // 길 이 ---
  cmd_send(String(sOut),1000);
}

void setup1()
{
  cmd_send("AT+CIFSR",100);  
  Serial.println(cmd_echo);
  Serial.println(cmd_echo.indexOf("0.0.0.0"));
  {

    cmd_send("AT+RST",2000);
    delay (2500);

    cmd_send("AT+CWMODE=1",100);

    Serial.println("mySerial Start!");
    while (mySerial.available())
    
    Serial.write(mySerial.read());

    cmd_send("AT+CIFSR",100);  Serial.println(cmd_echo);

  int ck=0;
  for (int i=0; i<5; i++)
  {
    Serial.println("iptime-70a");if (ck==0) ck=connectWiFi("iptime-70a", "1055153130");
    //Serial.println("iptime-10a");if (ck==0) ck=connectWiFi("iptime-10a", "1055153130");
    //Serial.println("iptime-azz");if (ck==0) ck=connectWiFi("iptime-azz", "1055153130");
    if (ck==1) i=5;
  }//for (int i=0; i<5; i++)
  Serial.println("mySerial End");
  
  }

  cmd_send("AT+CIFSR",100);  Serial.println(cmd_echo);
  cmd_send(s1,3000);   // 2015-03-12
  if (cmd_echo.indexOf("Linked")==-1)
  for (int i=0; i<5; i++){
    Serial.print(i);
    if (i==4) setups++;

    delay (1000);  
    cmd_send(s1,3000);   // 2015-03-12
    if (cmd_echo.indexOf("Linked")>-1) i=10;
    if (cmd_echo.indexOf("ALREAY")>-1) i=10;
  }
  Serial.print("Server Connect: ");
  Serial.print(setups);
}

boolean connectWiFi(String NetworkSSID,String NetworkPASS)
{
  cmd_send("AT+CWMODE=1",1000); 
  
  while (cmd_echo.indexOf("busy")>-1) 
    cmd_send("AT+CWMODE=1",2000);

  String cmd="AT+CWJAP=\"";
  cmd += NetworkSSID;
  cmd += "\",\"";
  cmd += NetworkPASS;
  cmd+="\"";
  
  Serial.println(cmd);
  mySerial.println(cmd);
  delay(5000);

//esp-01
//    if(mySerial.find("OK"))
//esp-12E    
  if(mySerial.find("CONNECTED")){
    Serial.println("OK, Connected to WiFi.");
    return true;
  } 
  else{
    Serial.println("Can not connect to the WiFi.");
    return false;
  }
  cmd_send("AT+CIFSR",100);  
  Serial.println(cmd_echo);
}

