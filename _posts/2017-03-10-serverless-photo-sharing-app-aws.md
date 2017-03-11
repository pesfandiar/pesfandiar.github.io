---
layout: post
title: "Serverless Photo-Sharing App Using Amazon Web Services"
description: ""
category:
tags: [JavaScript, Static Website, Amazon Web Services, Cloud, Serverless]
---
{% include JB/setup %}

As the self-appointed IT director of my new family, I was tasked with finding the best solution to easily upload, back up, and share pictures and videos of our newborn daughter. While there are a myriad of cloud services that a normal person would go for, I didn't want to rely on them to safeguard our most precious moments. To be honest, I was also itching to create a serverless app on AWS, without committing to much cost or maintenance overhead.

AWS has a host of relatively cheap services that makes creating small serverless apps easy. The JavaScript API allows you to keep most of the logic in your browser, where you can access AWS directly or through a proxy (e.g. API Gateway). I'll explain how I used them:

* **Cognito for authentication:** The main users of this app were my family and friends, so I only had to worry about unintended mistakes as opposed to malicious abuse. This allowed me to create pre-defined users for different roles (e.g. admin and visitor), and use Cognito to authenticate them. The JavaScript API lets you safely hard-code the public codes in the web page, and log in using a password only.

* **IAM for authorization:** Once users are authenticated, IAM should give them minimal privileges to do their tasks. For example, I gave file upload access to admin users, but only read access to visitors. [The Principle of Least Privilege](https://en.wikipedia.org/wiki/Principle_of_least_privilege) prevents users from wreaking havoc. IAM didn't have the finest-grained access levels, but for a trusted set of users, that should be good enough. For more flexibility in authorization, of course, it has to be done on a proxy or web server.

* **S3 for storage:** Amazon's simple storage is truly simple to use. I used it to store media files, thumbnails, and the static website assets. You may make the static site public, but put media files behind Cognito. The nice thing about S3+Cognito is that you can use the Cognito token in the S3 URL and access it in your website as you normally would with hosted images.

* **DynamoDB for database:** The list of galleries and files, timestamps, captions, user access, and comments have to be stored somewhere. While S3 provides limited ability to store metadata for each file, the permanently-free tier of DynamoDB has enough capacity to store them in a few tables. The NoSQL (or rather schemaless) nature of the database makes it easy to quickly add new features.

* **Lambda for processing:** A serverless architecture will not be complete without a function-as-a-service component! In this case, I used a S3-triggered function to create an entry in the database and process newly-uploaded images and videos. It could do anything as simple as generating thumbnails (Lambda is pre-packed with ImageMagick), or dealing with EXIF idiosyncrasies.

As for front-end, there wasn't much needed for this app. Besides the good ol' AWS JavaScript API, I used Bootstrap for easy styling, Knockout for two-way data binding, and [Unite Gallery](http://unitegallery.net) for media viewer. Unite Gallery is an open-source library with a few available themes and easy setup. However, getting videos to play on mobile and JPEG EXIF orientation proved to be challenging.

If I found time to improve the app, these areas would come up next:

* **CloudFormation:** As of now, most of the configuration has been manually done and its persistence is at the mercy of AWS. I can use CloudFormation to formulate how everything was set up, so it can be retrieved if anything goes wrong. Amazon provides CloudFormer to create a template from an existing architecture, but it didn't cover the bulk of my configuration around Cognito and security policies.

* **Automatic rotation:** not all browsers can show the right orientation of cellphone images based on their metadata. I can use a Lambda function to automatically rotate images based on their EXIF orientation field.

* **API Gateway:** Using the combination of API Gateway and Lambda, there would be no need to give users direct access to AWS resources. This will improve the security and may make the app useful for other people who can have a more serious use case for it.

* **Backup:** A scheduled task that backs up media files and database records and writes them onto cheaper storage (e.g. AWS Glacier). For a more paranoid approach, I can also store them on another cloud service provider such as Google Cloud.

I'd be happy to get feedback on the design, and ways to improve the architecture.