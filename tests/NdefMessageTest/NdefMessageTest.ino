#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <NdefMessage.h>
#include <NdefRecord.h>
#include <ArduinoUnit.h>

// Custom Assertion
void assertNoLeak(void (*callback)())
{
  int start = freeMemory();
  (*callback)();
  int end = freeMemory();
  assertEqual(0, (start - end));
}

void assertBytesEqual(const uint8_t* expected, const uint8_t* actual, int size) {
  for (int i = 0; i < size; i++) {
    // Serial.print("> ");Serial.print(expected[i]);Serial.print(" ");Serial.println(actual[i]);
    assertEqual(expected[i], actual[i]);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("\n");
  Serial.println(F("========="));
  Serial.println(freeMemory());
  Serial.println(F("========="));
}

test(memoryKludgeEnd)
{
  // TODO ensure the output matches start
  Serial.println(F("========="));
  Serial.print("End ");Serial.println(freeMemory());
  Serial.println(F("========="));
}

test(messageDelete)
{
  int start = freeMemory();

  NdefMessage* m1 = new NdefMessage();
  m1->addTextRecord("Foo");
  delete m1;

  int end = freeMemory();
//  Serial.print("Start ");Serial.println(start);
//  Serial.print("End ");Serial.println(end);
  assertEqual(0, (start-end));
}


test(assign)
{
  int start = freeMemory();
  
  if (true) // bogus block so automatic storage duration objects are deleted
  {
    NdefMessage* m1 = new NdefMessage();
    m1->addTextRecord("We the People of the United States, in Order to form a more perfect Union...");
    
    NdefMessage* m2 = new NdefMessage();
    
    *m2 = *m1;
    
    NdefRecord r1 = m1->getRecord(0);
    NdefRecord r2 = m2->getRecord(0);
    
    assertEqual(r1.getTnf(), r2.getTnf());
    assertEqual(r1.getTypeLength(), r2.getTypeLength());
    assertEqual(r1.getPayloadLength(), r2.getPayloadLength());
    assertEqual(r1.getIdLength(), r2.getIdLength());
  
    uint8_t* p1 = r1.getPayload();
    uint8_t* p2 = r2.getPayload();  
    int size = r1.getPayloadLength();
    assertBytesEqual(p1, p2, size);
    free(p1);
    free(p2);
  
    delete m2;
    delete m1;
  }
      
  int end = freeMemory();
  assertEqual(0, (start-end));
}

test(assign2)
{
  int start = freeMemory();
  
  if (true) // bogus block so automatic storage duration objects are deleted
  {
    NdefMessage m1 = NdefMessage();
    m1.addTextRecord("We the People of the United States, in Order to form a more perfect Union...");
    
    NdefMessage m2 = NdefMessage();
    
    m2 = m1;
    
    NdefRecord r1 = m1.getRecord(0);
    NdefRecord r2 = m2.getRecord(0);
    
    assertEqual(r1.getTnf(), r2.getTnf());
    assertEqual(r1.getTypeLength(), r2.getTypeLength());
    assertEqual(r1.getPayloadLength(), r2.getPayloadLength());
    assertEqual(r1.getIdLength(), r2.getIdLength());
  
    // TODO check type
  
    uint8_t* p1 = r1.getPayload();
    uint8_t* p2 = r2.getPayload();  
    int size = r1.getPayloadLength();
    assertBytesEqual(p1, p2, size);
    free(p1);
    free(p2);
  }

  int end = freeMemory();
  assertEqual(0, (start-end));
}

test(assign3)
{
  int start = freeMemory();
  
  if (true) // bogus block so automatic storage duration objects are deleted
  {

    NdefMessage* m1 = new NdefMessage();
    m1->addTextRecord("We the People of the United States, in Order to form a more perfect Union...");
    
    NdefMessage* m2 = new NdefMessage();
    
    *m2 = *m1;
    
    delete m1;
    
    NdefRecord r = m2->getRecord(0);
      
    assertEqual(TNF_WELL_KNOWN, r.getTnf());
    assertEqual(1, r.getTypeLength());
    assertEqual(79, r.getPayloadLength());
    assertEqual(0, r.getIdLength());
    
    char payload[] = "We the People of the United States, in Order to form a more perfect Union...";
  
    uint8_t* p = r.getPayload();  
    int size = r.getPayloadLength();
    assertBytesEqual((const uint8_t*)payload, p+3, sizeof(payload));
    free(p);
  
    delete m2;
  }
  
  int end = freeMemory();
  assertEqual(0, (start-end));
}

test(assign4)
{
  int start = freeMemory();
  
  if (true) // bogus block so automatic storage duration objects are deleted
  {

    NdefMessage* m1 = new NdefMessage();
    m1->addTextRecord("We the People of the United States, in Order to form a more perfect Union...");
    
    NdefMessage* m2 = new NdefMessage();
    m2->addTextRecord("Record 1");
    m2->addTextRecord("RECORD 2");
    m2->addTextRecord("Record 3");

    assertEqual(3, m2->getRecordCount());    
    *m2 = *m1;
    assertEqual(1, m2->getRecordCount());
    
//    NdefRecord ghost = m2->getRecord(1);
//    ghost.print();
//    
//    NdefRecord ghost2 = m2->getRecord(3);
//    ghost2.print();

//    
//    delete m1;
//    
//    NdefRecord r = m2->getRecord(0);
//      
//    assertEqual(TNF_WELL_KNOWN, r.getTnf());
//    assertEqual(1, r.getTypeLength());
//    assertEqual(79, r.getPayloadLength());
//    assertEqual(0, r.getIdLength());
//    
//    String s = "We the People of the United States, in Order to form a more perfect Union...";
//    byte payload[s.length() + 1];
//    s.getBytes(payload, sizeof(payload));
//  
//    uint8_t* p = r.getPayload();  
//    int size = r.getPayloadLength();
//    assertBytesEqual(payload, p+3, s.length());
//    free(p);
  
    delete m1;
    delete m2;
  }
  
  int end = freeMemory();
  assertEqual(0, (start-end));
}

// really a record test
test(doublePayload)
{
  int start = freeMemory();
  
  NdefRecord* r = new NdefRecord();
  uint8_t p1[] = { 0x1, 0x2, 0x3, 0x4, 0x5, 0x6 };
  r->setPayload(p1, sizeof(p1));
  r->setPayload(p1, sizeof(p1));
  
  delete r;
  
  int end = freeMemory();
  assertEqual(0, (start-end));
}

  
test(memoryKludgeStart)
{
  Serial.println(F("---------"));
  Serial.print("Start ");Serial.println(freeMemory());
  Serial.println(F("---------"));
}

void loop() {
  Test::run();
}
