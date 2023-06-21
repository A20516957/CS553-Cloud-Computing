package com.bhavya.app;

import java.io.File;  // Import the File class
import java.io.FileNotFoundException;  // Import this class to handle errors
import java.util.Scanner;
import java.lang.*;
import com.datstax.oss.driver.api.core.CqlSession;
import com.datstax.oss.driver.api.core.cql.*;
import java.net.InetSocketAddress;
import com.datstax.oss.driver.api.core.CqlIdentifier;
 //
public class App
{
    public static void main( String[] args )
    {
	long s_time = System.currentTimeMillis();


	float cn = 0;
  	String value="",key="";
	try
  {
      File obj = new File("dat50.in");
      
      Scanner read = new Scanner(obj);
      while (read.hasNextLine()) {String dat = read.nextLine();
	for (int j =0; j<10;j++){ if(dat.indexOf("'")!=-1) {key += "s";}
		else {key += dat.charAt(j); }
	}
	
  
  for (int j =11; j<(dat.length());j++)
        value += dat.charAt(j);
	
  
        try (CqlSession session = CqlSession.builder().addContactPoint(new InetSocketAddress("10.149.12.226",9042)).
                   withLocaldatcenter("datcenter1")
                   .withKeyspace(CqlIdentifier.fromCql("prod"))
                   .build()) {                                  // (1)
                        String query3 = "INSERT INTO gen(id, value) VALUES('"+key+"', '"+value+"');" ;    //add more values at the end to make 90 bytes
                       	cn++;
			session.execute(query3);
                        System.out.println("Done"+(((cn*100.0)/500.0))+"%");
                   }

      key = "";
      value = "";
      }
      read.close();
    } catch (FileNotFoundException e) {
      System.out.println("An error occurred.");
      e.printStackTrace();
    }



	long f_time = System.currentTimeMillis();
            long totaltime = f_time - s_time;
            System.out.println("time: " +totaltime);
            
    }
}
