void configure_display_type()
{
  //  currentStatus.dev1 = configPage1.display_type & B00011111;
    
  //   switch (configPage1.display_type & B00011111)
  //  {
  //    case 0:
          u8g2_0.begin();  
  //    break;

  //    case 1:
        //u8g2_1.begin();
  //    break;
  //  }     
}

void update_display()
{  
       if ((currentStatus.current_gear_Status != currentStatus.old_gear_Status)||(currentStatus.current_gear_Selected != currentStatus.old_gear_Selected))
        { 
         u8g2_0.firstPage();
         do {
             gear_letter(); 
             manual_auto();
             //draw line 1 
             gps_line();
             //draw line 2
             oiltemp_line();
             //draw line 3
             selector_line();
            } while ( u8g2_0.nextPage() );
             currentStatus.old_gear_Status = currentStatus.current_gear_Status;
             currentStatus.old_gear_Selected = currentStatus.current_gear_Selected;
        }   
}

void manual_auto()
{
             u8g2_0.setFont(u8g2_font_helvB14_tr);//u8g2_font_inb16_mr);//u8g2_font_inb57_mn)
           if (configPage1.manual_auto_status == 1){ u8g2_0.drawStr(54,15,"Auto");}
           if (configPage1.manual_auto_status == 0){ u8g2_0.drawStr(50,15,"Manual");}
}

void gear_letter()
{

   u8g2_0.setFont(u8g2_font_logisoso58_tr);
   switch (currentStatus.current_gear_Status)
        {  
         case 1:
              u8g2_0.drawStr(0,63,"1");
         break;

         case 2:
              u8g2_0.drawStr(0,63,"2");
         break;

         case 3:
              u8g2_0.drawStr(0,63,"3");
         break;

         case 4:    
              u8g2_0.drawStr(0,63,"4");
         break;
                  
         case 5:    
              u8g2_0.drawStr(0,63,"5");
         break;

         case 6:
              u8g2_0.drawStr(0,63,"6");
         break;

         case 7:         
              u8g2_0.drawStr(0,63,"7");
         break;

         case 8:   
              u8g2_0.drawStr(0,63,"8");
         break;          
             
         case 10:
              u8g2_0.drawStr(0,63,"N");
         break;  
                  
         case 11:
              u8g2_0.drawStr(0,63,"P");
         break;

         case 20:
              u8g2_0.drawStr(0,63,"D");
         break;
           
         case 30:
              u8g2_0.drawStr(0,63,"R");
         break;
                  
         case 80:
              u8g2_0.drawStr(0,63,"E");
         break; 
        }
    
              
}

void gps_line()
{
           // draw the GPS line data
           u8g2_0.setFont(u8g2_font_8x13B_tf);//helvB10_tr);
           u8g2_0.drawStr(40,30,"GPS ");
           u8g2_0.setCursor(70,30);
           u8g2_0.print("123");           
           u8g2_0.setFont(u8g2_font_7x14B_tf);
           u8g2_0.setCursor(100,30);
           u8g2_0.print("mph");  
}

void oiltemp_line()
{
           // draw the oil temp line data
           u8g2_0.setFont(u8g2_font_8x13B_tf);
           u8g2_0.drawStr(40,45,"OIL ");
           u8g2_0.setCursor(70, 45);
           u8g2_0.print("123");     
           u8g2_0.setFont(u8g2_font_7x14B_tf);      
           u8g2_0.drawGlyph(95,45, 0x00b0);
           u8g2_0.setCursor(100,45);
           u8g2_0.print("C");  
}

void selector_line()
{
          //draw the selcted gear line data
           u8g2_0.setFont(u8g2_font_8x13B_tf);
           u8g2_0.drawStr(40,60,"SELECT ");
           u8g2_0.setFont(u8g2_font_8x13B_tf);
           switch (currentStatus.current_gear_Selected)
                {
                  case 10:
                       u8g2_0.drawStr(100,60,"N");
                  break;

                  case 11:
                       u8g2_0.drawStr(100,60,"P");
                  break;

                  case 1:
                       u8g2_0.drawStr(100,60,"1");
                  break;

                  case 2:
                       u8g2_0.drawStr(100,60,"2");
                  break;

                  case 3:
                       u8g2_0.drawStr(100,60,"3");
                  break;

                  case 4:
                       u8g2_0.drawStr(100,60,"4");
                  break;

                  case 5:
                       u8g2_0.drawStr(100,60,"5");
                  break;

                  case 6:
                       u8g2_0.drawStr(100,60,"6");
                  break;

                  case 7:
                       u8g2_0.drawStr(100,60,"7");
                  break;

                  case 8:
                       u8g2_0.drawStr(100,60,"8");
                  break;  

                  case 80:
                       u8g2_0.drawStr(100,60,"Err");
                  break;

                  case 20:
                       u8g2_0.drawStr(100,60,"D");
                  break;

                  case 30:
                       u8g2_0.drawStr(100,60,"R");
                  break;
   
                }
}


