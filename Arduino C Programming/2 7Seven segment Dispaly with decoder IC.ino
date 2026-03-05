int a1=2;  
int b1=3;  
int c1=4;  
int d1=5;  

int a2=6;  
int b2=7;  
int c2=8;  
int d2=9;  

void setup(){
  pinMode(a1,OUTPUT);
  pinMode(b1,OUTPUT);
  pinMode(c1,OUTPUT);
  pinMode(d1,OUTPUT);
  
  pinMode(a2,OUTPUT);
  pinMode(b2,OUTPUT);
  pinMode(c2,OUTPUT);
  pinMode(d2,OUTPUT);
}

void loop(){
  
  for(int i =0; i<=30; i++){
    int tens = i/10;     
    int units = i%10;      
    
    digitalWrite(a1,(units>>0) & 1);
    digitalWrite(b1,(units>>1) & 1);
    digitalWrite(c1,(units>>2) & 1);
    digitalWrite(d1,(units>>3) & 1);
    
    digitalWrite(a2,(tens>>0) & 1);
    digitalWrite(b2,(tens>>1) & 1);
    digitalWrite(c2,(tens>>2) & 1);
    digitalWrite(d2,(tens>>3) & 1);
    
    delay(1000);  
  }
}