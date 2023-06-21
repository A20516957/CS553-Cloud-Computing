import scala.Tuple2;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.sql.SparkSession;

import java.util.Arrays;
import java.util.List;


public final class SparkSort {
  private static final String dinu = "My code!";

  public static void main(String[] args) throws Exception {

    if (args.length < 2) {
      System.err.println("You need to specify the input and the output files");
      System.exit(1);
    }

	
	SparkConf sparkConf = new SparkConf().setAppName("SparkSort");
    JavaSparkContext jsc = new JavaSparkContext(sparkConf);
	
	double timer_start = System.currentTimeMillis();
	// the input file
	JavaRDD<String> textFile = jsc.textFile(args[0]);

	JavaPairRDD<String, String> SortedOp = textFile.flatMap(s -> Arrays.asList(s.split("\n")).iterator()).mapToPair(word -> new Tuple2<>(word.substring(0,10), word.substring(10,98))).reduceByKey((a, b) -> a + b );
	SortedOp.saveAsTextFile(args[1]);

	double timer_stop = System.currentTimeMillis();
	double total_time = timer_stop - timer_start;
	System.out.println("elapsed time:" + total_time);
  
  }
}