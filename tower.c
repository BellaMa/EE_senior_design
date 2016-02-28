// code for arduino mega 


//#include <SoftwareSerial.h>
//SoftwareSerial xbee(0,1);

int a_dis, b_dis, c_dis,a_spe, b_spe, c_spe;

const int A_far_trig = 22;
const int A_far_echo = 23;
const int A_close_trig = 24;
const int A_close_echo = 25;
const int B_far_trig = 26;
const int B_far_echo = 27;
const int B_close_trig = 28;
const int B_close_echo = 29;
const int C_far_trig = 30;
const int C_far_echo = 31;
const int C_close_trig = 32;
const int C_close_echo = 33;


//int dis_holder;
int dis_holder_a,dis_holder_b,dis_holder_c;
int a_flag = 0;
int b_flag = 0;
int c_flag = 0;
int speed_flag = 0;
int new_speed;
int new_pwm = 0;
int again = 0;
int over = 0;

//int pwm[11] = {0, 80, 100, 120, 140, 160, 180, 200, 220, 240, 255};
//int table[11] = { 0, 10, 48, 55, 70, 97, 102, 127, 135, 160, 170};
int pwm[18] = { 0, 80, 80, 80, 80, 120, 120, 140, 140, 160, 180, 180, 200, 220, 220, 220, 240, 255};
int pos;
int speed_for_car;

int a_spe_flag = 0;
int b_spe_flag = 0;
int c_spe_flag = 0;

int a_dis_flag = 0;
int b_dis_flag = 0;
int c_dis_flag = 0;

int push_start = 7;
int push_stop = 8;

//Codes for communication & commands.
int start = 1;
int halt = 0;
int ask_a_spe = 2;
int ask_b_spe = 3;
int ask_c_spe = 4;
int a_delt_spe = 5;
int b_delt_spe = 6;
int c_delt_spe = 7;


void setup()
{
  //a_dis = 0;
  //xbee.begin(9600);
  pinMode(push_start, INPUT);
  pinMode(push_stop, INPUT);
  pinMode(A_far_trig, OUTPUT);
  pinMode(A_far_echo, INPUT);
  pinMode(A_close_trig, OUTPUT);
  pinMode(A_close_echo, INPUT);
  pinMode(B_far_trig, OUTPUT);
  pinMode(B_far_echo, INPUT);
  pinMode(B_close_trig, OUTPUT);
  pinMode(B_close_echo, INPUT);
  pinMode(C_far_trig, OUTPUT);
  pinMode(C_far_echo, INPUT);
  pinMode(C_close_trig, OUTPUT);
  pinMode(C_close_echo, INPUT);
  Serial.begin(9600);
  //Serial1.begin(4800);
  pinMode(10,INPUT);
  pinMode(12,INPUT);
}


void loop()
{
  //Serial.println();
  //Serial.println("************************");


  // Basic start/stop commands.
  if(digitalRead(push_stop) == HIGH)
  {
    //xbee.write(halt);
    Serial.write(halt);
    //Serial.flush();
    //a_spe = 0;
  }
  else if(digitalRead(push_start) == HIGH)
  {
    //xbee.write(start);
    //Serial.print("trip_1");
    Serial.write(start);
    //Serial.flush();
    delay(1000);
    a_dis_flag = 1;
    
  }



  if(a_spe_flag == 1)
  {
    //xbee.write(1000);
    //Serial.print("trip_2");
    if(Serial.peek() < 1)
    {
      //Serial.print("trip_3");
      Serial.write(ask_a_spe);
      
    }
    else if (Serial.peek() > 0)
    {

      a_spe = Serial.read();
      
        while(Serial.available())
  {
    Serial.read();
  }

      Serial.print("Car A speed is: ");
      Serial.print(a_spe);
      Serial.print('\n');


      Serial.print("Car A distance is: ");
      Serial.println(a_dis);
      Serial.flush();

      a_dis_flag = 0;
      a_spe_flag = 0;
      b_dis_flag = 1;
    }
  }



  if(b_spe_flag == 1)
  {
    if(Serial.peek() < 1)
    {
      Serial.write(ask_b_spe);
      
    }
    else if (Serial.peek() > 0)
    {

      b_spe = Serial.read();

  while(Serial.available())
  {
    Serial.read();
  }


      Serial.print("Car B speed is: ");
      Serial.print(b_spe);
      Serial.print('\n');

      Serial.print("Car B distance is: ");
      Serial.println(b_dis);
      Serial.flush();

      b_dis_flag = 0;
      b_spe_flag = 0;
      c_dis_flag = 1;
    }
  }


if(c_spe_flag == 1)
{
  if(Serial.peek() < 1)
  {
    Serial.write(ask_c_spe);
    
  }
  else if(Serial.peek() > 0)
  {
    c_spe = Serial.read();
    
      while(Serial.available())
  {
    Serial.read();
  }
    
    
    
    Serial.print("Car C speed is: ");
    Serial.print(c_spe);
    Serial.print('\n');
    
    Serial.print("Car C distance is: ");
    Serial.println(c_dis);
    Serial.flush();
    c_spe_flag = 0;
    c_dis_flag = 0;
    speed_flag = 1;
  }
}



  while(Serial.available())
  {
    Serial.read();
  }




  //Finding distance to B.
  if (b_dis_flag == 1)
  {
    if (b_flag == 0)
    {
      dis_holder_b = dis_1(B_far_trig, B_far_echo);

      if (30 < dis_holder_b < 200)  // changed from 5 to 30
      {
        b_dis = dis_holder_b + 200;
        b_spe_flag = 1;  
      }

      if( dis_holder_b < 30)
      {
        b_flag = 1; 
      }
    }
    else if(b_flag == 1)
    {
      dis_holder_b = dis_1(B_close_trig,B_close_echo);

      if (5 < dis_holder_b < 200)
      {
        b_dis = dis_holder_b;
        b_spe_flag = 1;
      }
      if( dis_holder_b < 5)
      {
        b_flag = 2;
        b_spe_flag = 0;
      }
    }
  }



  //Finding distance to A.
  if (a_dis_flag == 1)
  {
    if (a_flag == 0)
    {
      dis_holder_a = dis_1(A_far_trig, A_far_echo);

      if (30 < dis_holder_a < 200)   // changed from 5 to 10
      {
        a_dis = dis_holder_a + 200;
        a_spe_flag = 1;
      }

      if( dis_holder_a < 35)
      {
        a_flag = 1;
      }
    }
    else if(a_flag == 1)
    {
      dis_holder_a = dis_1(A_close_trig,A_close_echo);

      if (5 < dis_holder_a < 200)
      {
        a_dis = dis_holder_a;
        a_spe_flag = 1;
      }
      if( dis_holder_a < 5)
      {
        a_flag = 2;
        a_spe_flag = 0;
      }
    }
  }
  


    //Finding distance to C.
    if(c_dis_flag == 1)
    {
   if (c_flag == 0)
   {
     dis_holder_c = dis_1(C_far_trig, C_far_echo);
   if (30 < dis_holder_c < 200)   // changed from 5 to 10 
   {
     c_dis = dis_holder_c + 200;
     c_spe_flag = 1;

   }
   
   if( dis_holder_c < 35)
   {
     c_flag = 1;
   }
   }
   else if(c_flag == 1)
   {
   dis_holder_c = dis_1(C_close_trig,C_close_echo);
   
   if (5 < dis_holder_c < 200)
   {
   c_dis = dis_holder_c;
   c_spe_flag = 1;
   }
   if( dis_holder_a < 5)
   {
     c_flag = 2;
     c_spe_flag = 0;
   }
   }
}


  
  //collision calc
  if(speed_flag == 1)
  {
   new_speed = collision_calc(a_spe,b_spe,c_spe,a_dis,b_dis,c_dis);
   new_pwm = finding_pwm_from_hell(new_speed);
   if(new_speed > 0)
   {
   if (new_speed > 3000)
   {
     Serial.println("Car C, ");
     Serial.write(c_delt_spe);
     //Serial.flush();
     new_speed = new_speed - 3000;
   }
   else if(new_speed > 2000)
   {
     Serial.println("Car B, ");
     //new_speed = new_speed - 2000;
     Serial.write(b_delt_spe);
   }
   else if(new_speed > 1000)
   {
     Serial.println("Car A, ");
     //new_speed = new_speed - 1000;
     Serial.write(a_delt_spe);
   }
   //Serial.print("speed change to: ");
   if(pos > 17)
   {
     new_pwm = 255;
   }
   
   Serial.write(new_pwm);
   
   Serial.print("speed to: ");
   Serial.println(new_speed);
   Serial.print("pwm is: ");
   Serial.println(new_pwm);
   Serial.print("pos: ");
   Serial.println(pos);
   
   delay(50);
   speed_flag = 0;
   new_pwm = 0;
   again = 1;
   }
   else if(new_speed == 0)
   {
     Serial.println("no new speed");
     //again = 1;
     speed_flag = 0;
     new_pwm = 0;
  }
  }

if(again == 1)
{
  if (a_flag == 2)
  {
    if(b_flag == 2)
    {
      if(c_flag == 2)
      {
        over = 1;
      }
    }
  }
  else
  {
    a_dis_flag = 1;
  }
  again = 0;
}

if(over == 1)
{
  Serial.write(halt);
  Serial.println("over");
}


}


///////////////////FUNCTIONS////////////////////



//distance measurement function
int dis_1(int trig, int echo)
{
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(20);
  digitalWrite(trig,LOW);
  int pulse = pulseIn(echo,HIGH);
  int distance = pulse/58;
  return distance;
}

//finds new pwm for related speed.
int finding_pwm_from_hell(int new_speed)
{
  
  speed_for_car = new_speed / 1000;
  
  pos = (new_speed / 10) - (speed_for_car * 100);    //here to find the pos at look-up table
  
  new_pwm = pwm[pos];
  
  return new_pwm;
}





//collision calculation & new speed determination function
int collision_calc(int a_spe, int b_spe, int c_spe, int a_dis, int b_dis,int c_dis)
{
  double ti_a, ti_b, ti_c, col_1, col_2, c_s, c_d, a_s, a_d, b_s, b_d;
  c_s = c_spe;
  c_d = c_dis;
  a_s = a_spe;
  a_d = a_dis;
  b_s = b_spe;
  b_d = b_dis;
  ti_a = a_d/a_s;
  ti_b = b_d/b_s;
  ti_c = c_d/c_s;
  col_1 = ti_a - ti_b;
  col_2 = ti_c - ti_b;
  
  col_1 = abs(col_1);
  col_2 = abs(col_2);
  
  if (col_1 < 2)
  {
    col_2 = ti_c - (ti_b + 1);
    col_2 = abs(col_2);
    if (col_2 < 2)
    {
      new_speed = ((a_dis/(ti_a - 1)) + 1000);
    }
    else
    {
      new_speed = ((b_dis/(ti_b+ 1)) + 2000);
    }
  }
  
  else if( col_2 < 2)
   {
   col_1 = ti_a - (ti_b + 1);
   if ( col_1 < 2)
   {
   new_speed = ((c_dis/(ti_c - 1)) + 3000);
   }
   else
   {
   new_speed = ((b_dis/(ti_b+1))+2000);
   }
   }
   
  else
  {
    new_speed = halt;
  }
  
  return new_speed;
}











