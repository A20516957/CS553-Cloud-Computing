import redis.clients.jedis.HostAndPort;
import java.io.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import redis.clients.jedis.Jedis;
import java.util.*;
public class ReadData {

    private static String[] method(String filePath,int num_lines)
    {

        String[] array = new String[num_lines];

        Stringbuild1 build1 = new Stringbuild1();

        try (BufferedReader buffer = new BufferedReader(
                 new FileReader(filePath))) {
            int n = 0;
            String str;
            while ((str = buffer.readLine()) != null) {
                if(n>=num_lines) break;
                array[n]=str;
                n++;
            }
        }

        catch (IOException e) {
            e.printStackTrace();
        }
        return array;
    }
    public static void main(String[] args)
    {
        String filePath = args[0];
        int num_lines = Integer.parseInt(args[1]);


        String[] genString = method(filePath,num_lines);
        Jedis jc = new Jedis("10.186.189.77", 6379);

        long sTime = System.currentTimeMillis();
        for(String s : genString){
            jc.set(s.substring(0, 11),s.substring(12, 96));
        }
        long eTime = System.currentTimeMillis();
        System.out.println("Execution time for Insert: " + (eTime - sTime));


        sTime = System.currentTimeMillis();
        for(String s : genString){
            jc.get(s.substring(0, 11));
        }
        eTime = System.currentTimeMillis();
        System.out.println("Execution time for Lookup: " + (eTime - sTime));


        sTime = System.currentTimeMillis();
        for(String s : genString){
            jc.del(s.substring(0, 11));
        }
        eTime = System.currentTimeMillis();
        System.out.println("Execution time for deletion: " + (eTime - sTime));


    }
}
