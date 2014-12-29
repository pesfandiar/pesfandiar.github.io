---
layout: post
title: "Crawling/Scraping with Apache Nutch on AWS"
description: ""
category: 
tags: [Nutch, Hadoop, AWS, Crawling, Scraping, MapReduce, HDFS]
---
{% include JB/setup %}

As I was waiting to start a new job in January, I took on a consulting gig to implement a scalable web crawling/scraping system for a company in Vancouver. My client insisted it has to be done using Apache Nutch, which is a great tool for high-scale scraping.

Even though Nutch is first and foremost a web search engine, its "fetcher" component remains a best-in-class web crawler. In fact, Lucene and Hadoop were originally two components of Nutch if this indicates anything about Nutch's quality. The plugin-based architecture of Nutch allowed me to easily cherry pick crawling-related parts, and plug in custom indexers for my client.

Another reasonable requirement by my client was using Amazon Web Services (AWS) for running the crawler. Without that requirement, I would advise the same too. AWS Elastic MapReduce (EMR) makes it incredibly easy and cheap to start a Hadoop cluster, and it's seamlessly integrated with other relevant AWS services such as S3. Using EMR is slightly more expensive than setting up your own Hadoop cluster on EC2, and there are a few quirks (e.g. you cannot use the cheaper generation of small and medium instance types), but the reduction in labour costs and general headache very well justifies it.

Nutch's plugin system conveniently provides a few [extension points](http://wiki.apache.org/nutch/AboutPlugins), where you can hook up your plugins, and they'll be called at the right stage with all the available information. I only needed to extend HtmlParseFilter to scrape data off of HTML files, and IndexingFilter and IndexWriter to store the extracted data on my client's existing database. HtmlParseFilter provides the HTML file's source, and in this case, regular expressions were powerful enough for me. If HTML parsing is required, the highly resilient [TagSoup](http://home.ccil.org/~cowan/tagsoup/) is shipped. You can even use the likes of Selenium to bypass scraper-deterring AJAX calls.

Once I finished writing and testing the plugins locally, it was time to deploy them on a Hadoop cluster. The process is seamless for the most part. There's probably no software more native to Hadoop than its parent, Nutch. [This Makefile](https://github.com/eleflow/nutch-aws) is a good start for writing deployment scripts. However, it doesn't readily work since the AWS command line interface has changed. Another obstacle in deployment was a [flaw in Nutch](https://issues.apache.org/jira/browse/NUTCH-937), where your plugin binaries were not present on cluster slaves, so I had to extract and copy the JAR files to the shared filesystem (HDFS) as a bootstrap action.

After overcoming configuration hurdles, the first scraping adventure went really well. It took 10's of hours on a cluster of 5 smaller slaves for a particular retailer's website. Scraping is IO-bound in general and doesn't need heavy-duty machines. Combine that with AWS spot instances and it ends up dirt cheap. I didn't touch most of the default configurations, and it seemed the bottleneck was the built-in minimum wait for politeness to the target servers. Nutch's crawl artifacts are also saved on HDFS and can be easily saved on S3 in Snappy format for later analysis.

As the last words, Nutch provides all conventional features for responsible crawling/scraping; from robots.txt compliance, to custom user agent, to minimum delay between calls. Have fun!
