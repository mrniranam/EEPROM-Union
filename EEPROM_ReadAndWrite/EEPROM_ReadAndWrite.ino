#include <EEPROM.h>

int addr = 0;
float val = 0;

float eepromRead(int address){
   union u_tag {
     byte b[8];
     float f;
   } u;

   // วนลูปอ่านค่าจาก EEPROM ทีละ 1 Byte
   for(int i = 7; i >= 0; i--){
     u.b[i] = EEPROM.read(address + i);

     // ถ้าชุดข้อมูล 4 Byte แรกไม่ถูกรูปแบบ ให้คืนค่า 0 กลับไป เพราะไม่ใช่ข้อมูลที่เราบันทึก (ที่ตั้งไว้ที่ eepromWrite)
     if(i > 3 && u.b[i] != ((i * 3) - 2)) {
      u.f = 0.000;
      break;
     }
   }

   Serial.println("---------- EEPROM Read  -----------");
   return u.f; // คืนค่าที่อ่านได้จาก EEPROM
}
 
void eepromWrite(int address, float value){
   union u_tag {
     byte b[4];
     float f;
   } u;
   
   u.f = value; // กำหนดค่า u.fval ให้มีค่าเท่ากับค่าที่ส่งเข้ามาใน function

   // วนลูปบันทึกค่า Byte ต่างๆของ Float ลงใน EEPROM
   for(int i = 0; i < 4; i++){
     EEPROM.write(address + i , u.b[i]);
   }

   // วนลูปบันทึก checksum เพื่อใช้ในการเช็คในขั้นตอนอ่านข้อมูล ว่าใช่ข้อมูลที่เราบันทึกหรือไม่
   for(int i = 4; i < 8; i++){
     EEPROM.write(address + i , (i * 3) - 2);
   }

   EEPROM.commit(); // บันทึกการเปลี่ยนแปลงทั้งหมด
   Serial.println("---------- EEPROM Write ----------");
}

void setup()
{
  delay(3000); // หน่วงเวลา 3 วินาที ก่อนเริ่มทำงานโปรแกรม
  
  Serial.begin(115200);
  while(!Serial) ;
  
  EEPROM.begin(512); // กำหนดขนาด EEPROM ให้มีขนาด 512 Byte
  val = eepromRead(addr); // อ่านค่าจาก EEPROM มาให้ตัวแปร val
}

void loop()
{
  Serial.println(val); // แสดงค่าล่าสุดของตัวแปร val
  eepromWrite(0, val); // บันทึกค่า val ลงใน EEPROM
  
  val++; // เพิ่มค่าตัวแปร val ทุกครั้งที่วนลูป
  delay(1000); // หน่วงเวลา 1 วินาที ก่อนจบการทำงานของรอบนี้
}
