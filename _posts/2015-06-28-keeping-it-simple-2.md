---
layout: post
title: "Keeping It Simple and Clean (Part 2)"
description: ""
category:
tags: [Simple, Software Engineering, Engineering Philosophy, Design Patterns, Refactoring]
---
{% include JB/setup %}

*[For part 1 click here!](/blog/2015/06/28/keeping-it-simple-1)*

#### Do NOT Repeat Yourself

Another way to ensure things will remain simple is the [DRY Principle](https://en.wikipedia.org/wiki/Don't_repeat_yourself) ("Don't Repeat Yourself"), which states *"every piece of knowledge must have a single, unambiguous, authoritative representation within a system".* Violating the DRY principle can hurt on many different levels; if a state is stored in multiple places, a piece of code is copied and pasted, or a logic is handled by multiple components. As a result the information stored by the program is likely to become incorrect, your code is prone to contagious bugs, or miscommunication between team members are made easier.

Every time you find yourself copy/pasting a piece of code only to change it slightly, think again about how a helper function can be extracted. If a small piece of algorithm or logic has to be repeated in more than one place, that's another candidate for refactoring. Except for special cases in distributed computing, no information should come from more than one source either. Even if the logic around it is obvious to you at this time, modifying the logic later will prove tricky to other team members or your future self.

#### Fundamentals: Data Structures and Algorithms

If you're a hobbyist or only getting your feet wet, you may be excused, but there's absolutely no excuse for any career programmer at any level to not learn computer science basics. I skip telling horror stories about huge mistakes made by people who hated math at school and assumed "theoretical stuff" are only for academics. One of the most important areas is data structures and algorithms.

One significant benefit of learning the basics is being aware of time and memory complexity. How long a piece of code might take, or how much space would it need? What if it's fast for my test data, but it becomes prohibitively expensive in production? Your textbooks provide simple tools to answer those questions, and even if you haven't read them, you can at least get familiar with a few common data structures and algorithms and know how they scale as the data grows. However, there will never be a right answer to out-of-context design questions involving time and memory complexity, because design decisions will almost always involve some trade-off between time, memory, development time, maintenance costs, etc.

Another benefit of getting familiar with the basics is being able to follow the flow of information. Where the information is stored, how it is stored or retrieved, and where and why it is needed are usually answered by being aware of the underlying data structures and their algorithms. Besides the theoretical aspects, there are also best practices that come from experience and other software engineering practices. For instance, it's established that [DOM](https://en.wikipedia.org/wiki/Document_Object_Model) is mostly used to represent data, and should not be used to store data (HTML5 data attributes aside).

#### YAGNI

"You Aren't Going To Need It". Very well said! If a feature is not required by business, or there's no certain near-future plans to add it, you wouldn't implement it. The same logic can be applied to lower-level design decisions. More often than not, people would like to look way ahead and over-design their systems for a rainy day when the dreaded future feature will come and they just kill it with their (over-)preparedness. The idea behind the YAGNI principle is that even if future is foreseeable, you shouldn't sacrifice the current code base because of that.

Like many other similar principles, it doesn't give us a certain recipe, but it tells us that, because of human nature, we're more likely to over-design than under-design.

#### Idioms and [Design Patterns](https://en.wikipedia.org/wiki/Software_design_pattern)

Standing on the shoulder of giants should feel good. Walking on a beaten path will tell you other people have gone down the same path. The same goes for time-tested patterns in programming. Design patterns and programming idioms specific to a tech stack are the gift of past engineers to us to deal with the same issues. The only downside is that it may not be obvious which patterns will fit our current needs. A pattern that perfectly answers a specifc situation, may be too complicated for another. Therefore, design patterns should always be used carefully.

Intermediate engineers, after learning a few design patterns, tend to over-use them. Obtaining the hammer of design patterns shouldn't deceive you into seeing every programming issue as a nail. Take caution and be fine with rolling your own, but certainly add them to your toolbox as they will often come in handy.


### Keeping It Clean

Writing clean code is more than an obsession with aesthetics or mindless principles. There are numerous papers on how having a consistent coding style will make programmers more productive. Just like a well-written novel, a clean code doesn't require excessive cognitive power to read and understand, and it saves more of your human RAM for important coding decisions.

A clean and consistent code base will save other people and your future self time in reading, debugging and modifying. I'm not going to expand on how to write clean code, but I felt it was important enough to deserve a mention. Here are a few bullet points on keeping the code clean and consistent:

* Code should be self-documented. The modules, the fields and methods, and even the variable names should make it obvious what they're storing or doing.

* Documentation. Not everything can be conveyed by writing self-documented code. Sometimes, adding a concise comment next to an inevitably complicated piece of code can help others understand it faster. In my opinion, external documentation should be saved for very specific cases like space shuttle or robotic surgery software. It's often unnecessary for the rest.

* Coding Style. This one tends to be ignored as a nuisance to sloppy programmers. The value of familiar style is mostly psychological, and it's shown to result in more productivity. If your team doesn't have a styling guide, just take one of the common ones and pitch it.

* Consistency. Humans are more efficient on auto-pilot, and the most important factor in achieving that is consistency. Consistency can be maintained on many levels from styling to naming to larger design patterns.

* Simplicity? Sometimes making a code more readable may go against keeping it simple. Again, it's an art to find the right balance. Adding an interim variable to put a name on a value in the middle of calculation is justifiable. Adding a whole class to store that value is not.

