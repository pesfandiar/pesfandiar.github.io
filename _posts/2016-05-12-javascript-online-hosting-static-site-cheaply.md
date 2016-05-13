---
layout: post
title: "JavaScript Online: hosting a static site cheaply and effortlessly"
description: ""
category:
tags: [JavaScript, Static Website, Google Cloud Storage, Cloud Flare, Online Practice]
---
{% include JB/setup %}

A friend of mine was trying to get hired as a software developer, so he asked me about resources to hone his skills and practice for programming interviews. I came across a few websites where you could solve programming puzzles online. Your code would be sent to a server to run in a sandboxed container, and you would shortly get the result. My friend was specifically learning JavaScript. That made me wonder how cheaply and effortlessly I can create and maintain a similar site for JavaScript only.

The most expensive parts of hosting are usually tied to the computing power of servers. No server can beat the increasingly cheap option of not having a server at all. If your website can work with static resources, you can store everything you need on a storage service, configure your domain, spend a few cents a month, and never have to break a sweat about how your website is running. But how much can you accomplish with a static website?

In the case of an online JavaScript practice service, you can get away with not having a server for many things:

* **Core:** The core of your service, running users' code, can be delegated to their browsers. For this, web workers are a great feature that cleanly isolate potentially harmful code, and are supported in *modern* browsers.
* **Assets:** All assets on landing pages, blog posts, and other informational pages are static.
* **Security:** Users can see how you are running their code, see your test data for programming problems, and reverse-engineer them. In this case, let's agree, it is actually serving the purpose of teaching JavaScript.
* **Personalization:** Local Storage can be used to store the history of their problem-solving. This doesn't survive a browser data deletion or moving to another device, but oh well!
* **Community and Engagement:** I haven't added any feature of such nature yet, but there are free or cheap services like Disqus or SumoMe that can add comments or email collection widgets to your static page. I'm not aware of any service for a leaderboard, but I'm sure if the site becomes popular enough, I can roll my own AWS Lambda script to take care of that.

In order to create the site, I'm using a Node.js script. Jade templating engine, Markdown language, Uglify-js, and Express.js for a local test server have come in very handy. I've written an automated build script, where I only need to add a single JSON file for every problem, and it creates the whole page, adds it to the index and sitemap, and deploys new or updated files to Google Cloud Storage. Google Cloud Storage [makes it easy](https://cloud.google.com/storage/docs/hosting-static-website) to host a static website, but it doesn't support HTTPS yet. I'm using Cloudflare's free plan to add HTTPS, server-side analytics, a caching layer, denial-of-service protection, and even a widget to show warnings if a browser is not supported.

I might open-source this in the future, but for now, feel free to [practice JavaScript online for fun or technical interviews](https://www.javascript.onl)!