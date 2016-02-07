---
layout: post
title: "I'm Open-Sourcing My Node.js App"
description: ""
category:
tags: [Side Project, Web Applications, Open Source, Node.js, Express.js, Passport.js]
---
{% include JB/setup %}

After over one year of working on my side project and trying to monetize it, I've decided to call it quits and share my journey and the source code with the community ([Github](https://github.com/pesfandiar/Phonjour)). In this post, I'm going to explain the whole development process, describe my failed attempts, and get into more technical details in case someone wants to use the app as a boilerplate to create their own.

## The Idea

I was itching to create my own software-as-a-service web application while working full time. After all, the idea of having a few sources of passive income is frequently sold to software developers, and I wanted to take part in the "micropreneurship" revolution. I also wanted to practice time management and prove to myself that I can build something from scratch, and possibly have an extra source of income. There's no need to mention the rewarding sense of building things on top of all that.

The idea of an automated virtual phone system is neither original nor sexy. In fact, there are many companies out there that are doing the exact same thing (e.g. Grasshopper, eVoice, MightyCall, etc.). This meant I might be able to take over a very small part of an established market. I knew Twilio provided an easy-to-use API, so I just had to do the math and see how high the barrier to entry is.

## Research and Design

I researched the feature set of top players. I gathered a list of features that I thought I could implement in a few months and prioritized them. I then used KickoffLabs to create a fake landing page advertising all those features. Next thing I needed to do was to buy some paid traffic and measure what percentage of the visitors are willing to press the "Get Started" button. Even though pressing that button doesn't mean the intention to pay, it indicates the pitch is compelling enough for them to spare a few seconds of their time.

When I got a good conversion rate on the fake landing page (and collected a few email addresses of prospects), and compared the cost of acquiring new customers to their potential lifetime value, I started the design phase. At this stage, I carefully listed all possible features from competitors, and added some of what I thought a potential customer would need based on pure speculation. Then, I sorted them by how important they are and how much time each one of them would take me. The outcome of this stage was a high-level list of features that my minimum viable product (MVP) needed.

The next stage was technical design. I decided to use Express.js framework on Node.js, and use Twilio and Stripe as external service providers for telecom and payment processing respectively. The free tier of Amazon Web Services provided enough computing power to get a staging server up and running. I tried not to use anything too close to the cutting edge, and compromised with more established libraries like jQuery, but I leave the details for the "Under The Hood" section of this post. The technical design was based on an MVC architecture, and I only needed pen and paper to document the database design and screen mockups.

## The Interesting Part: Coding!

As many other fellow software engineers can agree, coding is the most interesting part of the process. It's certainly not the most important part though. Having been coding for 15 years, I found this part well inside my comfort zone. The real challenge was time management and staying focused. Working full time and the human need to be offline leave too little time for extra screen time. However, what really helped me was the great power of habits. I spent an hour or two every single day. No exceptions. If I was very tired or feeling down, I would work on the parts that didn't need much attention, but the trick was to keep up the streak.

I used Trello to break down the tasks and keep track of my progress. As expected, I had to deal with numerous unexpected issues. Any line of code that you create or comes with external libraries, and any API call to service providers is a potential headache. I took pleasure in solving them one at a time, and I think the source code at its current state contains a wealth of solved problems that I'll certainly use in my future projects.

I asked a few friends to beta-test the app for me. I applied their feedback and fixed a few bugs. The development took 3-4 months, and I was ready to launch the product after all. I used a free Bootstrap theme to design the landing page. The only money spent so far was about $40 to get the logo designed and register the domain.

## Going to The Market

To an engineering-minded person without any serious business experience, this part is the most daunting. To a business-savvy person, this is probably the most important part. Even though engineers like to make fun of all those "I have a great idea and will give 5% equity to an engineer to implement it" Craigslist ads, a person that can execute well on the idea is rarer than a 10X unicorn engineer.

So I got started by creating a couple of social media accounts, and buying some paid trafic from Google search network. The way my app was designed required users to enter their credit card number before buying a virtual phone number. I was too afraid of abuse, and didn't want to give away too much freebies to acquire new customers. The first campaign showed that I need to be more open. A good number of people signed up, but all of them stopped at the payment screen.

I added a couple of free trial options. I also commissioned a friend to create a low-budget explainer video. I also changed the on-boarding workflow so users can see the control panel before entering their credit card number. These changes allowed more people to get into later stages of the sales funnel. However, none of the users that bought a phone number started paying for it after the trial period.

I guess I didn't have enough motivation to try inbound marketing, or cold-calling potential customers. They were certainly outside my comfort zone. Attempts at selling the website on Flippa or finding someone to help me with sales and marketing didn't go anywhere either. The amount of effort that it takes to get any traction for this web app seemed too high, and it eventually led me to open-source it and move on.

## Under The Hood: [Source Code](https://github.com/pesfandiar/Phonjour)

I used Express.js on Node.js for back-end, and Bootstrap and Knockout.js for front-end. Twilio was the obvious choice for telecom API, and I hooked up my app to Stripe for payment processing. I deferred writing tests for when I had paying customers, but the software was designed robust enough to evolve into a solid application once it was proven in the market. I used exact versioning in my NPM manifest file, and included all the packages in the Git repository to remove any incompatibility or NPM infrastructure risks.

The architecture on the back-end is MVC, and the front-end uses AJAX calls to populate the page. The server merely sends a Knockout-enabled page to the client, which in turn calls an API command or two to get the required data in JSON format. By this design, I avoided the headache that managing a single-page application will give you, and provided a blank page for users as soon as possible. I think users like to see "somehintg" (the loading indicator) very soon, but show more patience for loading data after that.

There are many opportunities to refactor the code (especially in front-end views), and there are certainly bugs to be found. However, here's a list of different libraries and components that I used or wrote:

* Express.js: Express is a great web framework, and the rich ecosystem of middleware allows you to shop around for best solutions to common needs.
* Passport: A useful library for user authentication. I had to write my own session store that uses MySql (session_store.js), and use bcrypt to encrypt passwords, but Passport pretty much provided the rest.
* Sequelize: It's a good ORM that you can use for data modelling. Express doesn't come with an ORM, so I had to pick between this or Bookshelf. I finally went with Sequelize, and it worked just fine for me. Getting entity relationships right was a bit tricky though.
* Q: This promise/async library helped a lot with avoiding the infamous Node.js callback hell.
* BigNumber.js: When it comes to money calculations, you shouldn't trust JavaScript at all. To avoid using their float numbers, I used BigNumber. It's a bit slow to write simple math, but the reliability makes it worthwhile.
* Connect-assets: This is an older asset packaging library. It minifies and aggregates Javascript files, and compiles stylesheet files. That's good enough for smaller web apps.
* Moment: The absolute best when it comes to manipulating dates. Its timezone library made working with timezones a breeze.
* Jade: I used this for my views. It removes the HTML clutter.
* NodeMailer: I wrote utility files to compile Jade files, and send emails via Amazon's mail service.
* Twilio: If you're using Twilio, their Node.js library is a no-brainer.
* Stripe: Not having to deal with storing credit cards is great. They provide an easy API, and all you need to store and work with is tokens instead of sensitive user information.
* secret_sauce.js: This is a "secret" API call that I thought might be useful for invoking certain scheduled tasks, for instance cleaning up expired sessions, or charging customers. I never got to the stage that required automating them though.
* Bootstrap: Some people may be allergic to Bootstrap at this time, but it gives you a generic well-designed-looking page for cheap.
* Knockout.js: This is great for front-end data binding. If all you have to do is deal with a few forms on your page as opposed to a full-on single-page app, Knockout.js is your friend!

## Conclusion

Creating Phonjour was a great learning experience for me. I don't regret putting effort into it, and I hope the journey and the code scraps can be useful to others as well. In my future attempts at a side project, I'll try to find a business-savvy person to take care of the business side.