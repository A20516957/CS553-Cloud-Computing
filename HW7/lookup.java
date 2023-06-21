package com.mycompany.app;
import java.io.File;  // Import the File class
import java.io.FileNotFoundException;  // Import this class to handle errors
import java.util.Scanner;
import java.lang.*;
import com.datastax.oss.driver.api.core.CqlSession;
import com.datastax.oss.driver.api.core.cql.*;
import java.net.InetSocketAddress;
import com.datastax.oss.driver.api.core.CqlIdentifier;
/**
 * Hello world!
 *
 */
class App_lookup
{
    public static void main( String[] args )
    {
	long s_time = System.currentTimeMillis();
	float n = 0;
  	String value="",key="";

	try
  {
      File myObject = new File("/home/ubuntu/cc/src/main/java/com/mycompany/app/data2.in");
      Scanner myReader = new Scanner(myObject);
      while (myReader.hasNextLine()) {
	String data = myReader.nextLine();
	for (int j =0; j<10;j++){
		if(data.indexOf("'")!=-1)
		key += "s";
		else
		key += data.charAt(j);
	}
	for (int j =11; j<(data.length());j++)
        value += data.charAt(j);
	try (CqlSession session = CqlSession.builder().addContactPoint(new InetSocketAddress("10.149.12.226",9042)).
                   withLocalDatacenter("datacenter1")
                   .withKeyspace(CqlIdentifier.fromCql("prod"))
                   .build()) {
                       	String select_query = "SELECT * from gen where id='"+key+"';";
			n++;
			ResultSet result = session.execute(select_query);
			Row r = result.one();
			String v = r.getString("value");
                        System.out.println("OUTPUT: "+v);
			System.out.println("Completed "+(((n*100.0)/500.0))+"%");
                   }

      key = "";
      value="";
      }
      myReader.close();
    } catch (FileNotFoundException e) {
      System.out.println("An error occurred.");
      e.printStackTrace();
    }

	long f_time = System.currentTimeMillis();
            long totaltime = f_time - s_time;
            System.out.println("Total time taken: " +totaltime);
            System.out.println("===============================================================================");
    }
}
