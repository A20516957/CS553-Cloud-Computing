package com.mycompany.app;
import java.io.File;  // Import the File class
import java.io.FileNotFoundException;  // Import this class to handle errors
import java.util.Scanner;
import java.lang.*;
import com.datastax.oss.driver.api.core.CqlSession;
import com.datastax.oss.driver.api.core.cql.*;
import java.net.InetSocketAddress;
import com.datastax.oss.driver.api.core.CqlIdentifier;



class App_delete
{
    public static void main( String[] args )
    {
	long s_time = System.currentTimeMillis();    //assigning variable
	float cn = 0;
  	String value="",key="";
	try {
      File myObj = new File("/home/ubuntu/cc/src/main/java/com/mycompany/app/data2.in");
      Scanner myReader = new Scanner(myObj);    //taking a scanner
      while (myReader.hasNextLine()) {
	String data = myReader.nextLine();
	for (int j =0; j<10;j++){

		if(data.indexOf("'")!=-1)
		key = key + "s";
		else
		key = key + data.charAt(j);
	}
	for (int j =11; j<(data.length());j++)
        value = value + data.charAt(j);
	try (CqlSession session = CqlSession.builder().addContactPoint(new InetSocketAddress("10.149.12.226",9042)).
                   withLocalDatacenter("datacenter1")
                   .withKeyspace(CqlIdentifier.fromCql("prod"))
                   .build()) {
                       	String select_query = "DELETE where id='"+key+"';";
			cn++;
			System.out.println("Completed "+(((cn*100.0)/500.0))+"%");
                   }
      key = "";
      value="";
      }
      myReader.close();
    } catch (FileNotFoundException e) {
      System.out.println(" error");
      e.printStackTrace();
    }
	long f_time = System.currentTimeMillis();
            long totaltime = f_time - s_time;
            System.out.println("Total time taken: " +totaltime);
            System.out.println("===============================================================================");
    }
}
