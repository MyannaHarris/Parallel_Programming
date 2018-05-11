import java.io.IOException;
import java.util.*;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;
import org.apache.hadoop.util.*;

public class TermPairCount 
{

    public static class Map extends MapReduceBase implements Mapper<LongWritable, Text, Text, IntWritable> 
    {
		
        private ArrayList unique = new ArrayList();
		private Text newKey = new Text();
 
        public void map(LongWritable key, Text value, OutputCollector<Text, IntWritable> output, Reporter reporter) throws IOException 
        {
			newKey.set(key.toString());
            String line = value.toString();
            StringTokenizer tokenizer = new StringTokenizer(line);
            while (tokenizer.hasMoreTokens())
            {
				String word = new String(tokenizer.nextToken());
				if (unique.contains(word)==false){
					unique.add(word);
					IntWritable CID = new IntWritable(Integer.parseInt(word));
					output.collect(newKey, CID);
				}
			}
        }
    }

   public static class Reduce extends MapReduceBase implements Reducer<Text, IntWritable, Text, Text> 
    {
		private Text conceptList = new Text();
		private ArrayList unique = new ArrayList();
        public void reduce(Text key, Iterator<IntWritable> values, OutputCollector<Text, Text> output, Reporter reporter) throws IOException 
        {
			while (values.hasNext()) 
            {
				int tempval = new Integer(values.next().get());
                unique.add(tempval);
            }
			QuickSort q = new QuickSort();
			unique = q.quicksort(unique);
			
			StringBuilder conceptPairs = new StringBuilder();
            for (int i = 0; i<unique.size()-1; i++){
				for (int j = i+1; j<unique.size()-1;j++)
				{
					conceptPairs.append("(");
					conceptPairs.append(unique.get(i));
					conceptPairs.append(", ");
					conceptPairs.append(unique.get(j));
					conceptPairs.append("), ");
				}
			}
			conceptList.set(conceptPairs.toString());
			output.collect(key, conceptList);
        }
    }


    public static void main(String[] args) throws Exception 
    {
        JobConf conf = new JobConf(TermPairCount.class);

        conf.setJobName("TermPairCount");
        conf.setOutputKeyClass(Text.class);
        conf.setOutputValueClass(IntWritable.class);
        conf.setMapperClass(Map.class);
        conf.setReducerClass(Reduce.class);
        conf.setInputFormat(TextInputFormat.class);
        conf.setOutputFormat(TextOutputFormat.class);
        FileInputFormat.setInputPaths(conf, new Path(args[0]));
        FileOutputFormat.setOutputPath(conf, new Path(args[1]));

        JobClient.runJob(conf);
   }
}
