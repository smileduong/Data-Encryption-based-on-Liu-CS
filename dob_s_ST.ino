  #include <ESP8266WiFi.h>

// Ten va mat khau cua ESP8266 AP lam server se vao
const char *ssid = "duong";
const char *password = "12345678";

IPAddress server_ip(192, 168, 10, 1);
#define PORT  23
// port 23 la port cua esp8226 lam AP da khoi tao.
WiFiClient client;
int a = 1;
int b = 2;
int c = 1;
int d = 3;
int k=2;
float xs0, gs0, zs0;
float xm=0, gm=0, zm=0,xmdot=0,gmdot=0,zmdot=0;;
float xs, zs, gs,xsdot=0,gsdot=0,zsdot=0;
float sx,sg,sz,sign_x,sign_g,sign_z,sign_dx,sign_dg,sign_dz,pow_sx,pow_sg,pow_sz;
float n01,n11;
float m01,m11;
float lamda_x=1000,lamda_g=1000,lamda_z=1000;
float k1=0.01,k2=2,k3=0.01,k4=0.01; 
float kd1=1500,kd2=5000,kd3=1000,kd4=1000;
float dx=0,dg=0,dz=0,d1=0,d2=0,d3=0;
float d_x=0,d_g=0,d_z=0;
float lx=0,lg=0,lz=0;
float edx=0,edg=0,edz=0;
float integrator_dobx,integrator_dobg,integrator_dobz;
float t=0,u=0,j=0;
float currentTime, previousTime;
float eT=0.0001,eT1=0.0005;
float ex,eg,ez;
float ux=0,ueqx=0,uswx=0;
float ug=0,ueqg=0,uswg=0;
float uz=0,ueqz=0,uswz=0;
float integrator_ex,integrator_eg,integrator_ez;
float integrator_sat_dx,integrator_sat_dg,integrator_sat_dz;
float integrator_d_x,integrator_d_g,integrator_d_z;
float integrator2_d_x,integrator2_d_g,integrator2_d_z;
float integrator3_d_x,integrator3_d_g,integrator3_d_z;
float integrator4_d_x,integrator4_d_g,integrator4_d_z;
float abs_sx,abs_sg,abs_sz,abs_dx,abs_dg,abs_dz;
float encrypted,decrypted;
int decrypted1,h=0;
String str,string1,string2,string3,string4;
String message;
String str1,str2,str3,str4;
String gui,tinnhan;

void setup() {
 uint8_t i = 0;
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  // Kiem tra tình trang ket noi, neu chua ket noi duoc
  // se in chuoi "connecting..." tren man hinh serial terminal.
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...");
    delay(500);
  }

xs0=-1;
gs0=-1;
zs0=1;
}
unsigned long previousMillis = 0;
void loop() {
   if (WiFi.status() == WL_CONNECTED) {
    // Kiem tra neu client(STA) chua duoc ket noi.
    // Kiem tra tiep tuc neu khong duoc ket noi den IP va PORT cua server(AP
    // thi in ra serial terminal chuoi "connection failed".
    while (!client.connected()) {
      if (!client.connect(server_ip, PORT)) {
        Serial.println("connection failed");
        delay(1000);
        return;
      }
    }
    while (client.available()) {
 
       str = client.readStringUntil('\r');
   

        if (str.length() > 1)
  {
   // firsttime=micros();
    int index = str.indexOf(',');
    string1 = str.substring(0, index);
    str1 = str.substring(index+1);
    int index2 = str1.indexOf(',');
     string2 = str1.substring(0, index2);
     str2 = str1.substring(index2+1);
        int index3 = str2.indexOf(',');
     string3 = str2.substring(0, index3);
     str3 = str2.substring(index3+1);
     int index4 = str3.indexOf(',');
     string4 = str3.substring(0, index4);
     str4 = str3.substring(index4+1);
    xm=string1.toFloat()/10000;
    gm=string2.toFloat()/10000;
    zm=string3.toFloat()/10000;
    encrypted=string4.toFloat();
    


eT = 0.0001;//(double)(currentTime - previousTime)/1000;        //compute time elapsed from previous computation
eT1 = 0.0005;
//xs=xs0+eT*(-a*xs0+b*gs0+ux+dx+lx);
//gs=gs0+eT*(-k*xs0*zs0+ug+dg+lg);
//zs=zs0+eT*(c*k*xs0*xs0+k*xs0*gs0-d/k+uz+dz+lz);

xs=xs0+eT*(-a*xs0+b*gs0+ux+dx+lx);
gs=gs0+eT*(-k*xs0*zs0+ug+dg+lg);
zs=zs0+eT*(c*k*xs0*xs0+k*xs0*gs0-d/k+uz+dz+lz);
 xs0=xs;
 gs0=gs;
 zs0=zs; 


    m01 = (5 - xm)/10;
m11 = (5 + xm)/10;
   n01 = (5 - xs0)/10; 
n11 = (5 + xs0)/10;

         ex = xm- xs;                                // determine error
       integrator_ex += ex * eT;                // compute integral
      abs_sx=abs(sx);
        sx = lamda_x*integrator_ex + ex ;
//        pow_sx1 = pow(abs_sx,0.95)  
        sign_x=sx;
        if (sx>0)
        sign_x=1;
        else if (sx<0)
        sign_x=-1;
        else if (sx=0)
        sign_x=0;
        //
          eg = gm- gs; 
          // determine error
        integrator_eg += eg * eT;                // compute integral
        abs_sg=abs(sg);
        sg = lamda_g*integrator_eg + eg ;  
          sign_g=sg;
        if (sg>0)
        sign_g=1;
        else if (sg<0)
        sign_g=-1;
        else if (sg=0)
        sign_g=0;
        //
          ez =zm - zs;                                // determine error
        integrator_ez += ez * eT;                // compute integral
        abs_sz=abs(sz);
        sz = lamda_z*integrator_ez + ez ;  
         sign_z=sz;
        if (sz>0)
        sign_z=1;
        else if (sz<0)
        sign_z=-1;
        else if (sz=0)
        sign_z=0;
        //
                
        xmdot=-a*xm+b*gm;
        gmdot=10*m01*zm-10*m11*zm;
        zmdot=-10*m01*xm - 10*m01*gm + 10*m11*xm + 10*m11*gm - d/k;
        //
       xsdot = -xs0 + 2*gs0;
       gsdot = 10*n01*zs0 - 10*n11*zs0;
       zsdot = -10*n01*xs0 - 10*n01*gs0 + 10*n11*xs0 + 10*n11*gs0 - d/k;
        
        // 
        ueqx=lamda_x*ex+xmdot-xsdot;
        uswx=k1*pow(abs_sx,0.95)*sign_x + eT*(k2*pow(abs_sx,0.9)*sign_x) + eT*eT*(k3*pow(abs_sx,0.85)*sign_x) + eT*eT*eT*(k4*pow(abs_sx,0.8)*sign_x) ;
        ux=ueqx+uswx; 
        //
        ueqg=lamda_g*eg+gmdot-gsdot;
        uswg=k1*pow(abs_sg,0.95)*sign_g + eT*(k2*pow(abs_sg,0.9)*sign_g) + eT*eT*(k3*pow(abs_sg,0.85)*sign_g) + eT*eT*eT*(k4*pow(abs_sg,0.8)*sign_g) ;
        ug=ueqg+uswg; 
        //
        ueqz=lamda_z*ez+zmdot-zsdot;
        uswz=k1*pow(abs_sz,0.95)*sign_z + eT*(k2*pow(abs_sz,0.9)*sign_z) + eT*eT*(k3*pow(abs_sz,0.85)*sign_z) + eT*eT*eT*(k4*pow(abs_sz,0.8)*sign_z) ;
        uz=ueqz+uswz;  
        //  
        d_x =(xsdot-xmdot+ux);
        d_g =(gsdot-gmdot+ug);
        d_z =(zsdot-zmdot+uz);
//        Serial.print(d_x,4);
//Serial.print(",");
//Serial.print(d_g,4);
//Serial.print(",");
//Serial.print(d_z,4);
//Serial.print(",");
        //
//        d_x =(xsdot-xmdot);
//        d_g =(gsdot-gmdot);
//        d_z =(zsdot-zmdot);
     //

   
//        integrator_d_x +=eT*(xsdot-xmdot+ux);
//        integrator_d_g +=eT*(gsdot-gmdot+ug);
//        integrator_d_z +=eT*(zsdot-zmdot+uz);
        //
//         integrator2_d_x +=eT*integrator_d_x;
//       integrator2_d_g +=eT*integrator_d_g;
//    integrator2_d_z +=eT*integrator_d_z;
        //

      //
//        lx=1500*integrator_d_x+1000*integrator2_d_x+d_x;
//        lg=1500*integrator_d_g+1000*integrator2_d_g+d_g;
//        lz=1500*integrator_d_z+1000*integrator2_d_z+d_z;
        
                                   // determine error
               // compute integral
      abs_dx=abs(d_x);  
        sign_dx=d_x;
        if (d_x>0)
        sign_dx=1;
        else if (d_x<0)
        sign_dx=-1;
        else if (d_x=0)
        sign_dx=0;
        //
         abs_dg=abs(d_g);  
        sign_dg=d_g;
        if (d_g>0)
        sign_dg=1;
        else if (d_g<0)
        sign_dg=-1;
        else if (d_g=0)
        sign_dg=0;
        //
         abs_dz=abs(d_z);  
        sign_dz=d_z;
        if (d_z>0)
        sign_dz=1;
        else if (d_z<0)
        sign_dz=-1;
        else if (d_z=0)
        sign_dz=0;
        //
//lx =kd1*pow(abs_dx,0.95)*sign_dx + eT*(kd2*pow(abs_dx,0.9)*sign_dx) + eT*eT*(kd3*pow(abs_dx,0.85)*sign_dx) + eT*eT*eT*(kd4*pow(abs_dx,0.8)*sign_dx) ;
//lg =kd1*pow(abs_dg,0.95)*sign_dg + eT*(kd2*pow(abs_dg,0.9)*sign_dg) + eT*eT*(kd3*pow(abs_dg,0.85)*sign_dg) + eT*eT*eT*(kd4*pow(abs_dg,0.8)*sign_dg) ;    
//lz =kd1*pow(abs_dz,0.95)*sign_dz + eT*(kd2*pow(abs_dz,0.9)*sign_dz) + eT*eT*(kd3*pow(abs_dz,0.85)*sign_dz) + eT*eT*eT*(kd4*pow(abs_dz,0.8)*sign_dz) ;   
//        
//lx =d_x + kd1*pow(abs_dx,0.95)*sign_dx + eT*(kd2*pow(abs_dx,0.9)*sign_dx) + eT*eT*(kd3*pow(abs_dx,0.85)*sign_dx) + eT*eT*eT*(kd4*pow(abs_dx,0.8)*sign_dx) ;
//lg =d_g + kd1*pow(abs_dg,0.95)*sign_dg + eT*(kd2*pow(abs_dg,0.9)*sign_dg) + eT*eT*(kd3*pow(abs_dg,0.85)*sign_dg) + eT*eT*eT*(kd4*pow(abs_dg,0.8)*sign_dg) ;    
//lz =d_z + kd1*pow(abs_dz,0.95)*sign_dz + eT*(kd2*pow(abs_dz,0.9)*sign_dz) + eT*eT*(kd3*pow(abs_dz,0.85)*sign_dz) + eT*eT*eT*(kd4*pow(abs_dz,0.8)*sign_dz) ;     
//
integrator_d_x +=eT*kd1*pow(abs_dx,0.95)*sign_dx;
integrator2_d_x +=eT*eT*(kd2*pow(abs_dx,0.9)*sign_dx);
integrator3_d_x +=eT*eT*eT*(kd3*pow(abs_dx,0.85)*sign_dx);
integrator4_d_x +=eT*eT*eT*eT*(kd4*pow(abs_dx,0.8)*sign_dx);
//
integrator_d_g +=eT*kd1*pow(abs_dg,0.95)*sign_dg;
integrator2_d_g +=eT*eT*(kd2*pow(abs_dg,0.9)*sign_dg);
integrator3_d_g +=eT*eT*eT*(kd3*pow(abs_dg,0.85)*sign_dg);
integrator4_d_g +=eT*eT*eT*eT*(kd4*pow(abs_dg,0.8)*sign_dg);
//
integrator_d_z +=eT*kd1*pow(abs_dz,0.95)*sign_dz;
integrator2_d_z +=eT*eT*(kd2*pow(abs_dz,0.9)*sign_dz);
integrator3_d_z +=eT*eT*eT*(kd3*pow(abs_dg,0.85)*sign_dg);
integrator4_d_z +=eT*eT*eT*eT*(kd4*pow(abs_dz,0.8)*sign_dz);
//
lx =d_x + integrator_d_x +integrator2_d_x + integrator3_d_x + integrator4_d_x ;
lg =d_g + integrator_d_g +integrator2_d_g + integrator3_d_g + integrator4_d_g ;    
lz =d_z + integrator_d_z +integrator2_d_z + integrator3_d_z + integrator4_d_z ;
        
           if (t<31.4)
        {
          //dx=10*sin(0.1*t);
          //dg=3*sin(0.2*t);
          dz=15*sin(1*t);
          t=t+0.01;
        }
       else 
        {
          t=-31.4;
        }     
        if ( u<=300)
        {
         dg=80;
          u=u+1;
        }
       else if (u>300&&u<=600)
        {
         dg=0;
          u=u+1; 
        }   
        else
        {
          u=0;             
        }
  if ( j<=400)
        {
         dx=15;
          j=j+1;
        }
       else if (j>400&&j<=500)
        {
         dx=0;
          j=j+1; 
        }   
        else
        {
          j=0;             
        }
            
 decrypted=encrypted-gs;
//result=t;
//dtostrf(decrypted,5,2,result);
decrypted1=round(decrypted);
//Serial.print(char(decrypted1));
if (micros()>19000000 && micros() < 125000000)
{
Serial.print(encrypted);
Serial.print(",");
Serial.print(decrypted);
Serial.print(",");
Serial.println(micros());
}
if (decrypted1>31){
gui=gui+char(round(encrypted));
tinnhan+=char(round(decrypted1));
}
 if (micros()> 126000000 &  h==0) {
      Serial.println(gui);
      Serial.println(tinnhan);
//      Serial.println(lx);
//      Serial.println(lg);
//      Serial.println(lz);
      h=1;
    }
//Serial.print(",");
//Serial.print(ex,4);
//Serial.print(",");
//Serial.print(eg,4);
//Serial.print(",");
//Serial.print(ez,4);
//Serial.print(",");
//Serial.print(xs,4);
//Serial.print(",");
//Serial.print(gs,4);
//Serial.print(",");
//Serial.print(zs,4);
//Serial.print(",");
//Serial.print(dx,4);
//Serial.print(",");
//Serial.print(-lx,4);
//Serial.print(",");
//Serial.print(dg,4);
//Serial.print(",");
//Serial.print(-lg,4);
//Serial.print(",");
//Serial.print(dz,4);
//Serial.print(",");
//Serial.print(-lz,4);
//Serial.print(",");
//Serial.print(dz);
//Serial.print(",");
//Serial.print(-lz);
//Serial.print(",");
//Serial.println(micros());
//Serial.print(",");
//Serial.println(char(encrypted));
//Serial.print(",");
//Serial.println(char(decrypted1));
//}
//Serial.println(integrator_dobx);
str="";
//eT = (micros()-currentTime)/1000000;
//Serial.println(eT,5);

  }
    }
client.write("Hello\r");
}
}
