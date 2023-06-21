import redis.clients.jedis.HostAndPort;
import java.io.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import redis.clients.jedis.JedisCluster;
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

        Set<HostAndPort> jedisClusterNodes = new HashSet<HostAndPort>();

        jedisClusterNodes.add(new HostAndPort("10.186.189.249", 6379));
        jedisClusterNodes.add(new HostAndPort("10.186.189.19", 6379));
        jedisClusterNodes.add(new HostAndPort("10.186.189.182", 6379));
        jedisClusterNodes.add(new HostAndPort("10.186.189.112", 6379));
        jedisClusterNodes.add(new HostAndPort("10.186.189.207", 6379));
        jedisClusterNodes.add(new HostAndPort("10.186.189.56", 6379));
        jedisClusterNodes.add(new HostAndPort("10.186.189.191", 6379));
        jedisClusterNodes.add(new HostAndPort("10.186.189.86", 6379));
        JedisCluster jc = new JedisCluster(jedisClusterNodes);


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
