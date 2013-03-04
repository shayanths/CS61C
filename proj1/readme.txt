CS61C Fall 2012 Project 1: MapReduce in the Cloud

TA's: Alan Christopher, Ravi Punj


Intermediate milestone due:  09/16@23:59:59 PST
Due:  09/23@23:59:59 PST


Clarifications, Notifications, and Exclamations



Your algorithm must run correctly with or without your combiner running. If you implement an extremely simple reduce function in one of your jobs you can omit a combiner for that job.

There's been a lot of confusion about what exactly we're calculating for generalized co-occurrence. Hopefully this will help shed some light on the situation.

It's ok if you truncate your output at the first 100 words or so, as long as you output at least the first 100 (we aren't fussy about how you do this).

We have made some sample outputs from the reference solution for you guys to test against. The files are jury-0 (target="jury", f="0"), freedom-0(target="freedom", f="0"), freedom-1(target="freedom", f="1"), shallnot-0(target="shall not", f="0"), and shallnot-1(target="shall not", f="1"). All of the files were run on billOfRights.txt.seq and can be pulled in from proj1 skeleton repository.

You should not be calculating self-co-occurence for this project. That is, do not output the target gram and it's co-occurence rate with itself.

For this project, the n in n-gram will be inferred from the target gram. For example, if you have as your target gram "hello goodbye" you'll be doing analysis on 2-grams.

Floating point arithmetic isn't perfectly precise. It's fine if your numeric co-occurrence results disagree with ours by a few parts per billion or whatever. We will test your program in a way that won't be sensitive to these small numerical inaccuracies. So don't worry about the last few trailing decimal place.

If you get a messages like "<date> INFO ipc.Client: Retrying connect to server: ec2-123-45-67-89.amazonaws..." (possibly followed by an exception and a long stack trace), please following the second set of instructions under the heading before doing something expensive and time-consuming like starting a new EC2 cluster.

Overview

For this project, you're finally going to roll up your sleeves and use MapReduce to answer a big data problem.

The question you're going to answer is this: given a word, what other words are statistically associated with it? If I say 'love', or 'death', or 'terrorism', what other words and concepts go with it?

A reasonable statistical definition is: 

    Let Aw be the number of occurrences of w in the corpus.
    Let Cw be the number of occurrences of w in documents that also have the target word.
   Co-occurrence rate :=  if(Cw > 0)   Cw * (log(Cw))3  / Aw 
                                    else  0

Here is an example that illustrates this definition. Let the target word be "Dave"

Doc_ID#1: Randy, Krste, Dave
Doc_ID#2: Randy, Randy, Dave
Doc_ID#3: Dave, Krste, Randy

Occurrences: ARandy = 4; AKrste = 2; ADave = 3; 
Co-occurrences: CRandy = 4; CKrste = 2; 
Co-occurrence Rate:
with Randy: CRandy * (log(CRandy))3 / ARandy = (log(4))3 = 2.664
with Krste: CKrste * (log(CKrste))3 / AKrste = (log(2))3 = 0.333

This does nothing to account for the distance between words however. A fairly straightforward generalization of the problem is to, instead of giving each co-occurence a value of 1, give it a value f(d), where d is the minimum distance (number of spaces separating the word occurrences, infinity if one of the words is not in the document) from the word occurrence to the nearest instance of the target word. To make our definition cleaner we will restrict our choice of f so that f sends infinity to 0 and positive numbers to numbers greater than or equal to one. The result of the refinement is as follows:

    Let W be the set of all instances of a given word in the corpus
    Let Sw be the the sum of f(dw) over all w in W .
   Co-occurrence rate :=  if(Sw > 0)   Sw * (log(Sw))3  / |W| 
                                    else  0

Here's another generalization, instead of only looking at how words relate to one another, we can look at how phrases relate to one another. To do that we will look a sequences of n words, or an n-gram, instead of just one word, which is a 1-gram. To do this we will instead of defining a target word define a target gram, and we will define the distance between two n-gram instances to be the number of spaces between their left-most word or infinity if they are not in the same document. Thus, the final refinement of our model is:

    Let G be the set of all instances of a given n-gram in the corpus
    Let Sg be the the sum of f(dg) over all g in G .
   Co-occurrence rate :=  if(Sg > 0)   Sg * (log(Sg))3  / |G| 
                                    else  0

Your task is to produce an ordered list of n-grams for the target gram sorted by generalized Co-occurrence rate.

Your list should be ordered with the biggest co-occurrence rates at the top.

This isn't the most sophisticated text-analysis algorithm out there. But it's enough to illustrate what you can do with MapReduce.

The data will be the same Usenet corpus as in Lab 3 (and in turn, the same format as in Lab 2). Hence, the test data supplied for Lab 2 will also work here.

Do this project individually. You may not share code with other students or read other students' code. No exceptions.
     
Detailed Directions

We'll give you skeleton code for two MapReduce jobs and a Makefile in a git repo at ~cs61c/proj/fa12/01. You should pull from that repository into your working repository (e.g. git pull ~cs61c/proj/fa12/01 master), and then start modifying your copy of Proj1.java. Both jobs live in the same source file. That directory will also include  some test data which will contain some of the results from our reference implementation. You should get the same output as we did.

We'll also give you a program (Importer.java) that you can use to convert text files to the appropriate sequence file format. This will let you create your own test data.

Your solution should implement the algorithm described above, with the exception that we will only be implementing three different f's for convenience. Still, it should be trivial to modify your code to accept more f's. Our reference solution does define new classes and methods, and we encourage you to do the same so that your code remains readable.  You should not need to change much in main, but it is likely that you will want to change the type signatures of your MR jobs, and this requires some modifications to main. We've commented the regions we expect you to need to edit.

Note that Hadoop guarantees that a reduce task receives its keys in sorted order. You may (should!) use this fact in your implementation.

It's possible that some Map or Reduce phases will be the identity function. This is the default behavior if you don't override the base class map() and reduce(). (aka if you just delete the map or reduce function.)

As you parse the text, you should convert it to lower case. Have a look at the Javadoc for java.lang.String here for a Java method that should be helpful to you. You may also need to use Java's HashMap and Math classes.

We've defined Combiner functions for both jobs. These are disabled by default; you can turn them on by specifying "-Dcombiner=true" on the command line. You should try to write the code to make use of these combiners. It'll make your job run faster, and more importantly you can't get full credit without implementing a non-trivial combiner. It shouldn't be hard — our combiner implementation was just a handful of lines.

The framework expects you to output (from the second job) a DoubleWritable and Text pair. These should be the score for each word and the word itself, respectively.


Running things locally

The way you should launch the jobs is via:

    hadoop jar proj1.jar Proj1 -conf conf.xml <input> <intermediateDir> <outDir>

You should edit conf.xml to refer to the target gram and f for the analysis (the three f's we'll be using are numbered 0, 1, and 2), and <intermediateDir> and <outDir> can be whatever paths you like.

The -D syntax sets a Hadoop configuration parameter, and the -conf syntax specifies an xml which defines multiple configuration parameters simultaneously. Inside your job, you can retrieve this parameter via context.getConfiguration().get("targetGram"). The framework we give you has this code already there for you.

The intermediate directory will hold the output from the first MapReduce job, which is used as input for the second job. This may be helpful in debugging.

The framework we give you supports two other options: Specifiying -Dcombiner=true will turn on the combiner. Specifiying -DrunJob2=false will cause only the first job to run, and will cause its output to be in a friendly (Text) format instead of as sequence files. This is intended for debugging.
    

Make sure you delete the intermediate and final outputs between runs.


STOP.  You have now done enough work for the intermediate checkpoint, due on the 16th. For the intermediate milestone, submit the contents of your proj1-1 directory with the tag proj1-1.

Before submitting, you should verify that the invoke command above causes your code to run correctly. We'll be using that command, or one very similar to it, for our grading.

We're going to grade your code by running it in an automated test harness on sample data. We expect you to give [approximately] the right numeric answers, barring floating point roundoff. So don't try to improve on the formula we give you to get "better" results (you are free to toy around with additional f's, but do not modify the standard three).  We promise to avoid tests that are overly sensitive to roundoff.

Running things in the cloud (Proj1-2)


PLEASE START EARLY. Only a certain number of clusters can be initialized under the cs61c master account. That means that if you procrastinate on the project right now, there's a good chance that you won't be able to start a cluster easily towards the end of the week, when traffic is heaviest.

We expect you to test your code in the cloud. This should be a fun experience, not a chore. You're making machines hundreds of miles away jump to your bidding. And you don't even have to pay for it. Berkeley, however, does have to pay for it so please try to be cost-conscious.

Start by creating a proj1-2 directory in your work directory, and copying files over to the proj1-2 directory. You can use the following commands to achieve this:

        mkdir ~/<working directory>/proj1-2
        cp -r ~/<working directory>/proj1-1/* ~/<working directory>/proj1-2/

On EC2 servers, Hadoop assigns LongWritable document IDs, instead of Text. So, you should change the signature of the Map1 class and its map function. Changing the input key class to org.apache.hadoop.io.LongWritable or org.apache.hadoop.io.WritableComparable will work.

For this assignment, we want you to run your job a total six times in the cloud.

You'll be exclusively using "large" instances. For each of the three n-grams, and funcNum pairs { ("jurisdiction", 0), ("court order", 1), ("in my opinion", 2) } run using both 5 workers and then 9 workers in the cluster.  Be sure to save the output and the job status pages (so you can determine input size and runtime) for each run. Once you terminate a cluster, you will not be able to access the logs for that cluster, and neither Hadoop's distributed filesystem.

We estimate that each run with 5 workers will take around 15-20 minutes (a few minutes less with a combiner).

Do not leave EC2 machines running when you are not using them. The virtual machines cost the same amount of money when they are active as when they are sitting idle.


The EC2 usage instructions are substantially the same as those in Lab 3.
   If you haven't already setup your account for EC2 usage using:
        bash ~/<working directory>/lab03/ec2-init.sh
        source ~/ec2-environment.sh

   To start a cluster with N workers, say:
        hadoop-ec2 launch-cluster --auto-shutdown=230 <cluster name> N

   To redisplay the web interface URLs use:
        hadoop-ec2 list <cluster name>

   You can then start the job on this cluster via: 
        hadoop-ec2 proxy <cluster name>

        hc <cluster name> jar proj1.jar Proj1 -conf conf.xml -Dcombiner=<true/false> -DrunJob2=<true/false> s3n://cs61cUsenet/s2006 hdfs:///<intermediateDir> hdfs:///<outDir>

Remember to terminate your cluster when you're done.

After the job is complete you could in principle retrieve your output with the command

    hc large dfs -cp hdfs://FILE_NAME <dst>

For the final submission, answer the following questions in a file named ec2experience.txt:

On the big dataset, what were the top 20 words by relevance for each of these n-grams, and funcNum pairs: ("jurisdiction", 0), ("court order", 1) ("in my opinion", 2)?
How long did each run of program take on each number of instances? How many mappers, and how many reducers did you use?
What was the median processing rate per GB (= 2^30 bytes) of input for the tests using 5 workers?  Using 9 workers?
What percentage speedup did you get using 9 workers instead of 5? How well, in your opinion, does Hadoop parallelize your code?
What was the price per GB processed? (Recall that an extra-large instance costs $0.68 per hour, rounded up to the nearest hour.)
How many dollars in EC2 credits did you use to complete this project? (Please don't use ec2-usage, as it tends to return bogus costs. You should work out the math yourself.)
Extra credit: did you use a combiner? What does it do, specifically, in your implementation?

    If you find bugs in your java code, you can fix them after the milestone and before the final submission.

When you're done with a cluster (after you have retrieved the output files, and copied the logs), be sure to shut down your cluster via

    hadoop-ec2 terminate-cluster large

Do not leave instances running when you are not using them. (You can always check whether you have instances running using 'hadoop-ec2 list' or 'ec2-my-instances'.)

For the final results, you will be submitting two files:  Your Proj1.java and the ec2experience.txt. Make sure they are in the ~/<working directory>/proj1-2 directory. Tag them as "proj1-2", and push them to your remote repository.

The deadline for submission is 11:59PM on 9/23/2012 (Sunday).

When things go wrong
Stopping running Hadoop jobs

Often it is useful to kill a job. Stopping the Java program that launches the job is not enough; Hadoop on the cluster will continue running the job without it. The command to tell Hadoop to kill a job is:
    
    hc large job -kill JOBID

where JOBID is a string like "job_201101121010_0002" that you can get from the output of your console log or the web interface. You can also list jobs using 

      hc large job -list
Proxy problems

"12/34/56 12:34:56 INFO ipc.Client: Retrying connect to server: ec2-123-45-67-89.amazonaws....."

or
"Exception in thread "main" java.io.IOException: Call to ec2-123-45-67-89.compute-1.amazonaws.com/123.45.67.89:8020 failed on local exception: java.net.SocketException: Connection refused

<long stack trace>"


If you get this error from 'hc' try running

      hadoop-ec2 proxy large

again. If you continue getting this error from hc after doing that, check that your cluster is still running using
    
    hadoop-ec2 list large

and by making sure the web interface is accessible.
Deleting configuration files

If you've accidentally deleted one of the configuration files created by ec2-init.sh, you can recreate it by rerunning bash ec2-init.sh.
Last resort

It's okay to stop and restart a cluster if things are broken. But it wastes time and money.
Resources


Most the code you need to write for this project could fit inside the map and reduce functions that we gave you in the skeleton. As a result, we're shielding you from the full complexity of the Hadoop APIs.

But if you need to dive deeper, there are a bunch of resources about MapReduce available on the web. It's okay [encouraged!] to use them. But be warned.  Hadoop changed its APIs in a backward in-compatible ways going from v19 to v21. The old API is under org.apache.hadoop.mapred. The new API is under org.apache.hadoop.mapreduce.  Version 20 is somewhere caught in the middle and the documentation is muddled in some places. You may need to tinker a bit to get things working.  With those caveats:

The Hadoop Javadoc for v20:  http://hadoop.apache.org/common/docs/r0.20.0/api/index.html
    [Note: The API part is machine-generated and basically correct. Some of the descriptive text for each class is stale/misleading. Watch out.]
The tutorial for v21:   http://hadoop.apache.org/mapreduce/docs/r0.21.0/mapred_tutorial.html


Some of you may be wondering why we're having you use tools that are this rough. The answer is, Hadoop is basically the only big-data tool out there. Yahoo!, Facebook, and Twitter all have bet their companies on using it. And, while Hadoop is pretty painful to use compared to polished consumer software, it's fairly representative of the just-good-enough-to-use tools that make modern Internet companies work. Welcome to computer science without the training wheels.    


Appendix: Miscellaneous EC2-related Commands

We don't think you'll need any of these...

Terminating/Listing Instances Manually
You can get a raw list of all virtual machines you have running using

    ec2-my-instances

This will include the public DNS name (starts with "ec2-" and ends with "amazonaws.com") and the private name (starts with "ip-...") of each virtual machine you are running, as well as whether it is running or shutting down or recently terminated, its type, the SSH key associated with it (probably USERNAME-default) and the instance ID, which looks like "i-abcdef12". You can use this instance ID to manually shut down an individual machine:

    ec2-terminate-instances i-abcdef12

Note that this command will not ask for confirmation. ec2-terminate-instances comes from the EC2 command line tools. ec2-my-instances is an alias for the command line tools' ec2-describe-instances command with options to only show your instances rather than the whole class's.

Listing/removing/moving files from S3
    
    s3cmd ls s3://cs61c/USERNAME/...
    s3cmd del s3://cs61c/USERNAME/...
    s3cmd mv s3://cs61c/USERNAME/... s3://... (OR to/from local file)

You can use glob patterns (e.g. s3://cs61c/USERNAME/out*) on S3 if you quote them. Note that you can delete/view other people's files from S3. Please do not abuse this.
Logging into your EC2 virtual machines

    hadoop-ec2 login large
    # or using a machine name listed by ec2-my-instances or hadoop-ec2 list
    ssh-nocheck -i ~/USERNAME-default.pem root@ec2-....amazonaws.com

The cluster you start is composed of ordinary Linux virtual machines. The file ~/USERNAME-default.pem is the private part of an SSH keypair for the machines you have started.
Viewing/changing your AWS credentials

You can view your AWS access key + secret access key using:
    ec2-util --list
If you somehow lose control of your AWS credentials, you can get new AWS access keys using
    ec2-util --rotate-secret 
    new-ec2-certificate
This is likely to break any running instances you have.

